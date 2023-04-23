#pragma once

#include "Defines.hpp"
#include "Debug.hpp"
#include "Math.hpp"
#include "Pair.hpp"
#include <initializer_list>
#include <unordered_map>

namespace wfe {
	/// @brief Holds multiple buckets of elements with unique keys.
	/// @tparam Key The type for the value's keys.
	/// @tparam T The type of the unordered set's values.
	template<class Key, class T>
	class unordered_map {
		/// @brief The unordered map's key.
		typedef Key key_type;
		/// @brief The unordered map's values.
		typedef T mapped_type;
		/// @brief A pair that holds the key and its corresponding value.
		typedef pair<const key_type, mapped_type> value_type;
		/// @brief A reference to the unordered map's value.
		typedef value_type& reference;
		/// @brief A const reference to the unordered map's value.
		typedef const value_type& const_reference;
		/// @brief A pointer to the unordered map's value.
		typedef value_type* pointer;
		/// @brief A const pointer to the unordered map's value.
		typedef const value_type* const_pointer;
	private:
		/// @brief The unordered map's node type.
		struct node_type {
			value_type val;
			node_type* next;
		};
	public:
		/// @brief An iterator to the unordered map's values.
		struct iterator {
			/// @brief Copies the given iterator.
			/// @param other The iterator to copy.
			iterator(const iterator& other) = default;
			/// @brief Moves the given iterator's contents.
			/// @param other The iterator to move from.
			iterator(iterator&& other) noexcept = default;

			/// @brief Copies the given iterator's contents into this iterator.
			/// @param other The iterator to copy from.
			/// @return A reference to this iterator.
			iterator& operator=(const iterator& other) = default;
			/// @brief Moves the given iterator's contents into this iterator.
			/// @param other The iterator to move from.
			/// @return A reference to this iterator.
			iterator& operator=(iterator&& other) noexcept = default;

			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are equal, otherwise false.
			bool8_t operator==(const iterator& other) const {
				return node == other.node;
			}
			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are different, otherwise false.
			bool8_t operator!=(const iterator& other) const {
				return node != other.node;
			}

			/// @brief Increments this iterator.
			/// @return A reference to this iterator.
			iterator& operator++() {
				// Increment the pointer
				++node;

				return *this;
			}
			/// @brief Increments this iterator.
			/// @return This iterator's previous state.
			iterator operator++(int) {
				// Save the current state of the iterator
				iterator currentIter = *this;

				// Increment the pointer
				++node;

				return currentIter;
			}

			/// @brief Accesses the iterator value's members.
			/// @return The iterator's pointer.
			pointer operator->() const {
				return &(node->val);
			}
			/// @brief Accesses the iterator's value.
			/// @return A reference to the iterator's value.
			reference operator*() const {
				return node->val;
			}

			/// @brief Destruct this iterator.
			~iterator() = default;
		private:
			iterator(node_type* node) : node(node) { }

			node_type* node;

			friend unordered_map;
		};
		/// @brief A const iterator to the unordered map's values.
		struct const_iterator {
			/// @brief Copies the given iterator.
			/// @param other The iterator to copy.
			const_iterator(const const_iterator& other) = default;
			/// @brief Moves the given iterator's contents.
			/// @param other The iterator to move from.
			const_iterator(const_iterator&& other) noexcept = default;
			/// @brief Copies the given iterator.
			/// @param other The iterator to copy.
			const_iterator(const iterator& other) : node(other.node) { }

			/// @brief Copies the given iterator's contents into this iterator.
			/// @param other The iterator to copy from.
			/// @return A reference to this iterator.
			const_iterator& operator=(const const_iterator& other) = default;
			/// @brief Moves the given iterator's contents into this iterator.
			/// @param other The iterator to move from.
			/// @return A reference to this iterator.
			const_iterator& operator=(const_iterator&& other) noexcept = default;

			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are equal, otherwise false.
			bool8_t operator==(const const_iterator& other) const {
				return node == other.node;
			}
			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are different, otherwise false.
			bool8_t operator!=(const const_iterator& other) const {
				return node != other.node;
			}

			/// @brief Increments this iterator.
			/// @return A reference to this iterator.
			const_iterator& operator++() {
				// Increment the pointer
				++node;

				return *this;
			}
			/// @brief Increments this iterator.
			/// @return This iterator's previous state.
			const_iterator operator++(int) {
				// Save the current state of the iterator
				const_iterator currentIter = *this;

				// Increment the pointer
				++node;

				return currentIter;
			}

			/// @brief Accesses the iterator value's members.
			/// @return The iterator's pointer.
			const_pointer operator->() const {
				return &(node->val);
			}
			/// @brief Accesses the iterator's value.
			/// @return A reference to the iterator's value.
			const_reference operator*() const {
				return node->val;
			}

			/// @brief Destruct this iterator.
			~const_iterator() = default;
		private:
			const_iterator(const node_type* node) : node(node) { }

			const node_type* node;

			friend unordered_map;
		};
		/// @brief A const iterator used to access the elements of a bucket.
		struct const_local_iterator {
			/// @brief Copies the given iterator.
			/// @param other The iterator to copy.
			const_local_iterator(const const_local_iterator& other) = default;
			/// @brief Moves the given iterator's contents.
			/// @param other The iterator to move from.
			const_local_iterator(const_local_iterator&& other) noexcept = default;

			/// @brief Copies the given iterator's contents into this iterator.
			/// @param other The iterator to copy from.
			/// @return A reference to this iterator.
			const_local_iterator& operator=(const const_local_iterator& other) = default;
			/// @brief Moves the given iterator's contents into this iterator.
			/// @param other The iterator to move from.
			/// @return A reference to this iterator.
			const_local_iterator& operator=(const_local_iterator&& other) noexcept = default;

			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are equal, otherwise false.
			bool8_t operator==(const const_local_iterator& other) const {
				return node == other.node;
			}
			/// @brief Compares the two iterators.
			/// @param other THe iterator to compare with.
			/// @return True if the iterators are different, otherwise false.
			bool8_t operator!=(const const_local_iterator& other) const {
				return node != other.node;
			}

			/// @brief Increments this iterator.
			/// @return A reference to this iterator.
			const_local_iterator& operator++() {
				// Move onto the next node
				node = node->next;

				return *this;
			}
			/// @brief Increments this iterator.
			/// @return This iterator's previous state.
			const_local_iterator operator++(int) {
				// Save the current state of the iterator
				const_local_iterator currentIter = *this;

				// Move onto the next node
				node = node->next;

				return currentIter;
			}

			/// @brief Accesses the iterator value's members.
			/// @return The iterator's pointer.
			const_pointer operator->() const {
				return &(node->val);
			}
			/// @brief Accesses the iterator's value.
			/// @return A reference to the iterator's value.
			const_reference operator*() const {
				return node->val;
			}

			/// @brief Destruct this iterator.
			~const_local_iterator() = default;
		private:
		 	const_local_iterator(const node_type* node) : node(node) { }

			const node_type* node;

			friend unordered_map;
		};

		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Creates an empty unordered map.
		/// @param n The total number of buckets.
		unordered_map(size_type n = 16) : umapBucketCount(n), umapBuckets((node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY)), umapSize(0), umapCapacity(0), umapData(nullptr) {
			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")

			// Set all the bucket pointers to nullptr
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));
		}
		/// @brief Copies the given unordered map.
		/// @param other The unordered map to copy.
		unordered_map(const unordered_map& other) : umapBucketCount(other.umapBucketCount), umapBuckets((node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY)), umapSize(other.umapSize), umapCapacity(other.umapCapacity), umapData((node_type*)malloc(umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY)), umapMaxLoadFactor(other.umapMaxLoadFactor) {
			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")
			
			// Check if the unordered map's data was allocated correctly
			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")
			
			// Copy every bucket relative to this unordered map
			for(size_type i = 0; i != umapBucketCount; ++i) {
				// Set the current bucket's pointer to nullptr if the other bucket's pointer is also nullptr
				if(!other.umapBuckets[i]) {
					umapBuckets[i] = nullptr;
					continue;
				}

				// Get the other bucket's top index
				size_type bucketIndex = other.umapBuckets[i] - other.umapData;

				// Set the current bucket's pointer relative to this map's data
				umapBuckets[i] = umapData + bucketIndex;
			}

			// Copy every value from the other unordered map
			for(size_type i = 0; i != umapSize; ++i) {
				// Copy the other node's value
				new (&(umapData[i].val)) value_type(other.umapData[i].val);

				// Set the next pointer to nullptr if the other next pointer is also nullptr
				if(!other.umapData[i].next) {
					umapData[i].next = nullptr;
					continue;
				}

				// Get the other next pointer's index
				size_type nextIndex = other.umapData[i].next - other.umapData;

				// Set the current node's next pointer relative to this map's data
				umapData[i].next = umapData + nextIndex;
			}
		}
		/// @brief Moves the given unordered map's contents.
		/// @param other THe unordered map whose contents to move.
		unordered_map(unordered_map&& other) noexcept : umapBucketCount(other.umapBucketCount), umapBuckets(other.umapBuckets), umapSize(other.umapSize), umapCapacity(other.umapCapacity), umapData(other.umapData), umapMaxLoadFactor(other.umapMaxLoadFactor) {
			// Unlink all data from the other unordered set
			other.umapBuckets = nullptr;
			other.umapData = nullptr;
		}
		/// @brief Constructs an unordered set with all the values in the list.
		/// @param list The list to copy from.
		/// @param n The number of buckets.
		unordered_map(std::initializer_list<value_type> list, size_type n = 16) : umapBucketCount(n), umapBuckets((node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY)), umapSize(0), umapCapacity(1), umapData((node_type*)malloc(sizeof(node_type), MEMORY_USAGE_ARRAY)) {
			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")
			
			// Check if the unordered map's data was allocated correctly
			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")
			
			// Set all bucket pointers to nullptr
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));

			// Loop through every value in the given list
			const_pointer end = list.end();
			for(const_pointer ptr = list.begin(); ptr != end; ++ptr) {
				// Get the current value's bucket
				size_type currentBucket = bucket(ptr->first);

				node_type** elem = umapBuckets + currentBucket;

				// Look for the given value in the bucket
				while(*elem) {
					// Check if the current value is already in the unordered map
					if((*elem)->val.first == ptr->first)
						continue;
					
					// Move on to the next value
					elem = &((*elem)->next);
				}

				// Increment the map's size
				++umapSize;

				// Check if memory needs to be reallocated
				if(umapSize > umapCapacity) 
					reallocate(umapCapacity << 1);

				// Copy the current value into the unordered map
				new(&(umapData[umapSize - 1].val)) value_type(*ptr);

				// Add the new value into the current bucket
				umapData[umapSize - 1].next = nullptr;
				*elem = umapData + umapSize - 1;
			}
		}
		/// @brief Copies the given std library unordered map.
		/// @param other The std library unordered map to copy from.
		unordered_map(const std::unordered_map<key_type, mapped_type>& other) : umapBucketCount(other.bucket_count()), umapBuckets((node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY)), umapSize(other.size()), umapCapacity(1), umapData(nullptr) {
			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")

			// Set all of the unordered map's buckets to nullptr
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));
			
			// Set the unordered map's capacity to the lowest power of 2 higher than or equal to its size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((umapCapacity << step) < umapSize)
					umapCapacity <<= step;
			umapCapacity <<= 1;

			// Allocate the unordered map's data
			umapData = (node_type*)malloc(umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);

			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")
			
			// Add every value from the other unordered map to this unordered map
			auto otherPtr = other.begin();
			node_type* begin = umapData - 1;
			for(node_type* ptr = umapData + umapSize - 1; ptr != begin; --ptr) {
				// Copy the value
				new(&(ptr->val)) value_type(*otherPtr);

				// Get the current bucket
				size_type currentBucket = bucket(otherPtr->first);

				// Add the new node to the current bucket's linked list
				ptr->next = umapBuckets[currentBucket];
				umapBuckets[currentBucket] = ptr;
			}
		}

		/// @brief Copies the given unordered map's contents into this map.
		/// @param other The unordered map to copy from.
		/// @return A reference to this unordered map.
		unordered_map& operator=(const unordered_map& other) {
			// Check if the two maps are the same
			if(&other == this)
				return *this;
			
			// Delete the unordered map's previous data
			if(umapBuckets)
				free(umapBuckets, umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			if(umapData) {
				// Destruct every value in the unordered map
				node_type* end = umapData + umapSize;
				for(node_type* ptr = umapData; ptr != end; ++ptr)
					ptr->val.~value_type();
				
				// Free the unordered map's data
				free(umapData, umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered map's new values
			umapBucketCount = other.umapBucketCount;
			umapBuckets = (node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			umapSize = other.umapSize;
			umapCapacity = other.umapCapacity;
			umapData = (node_type*)malloc(umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			umapMaxLoadFactor = other.umapMaxLoadFactor;

			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")
			
			// Check if the unordered map's data was allocated correctly
			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")
			
			// Copy every bucket relative to this unordered map
			for(size_type i = 0; i != umapBucketCount; ++i) {
				// Set the current bucket's pointer to nullptr if the other bucket's pointer is also nullptr
				if(!other.umapBuckets[i]) {
					umapBuckets[i] = nullptr;
					continue;
				}

				// Get the other bucket's top index
				size_type bucketIndex = other.umapBuckets[i] - other.umapData;

				// Set the current bucket's pointer relative to this map's data
				umapBuckets[i] = umapData + bucketIndex;
			}

			// Copy every value from the other unordered map
			for(size_type i = 0; i != umapSize; ++i) {
				// Copy the other node's value
				new (&(umapData[i].val)) value_type(other.umapData[i].val);

				// Set the next pointer to nullptr if the other next pointer is also nullptr
				if(!other.umapData[i].next) {
					umapData[i].next = nullptr;
					continue;
				}

				// Get the other next pointer's index
				size_type nextIndex = other.umapData[i].next - other.umapData;

				// Set the current node's next pointer relative to this map's data
				umapData[i].next = umapData + nextIndex;
			}

			return *this;
		}
		/// @brief Moves the given unordered map's contents into this map.
		/// @param other The unordered map to move from.
		/// @return A reference to this unordered map.
		unordered_map& operator=(unordered_map&& other) {
			// Delete the unordered map's previous data
			if(umapBuckets)
				free(umapBuckets, umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			if(umapData) {
				// Destruct every value in the unordered map
				node_type* end = umapData + umapSize;
				for(node_type* ptr = umapData; ptr != end; ++ptr)
					ptr->val.~value_type();
				
				// Free the unordered map's data
				free(umapData, umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered map's new values
			umapBucketCount = other.umapBucketCount;
			umapBuckets = other.umapBuckes;
			umapSize = other.umapSize;
			umapCapacity = other.umapCapacity;
			umapData = other.umapData;
			umapMaxLoadFactor = other.umapMaxLoadFactor;

			// Unlink all data from the other unordered set
			other.umapBuckets = nullptr;
			other.umapData = nullptr;

			return *this;
		}
		/// @brief Copies the given list's values into this unordered map.
		/// @param list The list to copy from.
		/// @return A reference to this unordered map.
		unordered_map& operator=(std::initializer_list<value_type> list) {
			// Delete the unordered map's previous data
			if(umapBuckets)
				free(umapBuckets, umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			if(umapData) {
				// Destruct every value in the unordered map
				node_type* end = umapData + umapSize;
				for(node_type* ptr = umapData; ptr != end; ++ptr)
					ptr->val.~value_type();
				
				// Free the unordered map's data
				free(umapData, umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered map's bucket count to the lowest power of 2 higher than or equal to the list's size
			umapBucketCount = 1;
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if(umapBucketCount << step < list.size())
					umapBucketCount <<= step;
			
			// Set the unordered map's new values
			umapBuckets = (node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			umapSize = 0;
			umapCapacity = 1;
			umapData = (node_type*)malloc(sizeof(node_type), MEMORY_USAGE_ARRAY);
			umapMaxLoadFactor = 1.f;

			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")
			
			// Check if the unordered map's data was allocated correctly
			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")
			
			// Set all of the unordered map's buckets to nullptr
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));

			// Loop through every value in the given list
			const_pointer end = list.end();
			for(const_pointer ptr = list.begin(); ptr != end; ++ptr) {
				// Get the current value's bucket
				size_type currentBucket = bucket(ptr->first);

				node_type** elem = umapBucketCount + currentBucket;

				// Look for the given value in the bucket
				while(*elem) {
					// Check if the current value is already in the unordered map
					if((*elem)->val.first == ptr->first)
						break;
					
					// Move on to the next value
					elem = &((*elem)->next);
				}
				if(*elem)
					continue;

				// Increment the map's size
				++umapSize;

				// Check if memory needs to be reallocated
				if(umapSize > umapCapacity) 
					reallocate(umapCapacity << 1);

				// Copy the current value into the unordered set
				new(&(umapData[umapSize - 1].val)) value_type(*ptr);

				// Add the new value into the current bucket
				umapData[umapSize - 1].next = nullptr;
				*elem = umapData + umapSize - 1;
			}

			return *this;
		}

		/// @brief Accesses or inserts a value with the given key.
		/// @param key The key to look for.
		/// @return A reference to the value with the given key.
		mapped_type& operator[](const key_type& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			node_type** elem = umapBucketCount + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return (*elem)->val.second;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// Increment the map's size
			++umapSize;

			// Check if a rehash is required
			if(umapBucketCount * umapMaxLoadFactor < umapSize) {
				// Set the unordered map's new capacity to the lowest power of 2 that, multiplied by the max load factor, is higher than or equal to the unordered set's size
				size_type newBucketCount = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newBucketCount << step) * umapMaxLoadFactor < umapSize)
						newBucketCount <<= step;
				
				// Rehash the unordered map
				rehash(newBucketCount);
			}

			// Check if memory needs to be reallocated
			if(umapSize > umapCapacity) 
				reallocate(umapCapacity << 1);

			// Initialize the value
			new(&(umapData[umapSize - 1].val.first)) key_type(key);
			new(&(umapData[umapSize - 1].val.second)) mapped_type();

			// Add the new value into the current bucket
			umapData[umapSize - 1].next = nullptr;
			*elem = umapData + umapSize - 1;

			return umapData[umapSize - 1].val.second;
		}
		/// @brief Accesses or inserts a value with the given key.
		/// @param key The key to look for.
		/// @return A reference to the value with the given key.
		mapped_type& operator[](key_type&& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return (*elem)->val.second;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// Increment the map's size
			++umapSize;

			// Check if a rehash is required
			if(umapBucketCount * umapMaxLoadFactor < umapSize) {
				// Set the unordered map's new capacity to the lowest power of 2 that, multiplied by the max load factor, is higher than or equal to the unordered set's size
				size_type newBucketCount = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newBucketCount << step) * umapMaxLoadFactor < umapSize)
						newBucketCount <<= step;
				
				// Rehash the unordered map
				rehash(newBucketCount);
			}

			// Check if memory needs to be reallocated
			if(umapSize > umapCapacity) 
				reallocate(umapCapacity << 1);

			// Initialize the value
			new((void*)&(umapData[umapSize - 1].val.first)) key_type(key);
			new(&(umapData[umapSize - 1].val.second)) mapped_type();

			// Add the new value into the current bucket
			umapData[umapSize - 1].next = nullptr;
			*elem = umapData + umapSize - 1;

			return umapData[umapSize - 1].val.second;
		}
		/// @brief Accesses the value with the given key.
		/// @param key The key to look for.
		/// @return A reference to the value with the given key.
		mapped_type& at(const key_type& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return (*elem)->val.second;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; throw an error
			WFE_LOG_FATAL("Failed to find the wanted key in an unordered map!")

			return umapData[0].val.second;
		}
		/// @brief Accesses the value with the given key.
		/// @param key The key to look for.
		/// @return A const reference to the value with the given key.
		const mapped_type& at(const key_type& key) const {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			const node_type** elem = umapBucketCount + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return (*elem)->val.second;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; throw an error
			WFE_LOG_FATAL("Failed to find the wanted key in an unordered map!")
		}

		/// @brief Finds the value with the given key.
		/// @param key The key to look for.
		/// @return An iterator to the wanted value if it exists, otherwise unordered_map::end().
		iterator find(const key_type& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return *elem;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; return an end operator
			return end();
		}
		/// @brief Finds the value with the given key.
		/// @param key The key to look for.
		/// @return A const iterator to the wanted value if it exists, otherwise unordered_map::end().
		const_iterator find(const key_type& key) const {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			const node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return *elem;
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; return an end operator
			return end();
		}
		/// @brief Gets the number of values with the given key.
		/// @param key THe key to look for.
		/// @return The number of values with the given key.
		size_type count(const key_type& key) const {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			const node_type** elem = (const node_type**)(umapBuckets + currentBucket);

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return 1;
				
				// Move on to the next value
				elem = (const node_type**)&((*elem)->next);
			}

			// The value wasn't found; return 0
			return 0;
		}
		/// @brief Finds the range of values with the given key.
		/// @param key The key to look for.
		/// @return The bounds of the wanted range.
		pair<iterator, iterator> equal_range(const key_type& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return { *elem, *elem + 1 };
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; return a pair of end operators
			return { end(), end() };
		}
		/// @brief Finds the range of values with the given key.
		/// @param key The key to look for.
		/// @return The bounds of the wanted range.
		pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			const node_type** elem = umapBucketCount + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current key is already in the unordered map
				if((*elem)->val.first == key)
					return { *elem, *elem + 1 };
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// The value wasn't found; return a pair of end operators
			return { end(), end() };
		}

		/// @brief Emplaces the constructed value into the unordered map.
		/// @tparam ...Args The arg types to construct the value with.
		/// @param ...args The args to construct the value with.
		/// @return A pair containing an iterator to the emplaced value or a value with the same key and a bool that is true if a new value was inserted, otherwise false.
		template<class... Args>
		pair<iterator, bool8_t> emplace(Args&&... args) {
			// Construct the value
			value_type val(args...);

			// Insert the value into the unordered map
			return insert(val);
		}
		/// @brief Emplaces the constructed value into the unordered map using the given hint.
		/// @tparam ...Args The arg types to construct the value with.
		/// @param pos A hint for the position of the emplaced value.
		/// @param ...args The args to construct the value with.
		/// @return An iterator to the emplaced value or a value with the same key.
		template<class... Args> 
		iterator emplace_hint(const_iterator pos, Args&&... args) {
			// Construct the value
			value_type val(args...);

			// Insert the value into the unordered map using the given hint
			return insert(pos, val);
		}
		/// @brief Inserts the given value into the unordered map.
		/// @param val The value to insert.
		/// @return A pair containing an iterator to the inserted value or a value with the same key and a bool that is true if a new value was inserted, otherwise false.
		pair<iterator, bool8_t> insert(const_reference val) {
			// Get the current value's bucket
			size_type currentBucket = bucket(val.first);

			node_type** elem = umapBuckets + currentBucket;

			// Look for the given key in the bucket
			while(*elem) {
				// Check if the current value is already in the unordered map
				if((*elem)->val.first == val.first)
					return { *elem, false };
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// Increment the map's size
			++umapSize;

			// Check if a rehash is required
			if(umapBucketCount * umapMaxLoadFactor < umapSize) {
				// Set the unordered map's new capacity to the lowest power of 2 that, multiplied by the max load factor, is higher than or equal to the unordered set's size
				size_type newBucketCount = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newBucketCount << step) * umapMaxLoadFactor < umapSize)
						newBucketCount <<= step;
				
				// Rehash the unordered map
				rehash(newBucketCount);
			}

			// Check if memory needs to be reallocated
			if(umapSize > umapCapacity) 
				reallocate(umapCapacity << 1);

			// Initialize the value
			new(&(umapData[umapSize - 1].val)) value_type(val);

			// Add the new value into the current bucket
			umapData[umapSize - 1].next = nullptr;
			*elem = umapData + umapSize - 1;

			return { *elem, true };
		}
		/// @brief Inserts the given value into the unordered map using the given hint.
		/// @param pos A hint for the position of the inserted value.
		/// @param val The value to insert.
		/// @return An iterator the the inserted value or a value with the same key.
		iterator insert(const_iterator pos, const_reference val) {
			// Simply insert the value without using the given hint
			return insert(val).first;
		}
		/// @brief Inserts all values from the given list into the unordered map.
		/// @param list The list to insert from.
		void insert(std::initializer_list<value_type> list) {
			// Insert every value fron the given list
			const_pointer end = list.end();
			for(const_pointer ptr = list.begin(); ptr != end; ++ptr)
				insert(*ptr);
		}
		/// @brief Erases the value at the given position from the unordered map.
		/// @param pos The position to erase from.
		/// @return An iterator to the position occupied by the erased value.
		iterator erase(const_iterator pos) {
			// Get the current value's bucket
			size_type currentBucket = bucket(pos->first);

			// Look for the given value in the current bucket's linked list
			node_type** elem = umapBuckets + currentBucket;

			while(*elem && *elem != pos.node) {
				// Move on to the next element
				elem = &((*elem)->next);
			}

			// Remove the value from the current bucket's linked list
			(*elem)->val.~value_type();
			*elem = (*elem)->next;

			// Decrement the unordered map's size
			--umapSize;

			// Exit the function if the erased value was the last value
			if(pos == end())
				return (node_type*)pos.node;

			// Get the last value's bucket
			currentBucket = bucket(umapData[umapSize].val.first);

			// Look for the last value in the current bucket's linked list
			elem = umapBuckets + currentBucket;

			while(*elem && *elem != umapData + umapSize) {
				// Move on to the next element
				elem = &((*elem)->next);
			}

			// Move the value to the empty space
			memcpy((node_type*)pos.node, umapData + umapSize, sizeof(node_type));

			// Move the linked list pointer to the last value to the now filled space
			*elem = (node_type*)pos.node;

			return (node_type*)pos.node;
		}
		/// @brief Erases the value with the given key from the unordered map.
		/// @param key The key to look for.
		/// @return The number of erased values.
		size_type erase(const key_type& key) {
			// Get the current key's bucket
			size_type currentBucket = bucket(key);

			// Look for the given value in the current bucket's linked list
			node_type** elem = umapBuckets + currentBucket;

			while(*elem) {
				// Check if the given key is in the unordered map
				if((*elem)->val.first == key)
					break;

				// Move on to the next element
				elem = &((*elem)->next);
			}

			// Exit the function if no value with the given key was found
			if(!*elem)
				return 0;

			// Remove the value from the current bucket's linked list
			(*elem)->val.~value_type();

			// Save the value's position
			node_type* pos = *elem;

			*elem = (*elem)->next;

			// Decrement the unordered map's size
			--umapSize;

			// Exit the function if the erased value was the last value
			if(pos == umapData + umapSize)
				return 1;

			// Get the last value's bucket
			currentBucket = bucket(umapData[umapSize].val.first);

			// Look for the last value in the current bucket's linked list
			elem = umapBuckets + currentBucket;

			while(*elem && *elem != umapData + umapSize) {
				// Move on to the next element
				elem = &((*elem)->next);
			}

			// Move the value to the empty space
			memcpy(pos, umapData + umapSize, sizeof(node_type));

			// Move the linked list pointer to the last value to the now filled space
			*elem = pos;

			return 1;
		}
		/// @brief Erases all values from the given range.
		/// @param first An iterator to the first erased element.
		/// @param last An iterator to right after the last erased element.
		/// @return An iterator to the position occupied by the first erase value.
		iterator erase(const_iterator first, const_iterator last) {
			// Get the number of erased values
			size_type erasedCount = last->node - first->node;

			// Erase all the values
			for(size_type i = 0; i != erasedCount; ++i)
				erase(first);
			
			return (node_type*)first->node;
		}
		/// @brief Clears the contents of the unordered map.
		void clear() {
			// Destruct every value in the umordered set
			node_type* end = umapData + umapSize;
			for(node_type* ptr = umapData; ptr != end; ++ptr)
				ptr->val.~value_type();
			
			// Set the size to 0
			umapSize = 0;

			// Set all bucket pointers to nullptr
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));
		}
		/// @brief Swaps the contents of the two unordered maps.
		/// @param other The unordered map to swap with.
		void swap(unordered_map& other) {
			// Exit the function if the two unordered maps are the same
			if(&other == this)
				return;
			
			// Save this unordered map's values
			size_type auxBucketCount = umapBucketCount;
			node_type** auxBuckets = umapBuckets;
			size_type auxSize = umapSize;
			size_type auxCapacity = umapCapacity;
			node_type* auxData = umapData;
			float32_t auxMaxLoadFactor = umapMaxLoadFactor;

			// Set this umordered map's values
			umapBucketCount = other.umapBucketCount;
			umapBuckets = other.umapBuckets;
			umapSize = other.umapSize;
			umapCapacity = other.umapCapacity;
			umapData = other.umapData;
			umapMaxLoadFactor = other.umapMaxLoadFactor;

			// Set the other unordered map's values
			other.umapBucketCount = auxBucketCount;
			other.umapBuckets = auxBuckets;
			other.umapSize = auxSize;
			other.umapCapacity = auxCapacity;
			other.umapData = auxData;
			other.umapMaxLoadFactor = auxMaxLoadFactor;
		}

		/// @brief Returns an iterator to the beginning of the unordered map.
		/// @return An iterator to the beginning of the unordered map.
		iterator begin() {
			return umapData;
		}
		/// @brief Returns a const iterator to the beginning of the unordered map.
		/// @return A const iterator to the beginning of the unordered map.
		const_iterator begin() const {
			return umapData;
		}
		/// @brief Returns an iterator to the end of the unordered map.
		/// @return An iterator to the end of the unordered map.
		iterator end() {
			return umapData + umapSize;
		}
		/// @brief Returns a const iterator to the end of the unordered map.
		/// @return A const iterator to the end of the unordered map.
		const_iterator end() const {
			return umapData + umapSize;
		}
		/// @brief Returns a const iterator to the beginning of the unordered map.
		/// @return A const iterator to the beginning of the unordered map.
		const_iterator cbegin() const {
			return umapData;
		}
		/// @brief Returns a const iterator to the end of the unordered map.
		/// @return A const iterator to the end of the unordered map.
		const_iterator cend() const {
			return umapData + umapSize;
		}

		/// @brief Tests if the unordered map is empty.
		/// @return True if the unordered map is empty, otherwise false.
		bool8_t empty() const {
			return !umapSize;
		}
		/// @brief Gets the unordered map's size.
		/// @return The unordered map's size.
		size_type size() const {
			return umapSize;
		}
		/// @brief Returns the unordered map's maximum possible size.
		/// @return The unordered map's maximum possible size.
		size_type max_size() const {
			return SIZE_T_MAX / sizeof(node_type);
		}

		/// @brief Gets the number of buckets in the unordered map.
		/// @return The number of buckets in the unordered map.
		size_type bucket_count() const {
			return umapBucketCount;
		}
		/// @brief Gets the maximum possible number of buckets in the unordered map.
		/// @return The maximum possible number of buckets.
		size_type max_bucket_count() const {
			return SIZE_T_MAX / sizeof(node_type**);
		}
		/// @brief Gets the number of values in the given bucket.
		/// @param n THe wanted bucket's index.
		/// @return The number of values in the givne bucket.
		size_type bucket_size(size_type n) const {
			// Loop through the bucket and count the number of nodes
			const node_type* elem = umapBuckets[n];
			size_type count = 0;

			while(elem) {
				// Increment the count
				++count;

				// Move on to the next node
				elem = elem->next;
			}

			// Return the final count
			return count;
		}
		/// @brief Gets the given key's bucket index.
		/// @param key The key whose bucket index to get.
		/// @return The given key's bucket index.
		size_type bucket(const key_type& key) const {
			// Hash the given key
			uint64_t hash = Hash(key).val;

			return hash % umapBucketCount;
		}

		/// @brief Gets the unordered map's load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @return The unordered map's load factor.
		float32_t load_factor() const {
			return (float32_t)umapSize / (float32_t)umapBucketCount;
		}
		/// @brief Gets the unordered map's max load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @return The unordered map's load factor.
		float32_t max_load_factor() const {
			return umapMaxLoadFactor;
		}
		/// @brief Sets the unordered map's max load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @param z The unordered map's new load factor.
		void max_load_factor(float32_t z) {
			umapMaxLoadFactor = z;
		}
		/// @brief Sets the unordered map's bucket count.
		/// @param n The unordered map's new bucket count.
		void rehash(size_type n) {
			// Save the old bucket count
			size_type oldBucketCount = umapBucketCount;

			// Set the new bucket count
			umapBucketCount = n;

			// Reallocate the unordered map's buckets
			if(umapBuckets)
				umapBuckets = (node_type**)realloc(umapBuckets, oldBucketCount * sizeof(node_type*), umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			else
				umapBuckets = (node_type**)malloc(umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);

			// Check if the unordered map's buckets were allocated correctly
			if(!umapBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered map buckets!")

			// Set all bucket pointers to 0
			memset(umapBuckets, 0, umapBucketCount * sizeof(node_type*));

			// Loop through the unordered map's data backwards
			node_type* end = umapData - 1;
			for(node_type* ptr = umapData + umapSize - 1; ptr != end; --ptr) {
				// Get the current value's bucket
				size_type currentBucket = bucket(ptr->val.first);

				// Set the current node's next pointer
				ptr->next = umapBuckets[currentBucket];
				
				// Set the current linked list's new head
				umapBuckets[currentBucket] = ptr;
			}
		}
		/// @brief Sets the unordered map's bucket count to the most appropriate to contain the given number of values.
		/// @param n The requested number of values.
		void reserve(size_type n) {
			// Set the unordered map's new capacity to the lowest power of 2 that, multiplied by the max load factor, is higher than or equal to the unordered set's size
			size_type newBucketCount = 1;
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((newBucketCount << step) * umapMaxLoadFactor < n)
					newBucketCount <<= step;
			
			// Rehash the unordered map
			rehash(newBucketCount);
		}

		/// @brief Deletes the unordered map.
		~unordered_map() {
			// Delete the unordered map's buckets
			if(umapBuckets)
				free(umapBuckets, umapBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);

			// Delete the unordered map's data
			if(umapData) {
				// Destruct every value in the unordered map
				node_type* end = umapData + umapSize;
				for(node_type* ptr = umapData; ptr != end; ++ptr)
					ptr->val.~value_type();
				
				// Free the unordered map's data
				free(umapData, umapCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			}
		}
	private:
		void reallocate(size_type newCapacity) {
			// Save the unordered map's old data
			node_type* oldData = umapData;

			// Reallocate the unordered map's data
			umapData = (node_type*)realloc(umapData, umapCapacity * sizeof(node_type), newCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);

			// Check if the unordered map's data was allocated correctly
			if(!umapData)
				WFE_LOG_FATAL("Failed to allocate unordered map data!")

			// Set the unordered map's new capacity
			umapCapacity = newCapacity;

			// Reset every bucket relative to the new data
			for(size_type i = 0; i != umapBucketCount; ++i) {
				// Skip this bucket if it is already set to nullptr
				if(!umapBuckets[i])
					continue;

				// Get the bucket's top index
				size_type bucketIndex = umapBuckets[i] - oldData;

				// Set the current bucket's pointer relative to this map's data
				umapBuckets[i] = umapData + bucketIndex;
			}

			// Reset every next pointer relative to the new data
			for(size_type i = 0; i != umapSize; ++i) {
				// Skip the current node if the next pointer is already nullptr
				if(!umapData[i].next)
					continue;

				// Get the other next pointer's index
				size_type nextIndex = umapData[i].next - oldData;

				// Set the current node's next pointer relative to this map's data
				umapData[i].next = umapData + nextIndex;
			}
		}

		size_type umapBucketCount;
		node_type** umapBuckets;
		size_type umapSize;
		size_type umapCapacity;
		node_type* umapData;
		float32_t umapMaxLoadFactor = 1.f;
	};

	/// @brief Compares the given unordered maps.
	/// @tparam Key The unordered map key's type.
	/// @tparam T The unordered map value's type.
	/// @param umap1 The first unordered map to compare.
	/// @param umap2 The second unordered map to compare.
	/// @return True if the two unordered maps are equal, otherwise value.
	template<class Key, class T>
	WFE_INLINE bool8_t operator==(const unordered_map<Key, T>& umap1, const unordered_map<Key, T>& umap2) {
		// Exit the function if the two unordered maps have different sizes
		if(umap1.size() != umap2.size())
			return false;
		
		// Search for every value from the first map in the second map
		for(const auto& val : umap1) {
			// Search for the value in the second unordered map
			auto iter = umap2.find(val.first);

			// Exit the function if the value wasn't found
			if(iter == umap2.end())
				return false;
			
			// Exit the function if the values are different
			if(iter->second != val.second)
				return false;
		}

		return true;
	}
	/// @brief Compares the given unordered maps.
	/// @tparam Key The unordered map key's type.
	/// @tparam T The unordered map value's type.
	/// @param umap1 The first unordered map to compare.
	/// @param umap2 The second unordered map to compare.
	/// @return True if the two unordered maps are different, otherwise value.
	template<class Key, class T>
	WFE_INLINE bool8_t operator!=(const unordered_map<Key, T>& umap1, const unordered_map<Key, T>& umap2) {
		// Exit the function if the two unordered maps have different sizes
		if(umap1.size() != umap2.size())
			return true;
		
		// Search for every value from the first map in the second map
		for(const auto& val : umap1) {
			// Search for the value in the second unordered map
			auto iter = umap2.find(val.first);

			// Exit the function if the value wasn't found
			if(iter == umap2.end())
				return true;
			
			// Exit the function if the values are different
			if(iter->second != val.second)
				return true;
		}

		return false;
	}
}