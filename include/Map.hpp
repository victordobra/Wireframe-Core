#pragma once

#include "Defines.hpp"
#include "Debug.hpp"
#include "Memory.hpp"
#include "Pair.hpp"
#include <initializer_list>
#include <map>

namespace wfe {
	/// @brief Holds a map of values, each value having a key.
	/// @tparam Key The type for the value's keys.
	/// @tparam T The type for the map's values.
	template<class Key, class T>
	class map {
	public:
		/// @brief The map's key.
		typedef Key key_type;
		/// @brief The map's values.
		typedef T mapped_type;
		/// @brief A pair that holds the key and its coresponding value.
		typedef pair<Key, T> value_type;
		/// @brief A reference to the map's value.
		typedef value_type& reference;
		/// @brief A const reference to the map's value.
		typedef const value_type& const_reference;
		/// @brief A pointer to the map's value.
		typedef value_type* pointer;
		/// @brief A const pointer to the map's value.
		typedef const value_type* const_pointer;
		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Creates an empty map.
		constexpr map() = default;
		/// @brief Copies the given map.
		/// @param other The map to copy.
		map(const map& other) : mapSize(other.mapSize), mapCapacity(other.mapCapacity), mapData(malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY)) {
			// Check if the memory was allocated correctly
			if(!mapData)
				WFE_LOG_FATAL("Failed to allocate map memory!");

			// Copy every value from the given map
			pointer end = mapData + mapSize;
			const_pointer ptr2 = other.mapData;

			for(pointer ptr = mapData; ptr != end; ++ptr)
				new(ptr) value_type(*ptr2++);
		}
		/// @brief Moves the given map's contents.
		/// @param other The map whose contents to move.
		map(map&& other) noexcept : mapSize(other.mapSize), mapCapacity(other.mapCapacity), mapData(other.mapData) {
			// Unlink the data from the given map
			other.mapData = nullptr;
		}
		/// @brief Copies every value from the given list.
		/// @param list The map to copy from.
		map(std::initializer_list<value_type> list) : mapSize(0), mapCapacity(1), mapData(nullptr) {
			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(list.size() < max_size(), "The map's size must be lower than the maximum possible size!")

			// Set the map's capacity to the lowest power of 2 higher than or equal to the map's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((mapCapacity << step) < list.size())
					mapCapacity <<= step;
			mapCapacity <<= 1;

			// Allocate the map's memory
			mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);

			if(!mapData)
				WFE_LOG_FATAL("Failed to allocate map memory!")
			
			// Insert every pair from the list into the map
			pointer end = (pointer)list.end();

			for(pointer ptr = (pointer)list.begin(); ptr != end; ++ptr) {
				// Find the wanted pair
				size_type pos = mapSize;
				for(size_type step = mapCapacity; step; step >>= 1)
					if(pos >= step && !(mapData[pos - step].first < ptr->first))
						pos -= step;
				
				// Check if the current pair already exists
				if(mapData[pos].first == ptr->first) {
					// Set the current pair's new value
					mapData[pos].second = ptr->second;

					continue;
				}

				// Move part of the map forward to make room for the new pair
				memmove(mapData + pos + 1, mapData + pos, (mapSize - pos) * sizeof(value_type));

				// Create the new pair
				new(mapData + pos) value_type(*ptr);

				// Increment the set's size
				++mapSize;
			}
		}
		/// @brief Copies the given std library map.
		/// @param other The std library map to copy.
		map(const std::map<key_type, mapped_type>& other) : mapSize(other.size()), mapCapacity(1), mapData(nullptr) {
			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Set the map's capacity to the lowest power of 2 higher than or equal to the map's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((mapCapacity << step) < mapSize)
					mapCapacity <<= step;
			mapCapacity <<= 1;

			// Allocate the map's memory
			mapData = malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);

			if(!mapData)
				WFE_LOG_FATAL("Failed to allocate map memory!")
			
			// Copy every value from the given map
			pointer ptr = mapData;

			for(const auto& val : other)
				new(ptr++) value_type(val);
		}

		/// @brief Copies the given map's contents into this map.
		/// @param other The map to copy from.
		/// @return A reference to this map.
		map& operator=(const map& other) {
			// Exit the function if the given map is the same as this map
			if(&other == this)
				return *this;
			
			if(mapSize) {
				// Destruct the map's values
				pointer end = mapData + mapSize;

				for(pointer ptr = mapData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the map's memory
				free(mapData, mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
			}

			// Set the map's new values
			mapSize = other.mapSize;
			mapCapacity = other.mapCapacity;
			mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);

			// Check if the memory was allocated correctly
			if(!mapData)
				WFE_LOG_FATAL("Failed to allocate map memory!")
			
			// Copy every value from the given map
			pointer end = mapData + mapSize;
			const_pointer ptr2 = other.mapData;

			for(pointer ptr = mapData; ptr != end; ++ptr)
				new(ptr) value_type(*ptr2++);
			
			return *this;
		}
		/// @brief Moves the given map's contents into this map.
		/// @param other The map to move from.
		/// @return A reference to this map.
		map& operator=(map&& other) {
			if(mapSize) {
				// Destruct the map's values
				pointer end = mapData + mapSize;

				for(pointer ptr = mapData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the map's memory
				free(mapData, mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
			}

			// Set the map's new values
			mapSize = other.mapSize;
			mapCapacity = other.mapCapacity;
			mapData = other.mapData;

			// Unlink the data from the given map
			other.mapData = nullptr;

			return *this;
		}
		/// @brief Copies the items from the given list into this map.
		/// @param list The list to copy from.
		/// @return A reference to this map.
		map& operator=(std::initializer_list<value_type> list) {
			if(mapSize) {
				// Destruct the map's values
				pointer end = mapData + mapSize;

				for(pointer ptr = mapData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the map's memory
				free(mapData, mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
			}

			// Set the map's new values
			mapSize = 0;
			mapCapacity = 1;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(list.size() < max_size(), "The map's size must be lower than the maximum possible size!")

			// Set the map's capacity to the lowest power of 2 higher than or equal to the map's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((mapCapacity << step) < list.size())
					mapCapacity <<= step;
			mapCapacity <<= 1;

			// Allocate the map's memory
			mapData = malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);

			if(!mapData)
				WFE_LOG_FATAL("Failed to allocate map memory!")
			
			// Insert every pair from the list into the map
			pointer end = (pointer)list.end();

			for(pointer ptr = (pointer)list.begin(); ptr != end; ++ptr) {
				// Find the wanted pair
				size_type pos = mapSize;
				for(size_type step = mapCapacity; step; step >>= 1)
					if(pos >= step && !(mapData[pos - step].first < ptr->first))
						pos -= step;
				
				// Check if the current pair already exists
				if(pos != mapSize && mapData[pos].first == ptr->first) {
					// Set the current pair's new value
					mapData[pos].second = ptr->second;

					continue;
				}

				// Move part of the map forward to make room for the new pair
				memmove(mapData + pos + 1, mapData + pos, (mapSize - pos) * sizeof(value_type));

				// Create the new pair
				new(mapData + pos) value_type(*ptr);

				// Increment the set's size
				++mapSize;
			}
		}

		operator std::map<key_type, value_type>() const {
			std::map<key_type, value_type> map;

			const_pointer end = mapData + mapSize;
			for(const_pointer ptr = mapData; ptr != end; ++ptr)
				map.insert(*ptr);
			
			return map;
		}

		/// @brief Gets the value or creates a new one with the given key.
		/// @param key The key to look for.
		/// @return A reference to the wanted value.
		mapped_type& operator[](const key_type& key) {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted value
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted value exists
			if(pos != mapSize && mapData[pos].first == key)
				return mapData[pos].second;
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the map forward to make room for the new pair
			memmove(mapData + pos + 1, mapData + pos, (mapSize - 1 - pos) * sizeof(value_type));

			// Create the new value
			new(mapData + pos) value_type(key, {});

			return mapData[pos].second;
		}
		/// @brief Gets the value or creates a new one with the given key.
		/// @param key The key to look for.
		/// @return A reference to the wanted value.
		mapped_type& operator[](key_type&& key) {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted value
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted value exists
			if(pos != mapSize && mapData[pos].first == key)
				return mapData[pos].second;
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the map forward to make room for the new pair
			memmove(mapData + pos + 1, mapData + pos, (mapSize - 1 - pos) * sizeof(value_type));

			// Create the new value
			new(mapData + pos) value_type(key, {});

			return mapData[pos].second;
		}
		/// @brief Gets the value with the given key. The function will fail if the wanted value doesn't exist.
		/// @param key The key to look for.
		/// @return A reference to the wanted value.
		mapped_type& at(const key_type& key) {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted value
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Assert that the wanted value must exist in the map
			WFE_ASSERT(pos != mapSize && mapData[pos].first == key, "The wanted value must exist in the set!")

			return mapData[pos].second;
		}
		/// @brief Gets the value with the given key. The function will fail if the wanted value doesn't exist.
		/// @param key The key to look for.
		/// @return A reference to the wanted value.
		const mapped_type& at(const key_type& key) const {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted value
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Assert that the wanted value must exist in the map
			WFE_ASSERT(pos != mapSize && mapData[pos].first == key, "The wanted value must exist in the set!")

			return mapData[pos].second;
		}

		/// @brief Inserts the given pair into the map.
		/// @param val The pair to insert.
		/// @return A pair containing a pointer to the inserted pair or a pair with an identical key and a bool that is true if a new pair has been inserted, or false otherwise.
		pair<pointer, bool8_t> insert(const_reference val) {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < val.first))
					pos -= step;
			
			// Check if the wanted pair exists
			if(pos != mapSize && mapData[pos].first == val.first)
				return { mapData + pos, false };
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the map forward to make room for the new pair
			memmove(mapData + pos + 1, mapData + pos, (mapSize - 1 - pos) * sizeof(value_type));

			// Create the new pair
			new(mapData + pos) value_type(val);

			return { mapData + pos, true };
		}
		/// @brief Inserts the given pair into the map.
		/// @param val The pair to insert.
		/// @return A pair containing a pointer to the inserted pair or a pair with an identical key and a bool that is true if a new pair has been inserted, or false otherwise.
		pair<pointer, bool8_t> insert(value_type&& val) {
			// Set the bynary search step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < val.first))
					pos -= step;
			
			// Check if the wanted pair exists
			if(pos != mapSize && mapData[pos].first == val.first)
				return { mapData + pos, false };
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the map forward to make room for the new pair
			memmove(mapData + pos + 1, mapData + pos, (mapSize - 1 - pos) * sizeof(value_type));

			// Create the new pair
			new(mapData + pos) value_type(val);

			return { mapData + pos, true };
		}
		/// @brief Inserts the given pair into the map.
		/// @param pos A hint for the position where the pair can be inserted. Note that the element won't always be inserted at this position, sinces the pairs in a map must be in a specific order.
		/// @param val The pair to insert.
		/// @return A pointer to the inserted pair or a pair with an identical key.
		pointer insert(pointer pos, const_reference val) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos <= end(), "The given pointer must be in range!")

			// Check if the position hint is correct
			if(pos != end() && pos->first == val.first)
				return (pointer)pos;
			else if((pos != begin() && (pos - 1)->first < val.first) && (pos != end() && val.first < pos->first)) {
				// Save the position hint's index relative to the map's data
				size_type posInd = pos - mapData;

				// Increment the map's size
				++mapSize;

				// Assert that the map's size must be lower than the maximum possible size
				WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

				// Check if memory needs to be reallocated
				if(mapSize > mapCapacity) {
					// Save the map's old capacity
					size_type oldCapacity = mapCapacity;

					// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
					mapCapacity = 1;
					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((mapCapacity << step) < mapSize)
							mapCapacity <<= step;
					mapCapacity <<= 1;

					// Reallocate the set's memory
					if(!mapData)
						mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
					else
						mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
					
					// Check if the memory was allocated correctly
					if(!mapData)
						WFE_LOG_FATAL("Failed to allocate map memory!");
				}

				// Move part of the set forward to make room for the new value
				memmove(mapData + posInd + 1, mapData + posInd, (mapSize - 1 - posInd) * sizeof(value_type));

				// Create the new pair
				new(mapData + posInd) value_type(val);

				return mapData + posInd;
			}

			// Compare the position hint to the given pair
			size_type posBegin, size;

			if(pos == end()) {
				// Look through the whole map
				posBegin = 0;
				size = mapSize;
			} else if(val.first < pos->first) {
				// Look through the pairs before the position hint
				posBegin = 0;
				size = pos - mapData - 1;
			} else {
				// Look through the pairs after the position hint
				posBegin = pos - mapData + 1;
				size = mapData + mapSize - pos - 1;
			}

			// Set the binary search's step to the highest power of 2 lower than or equal to the map region's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 1; stepStep; stepStep >>= 1)
				if((step << stepStep) <= size)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type posInd = posBegin + size;
			for(; step; step >>= 1)
				if(posInd >= step + posBegin && !(mapData[posInd - step].first < val.first))
					posInd -= step;
			
			// Check if the wanted value exists
			if(posInd != mapSize && mapData[posInd].first == val.first)
				return mapData + posInd;
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the set forward to make room for the new value
			memmove(mapData + posInd + 1, mapData + posInd, (mapSize - 1 - posInd) * sizeof(value_type));

			// Create the new pair
			new(mapData + posInd) value_type(val);

			return mapData + posInd;
		}
		/// @brief Inserts the given pair into the map.
		/// @param pos A hint for the position where the pair can be inserted. Note that the element won't always be inserted at this position, sinces the pairs in a map must be in a specific order.
		/// @param val The pair to insert.
		/// @return A pointer to the inserted pair or a pair with an identical key.
		pointer insert(pointer pos, value_type&& val) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos <= end(), "The given pointer must be in range!")

			// Check if the position hint is correct
			if(pos != end() && pos->first == val.first)
				return (pointer)pos;
			else if((pos != begin() && (pos - 1)->first < val.first) && (pos != end() && val.first < pos->first)) {
				// Save the position hint's index relative to the map's data
				size_type posInd = pos - mapData;

				// Increment the map's size
				++mapSize;

				// Assert that the map's size must be lower than the maximum possible size
				WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

				// Check if memory needs to be reallocated
				if(mapSize > mapCapacity) {
					// Save the map's old capacity
					size_type oldCapacity = mapCapacity;

					// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
					mapCapacity = 1;
					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((mapCapacity << step) < mapSize)
							mapCapacity <<= step;
					mapCapacity <<= 1;

					// Reallocate the set's memory
					if(!mapData)
						mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
					else
						mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
					
					// Check if the memory was allocated correctly
					if(!mapData)
						WFE_LOG_FATAL("Failed to allocate map memory!");
				}

				// Move part of the set forward to make room for the new value
				memmove(mapData + posInd + 1, mapData + posInd, (mapSize - 1 - posInd) * sizeof(value_type));

				// Create the new pair
				new(mapData + posInd) value_type(val);

				return mapData + posInd;
			}

			// Compare the position hint to the given pair
			size_type posBegin, size;

			if(pos == end()) {
				// Look through the whole map
				posBegin = 0;
				size = mapSize;
			} else if(val.first < pos->first) {
				// Look through the pairs before the position hint
				posBegin = 0;
				size = pos - mapData - 1;
			} else {
				// Look through the pairs after the position hint
				posBegin = pos - mapData + 1;
				size = mapData + mapSize - pos - 1;
			}

			// Set the binary search's step to the highest power of 2 lower than or equal to the map region's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 1; stepStep; stepStep >>= 1)
				if((step << stepStep) <= size)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type posInd = posBegin + size;
			for(; step; step >>= 1)
				if(posInd >= step + posBegin && !(mapData[posInd - step].first < val.first))
					posInd -= step;
			
			// Check if the wanted value exists
			if(posInd != mapSize && mapData[posInd].first == val.first)
				return mapData + posInd;
			
			// Increment the map's size
			++mapSize;

			// Assert that the map's size must be lower than the maximum possible size
			WFE_ASSERT(mapSize < max_size(), "The map's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(mapSize > mapCapacity) {
				// Save the map's old capacity
				size_type oldCapacity = mapCapacity;

				// Set the map's capacity to the lowest power of 2 higher than or equal to the set's size
				mapCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((mapCapacity << step) < mapSize)
						mapCapacity <<= step;
				mapCapacity <<= 1;

				// Reallocate the set's memory
				if(!mapData)
					mapData = (pointer)malloc(mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				else
					mapData = (pointer)realloc(mapData, oldCapacity * sizeof(value_type), mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!mapData)
					WFE_LOG_FATAL("Failed to allocate map memory!");
			}

			// Move part of the set forward to make room for the new value
			memmove(mapData + posInd + 1, mapData + posInd, (mapSize - 1 - posInd) * sizeof(value_type));

			// Create the new pair
			new(mapData + posInd) value_type(val);

			return mapData + posInd;
		}
		/// @brief Inserts every pair from the given list.
		/// @param list The list to insert from.
		void insert(std::initializer_list<value_type> list) {
			// Exit the function if the given list is empty
			if(!list.size())
				return;
			
			// Insert the first item of the list
			pointer posHint = insert(*list.begin()).first;

			// Insert every other item using the given hint
			const_pointer end = list.end();

			for(const_pointer ptr = list.begin() + 1; ptr != end; ++ptr)
				posHint = insert(posHint, *ptr);
		}
		/// @brief Erases the pair at the given position.
		/// @param pos The position to erase from.
		/// @return A pointer to the space occupied by the erased pair.
		pointer erase(const_pointer pos) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Decrement the map's size
			--mapSize;
			
			// Destruct the value at the given position
			pos->~value_type();

			// Move part of the map backwards to erase the wanted pair
			memcpy((pointer)pos, pos + 1, (mapData + mapSize - pos) * sizeof(value_type));

			return (pointer)pos;
		}
		/// @brief Erases the pair with the given key.
		/// @param key The key whose pair to erase.
		/// @return The number of erased pairs.
		size_type erase(const key_type& key) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted pair doesn't exist
			if(pos == mapSize || !(mapData[pos].first == key))
				return 0;
			
			// Decrement the map's size
			--mapSize;

			// Destruct the value at the given position
			mapData[pos].~value_type();

			// Move part of the map backwards to erase the wanted pair
			memcpy(mapData + pos, mapData + pos + 1, (mapSize - pos) * sizeof(value_type));

			return 1;
		}
		/// @brief Erases the pair from the given range.
		/// @param first A pointer to the first erased value.
		/// @param last A pointer to right after the last erased value.
		/// @return A pointer to the space occupies by the first erased pair.
		pointer erase(const_pointer first, const_pointer last) {
			// Assert that the given pointers must be in range
			WFE_ASSERT(first >= begin() && first < end() && last > begin() && last <= end(), "The given pointers must be in range!")

			// Assert that the first pointer must be smaller than the last
			WFE_ASSERT(first < last, "The first pointer must be smaller than the last!")

			// Calculate the number of erased pairs
			size_type erasedSize = last - first;

			// Destruct every value in the given range
			for(pointer ptr = (pointer)first; ptr != last; ++ptr)
				ptr->~value_type();

			// Move part of the map backwards to erase the pairs from the given range
			memcpy((pointer)first, last, (mapData + mapSize - last) * sizeof(size_type));

			// Decrease the map's size by the erased size
			mapSize -= erasedSize;

			return (pointer)first;
		}
		/// @brief Swaps the two map's contents.
		/// @param other The map to swap with.
		void swap(map& other) {
			// Exit the function if the given map is the same as this map
			if(&other == this)
				return;
			
			// Save this map's values
			size_type auxSize = mapSize;
			size_type auxCapacity = mapCapacity;
			pointer auxData = mapData;

			// Set this map's new values
			mapSize = other.mapSize;
			mapCapacity = other.mapCapacity;
			mapData = other.mapData;

			// Set the given map's new values
			other.mapSize = auxSize;
			other.mapCapacity = auxCapacity;
			other.mapData = auxData;
		}
		/// @brief Clears the map's contents.
		void clear() noexcept {
			if(mapSize) {
				// Destruct every value in the map
				pointer end = mapData + mapSize;
				for(pointer ptr = mapData; ptr != end; ++ptr)
					ptr->~value_type();
			}

			// Set the map's size to 0
			mapSize = 0;
		}
		/// @brief Emplaces the pair constructed with the given args into the map.
		/// @tparam ...Args The arg types for constructing the pair.
		/// @param ...args The args for constructing the pair.
		/// @return A pair containing a pointer to the emplaced pair or a pair with an identical key and a bool that is true if a new pair has been emplace, or false otherwise.
		template<class... Args>
		pair<pointer, bool8_t> emplace(Args&&... args) {
			// Construct the pair
			value_type val(args...);

			// Insert the constructed pair
			return insert(val);
		}
		/// @brief Emplaces the pair constructed with the given args into the map.
		/// @tparam ...Args The arg types for constructing the pair.
		/// @param pos A hint for the position where the pair can be emplaced. Note that the element won't always be emplaced at this position, sinces the pairs in a map must be in a specific order.
		/// @param ...args The args for constructing the pair.
		/// @return A pointer to the emplaced pair or a pair with an identical key.
		template<class... Args>
		pointer emplace_hint(const_pointer pos, Args&&... args) {
			// Construct the pair
			value_type val(args...);

			// Insert the constructed pair with the given hint
			return insert(pos, val);
		}

		/// @brief Gets the beginning of the map.
		/// @return A pointer to the beginning of the map.
		pointer begin() {
			return mapData;
		}
		/// @brief Gets the beginning of the map.
		/// @return A const pointer to the beginning of the map.
		const_pointer begin() const {
			return mapData;
		}
		/// @brief Gets the end of the map.
		/// @return A pointer to the end of the map.
		pointer end() {
			return mapData + mapSize;
		}
		/// @brief Gets the end of the map.
		/// @return A const pointer to the end of the map.
		const_pointer end() const {
			return mapData + mapSize;
		}

		/// @brief Tests if the map is empty.
		/// @return True if the map is empty, otherwise false.
		bool8_t empty() const {
			return !mapSize;
		}
		/// @brief Gets the map's size.
		/// @return The map's size.
		size_type size() const {
			return mapSize;
		}
		/// @brief Gets the map's maximum possible size.
		/// @return The map's maximum possible size.
		size_type max_size() const {
			return SIZE_T_MAX / sizeof(value_type);
		}

		/// @brief Finds the pair with the given key.
		/// @param key The key whose pair to look for.
		/// @return A pointer to the wanted pair, or map::end() if it doesn't exist.
		pointer find(const key_type& key) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted pair exists
			if(pos != mapSize && mapData[pos].first == key)
				return mapData + pos;
			
			return end();
		}
		/// @brief Finds the pair with the given key.
		/// @param key The key whose pair to look for.
		/// @return A const pointer to the wanted pair, or map::end() if it doesn't exist.
		const_pointer find(const key_type& key) const {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted pair exists
			if(pos != mapSize && mapData[pos].first == key)
				return mapData + pos;
			
			return end();
		}
		/// @brief Gets the number of pairs with the given key.
		/// @param key THe key whose pairs to look for.
		/// @return The number of wanted pairs.
		size_type count(const key_type& key) const {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			// Check if the wanted pair exists
			if(pos != mapSize && mapData[pos] == key)
				return mapData + pos;
			
			return end();
		}
		/// @brief Finds the first pair whose key isn't lower than the given key.
		/// @param key The key to compare with.
		/// @return A pointer to the wanted pair, or map::end() if it doesn't exist.
		pointer lower_bound(const key_type& key) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			return mapData + pos;
		}
		/// @brief Finds the first pair whose key isn't lower than the given key.
		/// @param key The key to compare with.
		/// @return A const pointer to the wanted pair, or map::end() if it doesn't exist.
		const_pointer lower_bound(const key_type& key) const {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && !(mapData[pos - step].first < key))
					pos -= step;
			
			return mapData + pos;
		}
		/// @brief Finds the first pair whose key is higher than the given key.
		/// @param key The key to compare with.
		/// @return A pointer to the wanted pair, or map::end() if it doesn't exist.
		pointer upper_bound(const key_type& key) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && key < mapData[pos - step].first)
					pos -= step;
			
			return mapData + pos;
		}
		/// @brief Finds the first pair whose key is higher than the given key.
		/// @param key The key to compare with.
		/// @return A const pointer to the wanted pair, or map::end() if it doesn't exist.
		const_pointer upper_bound(const key_type& key) const {
			// Set the binary search's step to the highest power of 2 lower than or equal to the map's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= mapSize)
					step <<= stepStep;
			
			// Find the wanted pair
			size_type pos = mapSize;
			for(; step; step >>= 1)
				if(pos >= step && key < mapData[pos - step].first)
					pos -= step;
			
			return mapData + pos;
		}
		/// @brief Finds the range that includes all pairs whose keys are equal to the given key.
		/// @param key The key to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<pointer, pointer> equal_range(const key_type& key) {
			pair<pointer, pointer> range;

			// Find the range's lower bound
			range.first = lower_bound(key);

			// Set the range's upper bound based on the lower bound
			if(range.first != end() && key == range.first->first)
				range.second = range.first + 1;
			else
				range.second = range.first;
			
			return range;
		}
		/// @brief Finds the range that includes all pairs whose keys are equal to the given key.
		/// @param key The key to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<const_pointer, const_pointer> equal_range(const key_type& key) const {
			pair<pointer, pointer> range;

			// Find the range's lower bound
			range.first = lower_bound(key);

			// Set the range's upper bound based on the lower bound
			if(range.first != end() && key == range.first->first)
				range.second = range.first + 1;
			else
				range.second = range.first;
			
			return range;
		}

		/// @brief Deletes the map.
		~map() {
			if(mapData) {
				// Destruct every value in the map
				pointer end = mapData + mapSize;

				for(pointer ptr = mapData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the map's memory
				free(mapData, mapCapacity * sizeof(value_type), MEMORY_USAGE_ARRAY);
			}
		}
	private:
		size_type mapSize = 0;
		size_type mapCapacity = 0;
		pointer mapData = 0;
	};
}