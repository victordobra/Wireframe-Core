#pragma once

#include "Allocator.hpp"
#include "Compare.hpp"
#include "Defines.hpp"
#include "Memory.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "Pair.hpp"
#include <initializer_list>
#include <set>

namespace wfe {
	/// @brief Holds a vector of unique elements following a specific order.
	/// @tparam T The type of the set's values.
	/// @tparam Lower The function struct used to check if one value should be before another.
	/// @tparam Equal The function struct used to check if two values are equal.
	template<class T, class Lower = LowerComp<T>, class Equal = EqualComp<T>>
	class set {
	public:
		/// @brief The set's key.
		typedef T key_type;
		/// @brief The set's value.
		typedef T value_type;
		/// @brief A reference to the set's value.
		typedef T& reference;
		/// @brief A const reference to the set's value.
		typedef const T& const_reference;
		/// @brief A pointer to the set's value.
		typedef T* pointer;
		/// @brief A const pointer to the set's value.
		typedef const T* const_pointer;
		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Creates an empty set.
		constexpr set() = default;
		/// @brief Copies the given set.
		/// @param other The set to copy.
		set(const set& other) : setSize(other.setSize), setCapacity(other.setCapacity), setData((pointer)AllocMemory(setCapacity * sizeof(value_type))) {
			// Check if the memory was allocated correctly
			if(!setData)
				throw BadAllocException("Failed to allocate set data!");

			// Copy every value from the other set
			const_pointer ptr2 = other.setData;
			pointer end = setData + setSize;

			for(pointer ptr1 = setData; ptr1 != end; ++ptr1)
				new(ptr1) value_type(*ptr2++);
		}
		/// @brief Moves the contents from one set to another.
		/// @param other The set to move from.
		set(set&& other) noexcept : setSize(other.setSize), setCapacity(other.setCapacity), setData(other.setData) {
			// Unlink the data from the other set
			other.setData = nullptr;
		}
		/// @brief Creates a vector with the elements from the given list.
		/// @param list The list to copy from.
		set(std::initializer_list<value_type> list) : setSize(0), setCapacity(1), setData(nullptr) {
			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(list.size() < max_size(), "The set's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((setCapacity << step) < list.size())
					setCapacity <<= step;
			setCapacity <<= 1;

			// Allocate the set's memory
			setData = (pointer)AllocMemory(setCapacity * sizeof(value_type));

			if(!setData)
				throw BadAllocException("Failed to allocate set data!");

			// Insert every value from the list
			const_pointer end = list.end();

			for(const_pointer ptr = list.begin(); ptr != end; ++ptr) {
				// Find the position for the current value
				size_type pos = setSize;
				for(size_type step = setCapacity; step; step >>= 1)
					if(pos >= step && !Lower()(setData[pos - step], *ptr))
						pos -= step;
					
				if(pos != setSize && Equal()(setData[pos], *ptr))
					continue;
				
				// Move part of the set forward to make room for the new value
				memmove(setData + pos + 1, setData + pos, (setSize - pos) * sizeof(value_type));

				// Create the new value
				new(setData + pos) value_type(*ptr);

				// Increment the set's size
				++setSize;
			}
		}
		/// @brief Copies the given std library set into this set.
		/// @param other THe std library set to copy from.
		set(const std::set<value_type>& other) : setSize(other.size()), setCapacity(1), setData(nullptr) {
			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((setCapacity << step) < other.size())
					setCapacity <<= step;
			setCapacity <<= 1;

			// Allocate the set's memory
			setData = (pointer)AllocMemory(setCapacity * sizeof(value_type));

			if(!setData)
				throw BadAllocException("Failed to allocate set data!");
			
			// Copy every value from the given set
			const_pointer ptr2 = other.begin();
			pointer end = setData + setSize;

			for(pointer ptr1 = setData; ptr1 != end; ++ptr1)
				new(ptr1) value_type(*ptr2++);
		}

		/// @brief Copies the given set into this set.
		/// @param other The set to copy from.
		/// @return A reference to this set.
		set& operator=(const set& other) {
			// Exit the function if the given set is the same as this set
			if(&other == this)
				return *this;

			if(setData) {
				// Destruct every value in the set
				pointer end = setData + setSize;

				for(pointer ptr = setData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the set's memory
				FreeMemory(setData);
			}

			// Set the set's new values
			setSize = other.setSize;
			setCapacity = other.setCapacity;
			setData = (pointer)AllocMemory(setCapacity * sizeof(value_type));

			// Check if the memory was allocated correctly
			if(!setData)
				throw BadAllocException("Failed to allocate set data!");

			// Copy every value from the given set
			const_pointer ptr2 = other.setData;
			pointer end = setData + setSize;

			for(pointer ptr1 = setData; ptr1 != end; ++ptr1)
				new(ptr1) value_type(*ptr2++);
			
			return *this;
		}
		/// @brief Moves the contents from the given set into this set.
		/// @param other The set to move from.
		/// @return A reference to this set.
		set& operator=(set&& other) {
			if(setData) {
				// Destruct every value in the set
				pointer end = setData + setSize;

				for(pointer ptr = setData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the set's memory
				FreeMemory(setData);
			}

			// Set the set's new data
			setSize = other.setSize;
			setCapacity = other.setCapacity;
			setData = other.setData;

			// Unlink the data from the given set
			other.setData = nullptr;

			return *this;
		}
		/// @brief Copies the elements from the given list into this vector.
		/// @param list The list to copy from.
		/// @return A reference to this set.
		set& operator=(std::initializer_list<value_type> list) {
			if(setData) {
				// Destruct every value in the set
				pointer end = setData + setSize;

				for(pointer ptr = setData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the set's memory
				FreeMemory(setData);
			}

			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(list.size() < max_size(), "The set's size must be lower than the maximum possible size!")

			// Set the set's new data
			setSize = 0;
			setCapacity = 1;

			// Set the set's capacity to the lowest power of 2 higher than its size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((setCapacity << step) < list.size())
					setCapacity <<= step;
			setCapacity <<= 1;

			// Allocate the set's memory
			setData = (pointer)AllocMemory(setCapacity * sizeof(value_type));

			if(!setData)
				throw BadAllocException("Failed to allocate set data!");

			// Insert every value from the list
			const_pointer end = list.end();

			for(const_pointer ptr = list.begin(); ptr != end; ++ptr) {
				// Find the position for the current value
				size_type pos = setSize;
				for(size_type step = setCapacity; step; step >>= 1)
					if(pos >= step && !Lower()(setData[pos - step] < *ptr))
						pos -= step;
				
				// Check if the current value is already in the set
				if(pos != setSize && Equal()(ptr, setData[pos]))
					continue;
				
				// Move part of the set forward to make room for the new value
				memmove(setData + pos + 1, setData + pos, (setSize - pos) * sizeof(value_type));

				// Create the new value
				new(setData + pos) value_type(*ptr);

				// Increment the set's size
				++setSize;
			}
		}

		operator std::set<value_type>() const {
			std::set<value_type> set;
			
			const_pointer end = setData + setSize;
			for(const_pointer ptr = setData; ptr != end; ++ptr)
				set.insert(*ptr);
			
			return set;
		}

		/// @brief Inserts the given value into the set.
		/// @param val The value to insert.
		/// @return A pair containing a pointer to the inserted value or an identical value that was already in the set and a bool that is set to true if a new element was inserted, otherwise false.
		pair<pointer, bool8_t> insert(const_reference val) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;

			// Find the position for the given value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Check if the value is already in the set
			if(pos != setSize && Equal()(val, setData[pos]))
				return { setData + pos, false };
			
			// Increment the set's size
			++setSize;

			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(setSize > setCapacity) {
				// Set the capacity to the lowest power of 2 higher than the set's size
				setCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((setCapacity << step) < setSize)
						setCapacity <<= step;
				setCapacity <<= 1;

				// Reallocate the set's memory
				setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

				if(!setData)
					throw BadAllocException("Failed to allocate set data!");
			}

			// Move part of the set forward to make room for the new value
			memmove(setData + pos + 1, setData + pos, (setSize - 1 - pos) * sizeof(value_type));

			// Create the new value
			new (setData + pos) value_type(val);

			return { setData + pos, true };
		}
		/// @brief Inserts the given value into the set.
		/// @param val The value to insert.
		/// @return A pair containing a pointer to the inserted value or an identical value that was already in the set and a bool that is set to true if a new element was inserted, otherwise false.
		pair<pointer, bool8_t> insert(value_type&& val) {
			// Set the binary search's step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) < setSize)
					step <<= stepStep;

			// Find the position for the given value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Check if the value is already in the set
			if(pos != setSize && Equal()(val, setData[pos]))
				return { setData + pos, false };
			
			// Increment the set's size
			++setSize;

			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(setSize > setCapacity) {
				// Set the capacity to the lowest power of 2 higher than the set's size
				setCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((setCapacity << step) < setSize)
						setCapacity <<= step;
				setCapacity <<= 1;

				// Reallocate the set's memory
				setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

				if(!setData)
					throw BadAllocException("Failed to allocate set data!");
			}

			// Move part of the set forward to make room for the new value
			memmove(setData + pos + 1, setData + pos, (setSize - 1 - pos) * sizeof(value_type));

			// Create the new value
			new (setData + pos) value_type(val);

			return { setData + pos, true };
		}
		/// @brief Inserts the given value into the set.
		/// @param pos A hint for the position where the element can be inserted. Note that the element won't always be inserted at this position, since the values in a set must be in a specific order.
		/// @param val THe value to insert.
		/// @return A pointer to the inserted value or to an identical value that was already in the set.
		pointer insert(const_pointer pos, const_reference val) {
			// Assert that the given position hint must be in range
			WFE_ASSERT(pos >= begin() && pos <= end(), "The given position hint must be in range!");

			// Check if the position hint is correct
			if(pos != end() && Equal()(val, *pos))
				return (pointer)pos;
			if((pos == begin() || Lower()(*(pos - 1), val)) && (pos != end() && Lower()(val, *pos))) {
				// Increment the set's size
				++setSize;

				// Assert that the set's size must be lower than the maximum possible size
				WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

				// Save the position hint's index relative to the set's data
				size_type posInd = pos - setData;

				// Check if memory needs to be reallocated
				if(setSize > setCapacity) {
					// Set the capacity to the lowest power of 2 higher than the set's size
					setCapacity = 1;
					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((setCapacity << step) < setSize)
							setCapacity <<= step;
					setCapacity <<= 1;

					// Reallocate the set's memory
					setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

					if(!setData)
						throw BadAllocException("Failed to allocate set data!");
				}

				// Move part of the set forward to make room for the new value
				memmove(setData + posInd + 1, setData + posInd, (setSize - 1 - posInd) * sizeof(value_type));

				// Create the new value
				new(setData + posInd) value_type(val);

				return setData + posInd;
			}

			// Compare the position hint to the given value
			size_type posBegin, size;

			if(pos == end()) {
				// Look through the whole set
				posBegin = 0;
				size = setSize;
			} else if(Lower()(val, *pos)) {
				// Look through the values before the position hint
				posBegin = 0;
				size = pos - setData - 1;
			} else {
				// Look through the values after the position hint
				posBegin = pos - setData + 1;
				size = setData + setSize - pos - 1;
			}

			// Set the binary search's step to the highest power of 2 lower than or equal to the set region's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= size)
					step <<= stepStep;

			// Find the position for the given value
			size_type posInd = posBegin + size;
			for(; step; step >>= 1)
				if(posInd >= step + posBegin && !Lower()(setData[posInd - step], val))
					posInd -= step;
	
			// Check if the value is already in the set
			if(posInd != setSize && Equal()(val, setData[posInd]))
				return setData + posInd;
			
			// Increment the set's size
			++setSize;

			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(setSize > setCapacity) {
				// Set the capacity to the lowest power of 2 higher than the set's size
				setCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((setCapacity << step) < setSize)
						setCapacity <<= step;
				setCapacity <<= 1;

				// Reallocate the set's memory
				setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

				if(!setData)
					throw BadAllocException("Failed to allocate set data!");
			}

			// Move part of the set forward to make room for the new value
			memmove(setData + posInd + 1, setData + posInd, (setSize - 1 - posInd) * sizeof(value_type));

			// Create the new value
			new (setData + posInd) value_type(val);

			return setData + posInd;
		}
		/// @brief Inserts the given value into the set.
		/// @param pos A hint for the position where the element can be inserted. Note that the element won't always be inserted at this position, since the values in a set must be in a specific order.
		/// @param val THe value to insert.
		/// @return A pointer to the inserted value or to an identical value that was already in the set.
		pointer insert(const_pointer pos, value_type&& val) {
			// Assert that the given position hint must be in range
			WFE_ASSERT(pos >= begin() && pos <= end(), "The given position hint must be in range!");

			// Check if the position hint is correct
			if(pos != end() && Equal()(val, *pos))
				return (pointer)pos;
			if((pos == begin() || Lower()(*(pos - 1), val)) && (pos != end() && Lower()(val, *pos))) {
				// Increment the set's size
				++setSize;

				// Save the position hint's index relative to the set's data
				size_type posInd = pos - setData;

				// Check if memory needs to be reallocated
				if(setSize > setCapacity) {
					// Set the capacity to the lowest power of 2 higher than the set's size
					setCapacity = 1;
					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((setCapacity << step) < setSize)
							setCapacity <<= step;
					setCapacity <<= 1;

					// Reallocate the set's memory
					setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

					if(!setData)
						throw BadAllocException("Failed to allocate set data!");
				}

				// Move part of the set forward to make room for the new value
				memmove(setData + posInd + 1, setData + posInd, (setSize - 1 - posInd) * sizeof(value_type));

				// Create the new value
				new(setData + posInd) value_type(val);

				return setData + posInd;
			}

			// Compare the position hint to the given value
			size_type posBegin, size;

			if(pos == end()) {
				// Look through the whole set
				posBegin = 0;
				size = setSize;
			} else if(Lower()(val, *pos)) {
				// Look through the values before the position hint
				posBegin = 0;
				size = pos - setData - 1;
			} else {
				// Look through the values after the position hint
				posBegin = pos - setData + 1;
				size = setData + setSize - pos - 1;
			}

			// Set the binary search's step to the highest power of 2 lower than or equal to the set region's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= size)
					step <<= stepStep;

			// Find the position for the given value
			size_type posInd = posBegin + size;
			for(; step; step >>= 1)
				if(posInd >= step + posBegin && !Lower()(setData[posInd - step], val))
					posInd -= step;
	
			// Check if the value is already in the set
			if(posInd != setSize && Equal()(val, setData[posInd]))
				return setData + posInd;
			
			// Increment the set's size
			++setSize;

			// Assert that the set's size must be lower than the maximum possible size
			WFE_ASSERT(setSize < max_size(), "The set's size must be lower than the maximum possible size!")

			// Check if memory needs to be reallocated
			if(setSize > setCapacity) {
				// Set the capacity to the lowest power of 2 higher than the set's size
				setCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((setCapacity << step) < setSize)
						setCapacity <<= step;
				setCapacity <<= 1;

				// Reallocate the set's memory
				setData = (pointer)ReallocMemory(setData, setCapacity * sizeof(value_type));

				if(!setData)
					throw BadAllocException("Failed to allocate set data!");
			}

			// Move part of the set forward to make room for the new value
			memmove(setData + posInd + 1, setData + posInd, (setSize - 1 - posInd) * sizeof(value_type));

			// Create the new value
			new (setData + posInd) value_type(val);

			return setData + posInd;
		}
		/// @brief Inserts the elements from the given list into the set.
		/// @param list The list to insert from.
		void insert(std::initializer_list<value_type> list) {
			// Exit the function if there are no items in the list
			if(!list.size())
				return;
			
			// Insert the first item and store the position
			pointer posHint = insert(*list.begin()).first;

			// Insert every other item using the hint from before
			pointer end = (pointer)list.end();
			for(pointer ptr = (pointer)list.begin() + 1; ptr != end; ++ptr)
				posHint = insert(posHint, *ptr);
		}
		/// @brief Erases the element at the given position.
		/// @param pos The position to erase from.
		/// @return A pointer to the space occupied by the erase value.
		pointer erase(const_pointer pos) {
			// Assert that the given position must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given position must be in range!")

			// Decrement the set's size
			--setSize;

			// Destruct the value at the given position
			pos->~value_type();

			// Move part of the set backwards to erase the value at the given position
			memcpy((pointer)pos, pos + 1, (setData + setSize - pos) * sizeof(value_type));

			return (pointer)pos;
		}
		/// @brief Erases the given value from the set.
		/// @param val The value to erase from the set.
		/// @return The number of values erased.
		size_type erase(const_reference val) {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;

			// Set the binary search's step to the highest power of 2 lower than or equal to the set region's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) < setSize)
					step <<= stepStep;

			// Find the position for the given value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;

			// Check if the value doesn't exist in the set
			if(!Equal()(setData[pos], val))
				return 0;
			
			// Decrement the set's size
			--setSize;

			// Destruct the value at the given position
			setData[pos].~value_type();

			// Move part of the set backwards to erase the value at the given position
			memcpy(setData + pos, setData + pos + 1, (setSize - pos) * sizeof(value_type));

			return 1;
		}
		/// @brief Erased the values between the two given pointers.
		/// @param first A pointer to the first erased value.
		/// @param last A pointer to right after the last erased value.
		/// @return A pointer to the space occupied by the first erased value.
		pointer erase(const_pointer first, const_pointer last) {
			// Assert that the given pointers must be in range
			WFE_ASSERT(first >= begin() && first < end() && last > begin() && last <= end(), "The given pointers must be in range!")

			// Assert that the first pointer must be smaller than the last
			WFE_ASSERT(first < last, "The first pointer must be smaller than the last!");

			// Calculate the number of erased values
			size_type erasedSize = last - first;

			// Destruct every value in the given range
			for(pointer ptr = (pointer)first; ptr != last; ++ptr)
				ptr->~value_type();
			
			// Move part of the set backwards to erase the values in the given range
			memcpy((pointer)first, last, (setData + setSize - last) * sizeof(value_type));

			// Decrease the set's size
			setSize -= erasedSize;

			return (pointer)first;
		}
		/// @brief Swaps the two set's contents.
		/// @param other The set to swap with.
		void swap(set& other) {
			// Exit the function if the two sets are the same
			if(&other == this)
				return;
			
			// Save this set's values
			size_type auxSize = setSize;
			size_type auxCapacity = setCapacity;
			pointer auxData = setData;

			// Set this set's new values
			setSize = other.setSize;
			setCapacity = other.setCapacity;
			setData = other.setData;

			// Set the other set's new values
			other.setSize = auxSize;
			other.setCapacity = auxCapacity;
			other.setData = auxData;
		}
		/// @brief Clears the set.
		void clear() {
			if(setSize) {
				// Destruct every value in the set
				pointer end = setData + setSize;
				for(pointer ptr = setData; ptr != end; ++ptr)
					ptr->~value_type();
			}

			// Set the set's size to 0
			setSize = 0;
		}
		/// @brief Emplaces a value in the set.
		/// @tparam ...Args The arg types to create the value with.
		/// @param ...args The args to create the value with.
		/// @return A pair containing a pointer to the emplaced value or an identical value that was already in the set and a bool that is set to true if a new element was inserted, otherwise false.
		template<class... Args>
		pair<pointer, bool8_t> emplace(Args&&... args) {
			// Construct the value
			value_type val(args...);

			// Insert the value
			return insert(val);
		}
		/// @brief Emplaces a value in the set.
		/// @tparam ...Args The arg types to create the value with.
		/// @param pos A hint for the position where the element can be emplaced. Note that the element won't always be emplaced at this position, since the values in a set must be in a specific order.
		/// @param ...args The args to create the value with.
		/// @return A pointer to the emplaced value or to an identical value that was already in the set.
		template<class... Args>
		pointer emplace_hint(const_pointer pos, Args&&... args) {
			// Construct the value
			value_type val(args...);

			// Insert the value with the given hint
			return insert(pos, val);
		}

		/// @brief Gets the beginning of the set.
		/// @return A pointer to the beginning of the vector.
		pointer begin() {
			return setData;
		}
		/// @brief Gets the beginning of the set.
		/// @return A const pointer to the beginning of the vector.
		const_pointer begin() const {
			return setData;
		}
		/// @brief Gets the end of the set.
		/// @return A pointer to the end of the vector.
		pointer end() {
			return setData + setSize;
		}
		/// @brief Gets the end of the set.
		/// @return A const pointer to the end of the vector.
		const_pointer end() const {
			return setData + setSize;
		}

		/// @brief Tests if the set is empty.
		/// @return True if the set is empty, otherwise false.
		bool8_t empty() const {
			return !setSize;
		}
		/// @brief Gets the size of the set.
		/// @return The size of the set.
		size_type size() const {
			return setSize;
		}
		/// @brief Gets the maximum possible size of the set.
		/// @return The maximum possible size of the set.
		size_type max_size() const {
			return SIZE_T_MAX / sizeof(value_type);
		}

		/// @brief Finds the given value in the set.
		/// @param val The value to look for.
		/// @return A pointer to the value, if it was found, otherwise set::end().
		pointer find(const_reference val) {
			// Exit the function if the set is empty
			if(!setSize)
				return setData;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Check if the wanted value was found
			if(pos != setSize && Equal()(setData[pos], val))
				return setData + pos;
			
			return end();
		}
		/// @brief Finds the given value in the set.
		/// @param val The value to look for.
		/// @return A const pointer to the value, if it was found, otherwise set::end().
		const_pointer find(const_reference val) const {
			// Exit the function if the set is empty
			if(!setSize)
				return setData;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Check if the wanted value was found
			if(pos != setSize && Equal()(setData[pos], val))
				return setData + pos;
			
			return end();
		}
		/// @brief Finds the number of times the given value appears in the set.
		/// @param val The value to look for.
		/// @return The number of times the given value appears in the set.
		size_type count(const_reference val) const {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Check if the wanted value was found
			return pos != setSize && Equal()(setData[pos], val);
		}
		/// @brief Finds the first element that isn't lower than the given value.
		/// @param val The value to compare with.
		/// @return A pointer to the wanted value, if any exists, otherwise set::end().
		pointer lower_bound(const_reference val) {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Return a pointer to the wanted value
			return setData + pos;
		}
		/// @brief Finds the last element that isn't lower than the given value.
		/// @param val The value to compare with.
		/// @return A const pointer to the wanted value, if any exists, otherwise set::end().
		const_pointer lower_bound(const_reference val) const {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && !Lower()(setData[pos - step], val))
					pos -= step;
			
			// Return a pointer to the wanted value
			return setData + pos;
		}
		/// @brief Finds the first element that is higher than the given value.
		/// @param val The value to compare with.
		/// @return A pointer to the wanted value, if any exists, otherwise set::end().
		pointer upper_bound(const_reference val) {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && Lower()(val, setData[pos - step]))
					pos -= step;
			
			// Return a pointer to the wanted value
			return setData + pos;
		}
		/// @brief Finds the first element that is higher than the given value.
		/// @param val The value to compare with.
		/// @return A const pointer to the wanted value, if any exists, otherwise set::end().
		const_pointer upper_bound(const_reference val) const {
			// Exit the function if the set is empty
			if(!setSize)
				return 0;
			
			// Set the binary search step to the highest power of 2 lower than or equal to the set's size
			size_type step = 1;
			for(size_type stepStep = sizeof(size_type) << 2; stepStep; stepStep >>= 1)
				if((step << stepStep) <= setSize)
					step <<= stepStep;
			
			// Look for the wanted value
			size_type pos = setSize;
			for(; step; step >>= 1)
				if(pos >= step && Lower()(val, setData[pos - step]))
					pos -= step;
			
			// Return a pointer to the wanted value
			return setData + pos;
		}
		/// @brief Finds the range that includes all elements in the container equal to the given value.
		/// @param val The value to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<pointer, pointer> equal_range(const_reference val) {
			pair<pointer, pointer> range;

			// Find the range's lower bound
			range.first = lower_bound(val);

			// Set the range's upper bound based on the lower bound
			if(range.first != end() && val == *range.first)
				range.second = range.first + 1;
			else
				range.second = range.first;
			
			return range;
		}
		/// @brief Finds the range that includes all elements in the container equal to the given value.
		/// @param val The value to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<const_pointer, const_pointer> equal_range(const_reference val) const {
			pair<const_pointer, const_pointer> range;

			// Find the range's lower bound
			range.first = lower_bound(val);

			// Set the range's upper bound based on the lower bound
			if(range.first != end() && val == *range.first)
				range.second = range.first + 1;
			else
				range.second = range.first;
			
			return range;
		}

		/// @brief Destroys the set.
		~set() {
			if(setData) {
				// Destruct every value in the set
				pointer end = setData + setSize;
				for(pointer ptr = setData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the set's data
				FreeMemory(setData);
			}
		}
	private:
		size_type setSize = 0;
		size_type setCapacity = 0;
		pointer setData = nullptr;
	};
}