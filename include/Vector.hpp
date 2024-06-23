#pragma once

#include "Defines.hpp"
#include "Allocator.hpp"
#include "Exception.hpp"
#include "Memory.hpp"
#include <initializer_list>
#include <vector>

namespace wfe {
	/// @brief Holds a vector of elements.
	/// @tparam T The type of the vector's values.
	template<class T>
	class vector {
	public:
		/// @brief The vector's value.
		typedef T value_type;
		/// @brief A reference to the vector's value.
		typedef T& reference;
		/// @brief A const reference to the vector's value.
		typedef const T& const_reference;
		/// @brief A pointer to the vector's value.
		typedef T* pointer;
		/// @brief A const pointer to the vector's value.
		typedef const T* const_pointer;
		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Creates an empty vector.
		constexpr vector() = default;
		/// @brief Copies the given vector.
		/// @param other The vector to copy.
		vector(const vector& other) : vecSize(other.vecSize), vecCapacity(other.vecCapacity), vecData((pointer)AllocMemory(vecCapacity * sizeof(value_type))) {
			// Check if the memory was allocated correctly
			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");
			
			// Copy every value afrom the given vector
			for(size_type i = 0; i < vecSize; ++i)
				new(vecData + i) value_type(other.vecData[i]);
		}
		/// @brief Moves the contents from one vector to another.
		/// @param other The vector to move from.
		vector(vector&& other) noexcept : vecSize(other.vecSize), vecCapacity(other.vecCapacity), vecData(other.vecData) {
			// Unlink the data from the given vector
			other.vecData = nullptr;
		}
		/// @brief Creates a vector with the given size.
		/// @param n The size of the vector.
		vector(size_type n) : vecSize(n), vecCapacity(1), vecData(nullptr) {
			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;
			
			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Construct every value in the vector
			pointer end = vecData + vecSize;
			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type();
		}
		/// @brief Creates a vector with the given size and fills it with the
		/// @param n The size of the vector.
		/// @param val The value to fill the vector with.
		vector(size_type n, const_reference val) : vecSize(n), vecCapacity(1), vecData(nullptr) {
			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;
			
			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Construct every value in the vector
			pointer end = vecData + vecSize;
			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type(val);
		}
		/// @brief Creates a vector with the elements from the given list.
		/// @param list The initializer list to copy from.
		vector(std::initializer_list<value_type> list) : vecSize(list.size()), vecCapacity(1), vecData(nullptr) {
			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;
			
			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Copy every value in the vector from the initializer list
			pointer end = vecData + vecSize;
			const_pointer listIter = list.begin();

			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type(*listIter++);
		}
		/// @brief Copies the given std library vector.
		/// @param other The std library vector to copy.
		vector(const std::vector<T>& other) : vecSize(other.size()), vecCapacity(other.capacity()), vecData((pointer)AllocMemory(vecCapacity * sizeof(value_type))) {
			// Check if the memory was allocated correctly
			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");
			
			// Copy every value from the given vector
			for(size_type i = 0; i != vecSize; ++i)
				new(vecData + i) value_type(other[i]);
		}

		/// @brief Copies the given vector into this vector.
		/// @param other The vector to copy from.
		/// @return A reference to this vector.
		vector& operator=(const vector& other) {
			// Exit the function if the vectors are the same
			if(&other == this)
				return *this;
			
			// Free the vector's data (if it exists)
			if(vecData) {
				// Destruct every object in the vector
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					end->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}

			// Set the vector's new values
			vecSize = other.vecSize;
			vecCapacity = other.vecCapacity;
			vecData = (pointer)AllocMemory(vecSize * sizeof(value_type));

			// Check if the memory was allocated correctly
			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Copy every value from the given vector
			for(size_type i = 0; i != vecSize; ++i)
				new(vecData + i) value_type(other[i]);

			return *this;
		}
		/// @brief Moves the contents from the given vector into this vector.
		/// @param other The vector to move from.
		/// @return A reference to this vector.
		vector& operator=(vector&& other) {
			// Free the vector's data (if it exists)
			if(vecData) {
				// Destruct every object in the vector
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					end->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}

			// Set the vector's new values
			vecSize = other.vecSize;
			vecCapacity = other.vecCapacity;
			vecData = other.vecData;

			// Unlink the data from the given vector
			other.vecData = nullptr;

			return *this;
		}
		/// @brief Copies the elements from the given list into this vector.
		/// @param list The initializer list to copy from.
		/// @return A reference to this vector.
		vector& operator=(std::initializer_list<value_type> list) {
			// Free the vector's data (if it exists)
			if(vecData) {
				// Destruct every object in the vector
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					end->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}

			// Set the vector's new values
			vecSize = list.size();
			vecCapacity = 1;

			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;

			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Copy every value in the vector from the initializer list
			pointer end = vecData + vecSize;
			pointer listIter = list.begin();

			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type(*listIter++);
			
			return *this;
		}

		/// @brief Converts the vector to an std library vector.
		operator std::vector<value_type>() const {
			// Created the std library vector
			std::vector<value_type> vec(vecSize);

			// Copy every element to the std library vector
			pointer ptr = vecData;
			for(auto& elem : vec)
				elem = *ptr++;
			
			return vec;
		}

		/// @brief Gets the wanted element from the vector.
		/// @param ind The index of the wanted element.
		/// @return A reference to the wanted element.
		reference operator[](size_type ind) {
			// Assert that the given index must be in range
			WFE_ASSERT(ind < vecSize, "The given index must be in range!")

			// Return a reference to the wanted element
			return vecData[ind];
		}
		/// @brief Gets the wanted element from the vector.
		/// @param ind The index of the wanted element.
		/// @return A const reference to the wanted element.
		const_reference operator[](size_type ind) const {
			// Assert that the given index must be in range
			WFE_ASSERT(ind < vecSize, "The given index must be in range!")

			// Return a const reference to the wanted element
			return vecData[ind];
		}
		/// @brief Gets the first element from the vector.
		/// @return A reference to the vector's first element.
		reference front() {
			// Return a reference to the first element
			return *vecData;
		}
		/// @brief Gets the first element from the vector.
		/// @return A const reference to the vector's first element.
		const_reference front() const {
			// Return a const reference to the first element
			return *vecData;
		}
		/// @brief Gets the last element from the vector.
		/// @return A reference to the vector's last element.
		reference back() {
			// Return a reference to the last element
			return vecData[vecSize - 1];
		}
		/// @brief Gets the last element from the vector.
		/// @return A const reference to the vector's last element.
		const_reference back() const {
			// Return a reference to the last element
			return vecData[vecSize - 1];
		}
		/// @brief Gets the vector's data.
		/// @return A pointer to the vector's data.
		pointer data() {
			// Return a pointer to the data
			return vecData;
		}
		/// @brief Gets the vector's data.
		/// @return A const pointer to the vector's data.
		const_pointer data() const {
			// Return a const pointer to the data
			return vecData;
		}

		/// @brief Sets this vector's size to the given size and fills it with the new element.
		/// @param n The vector's new size.
		/// @param val The element to fill the vector with.
		void assign(size_type n, const_reference val) {
			// Free the vector's data (if it exists)
			if(vecData) {
				// Destruct every object in the vector
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					end->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}

			// Set the vector's new values
			vecSize = n;
			vecCapacity = 1;

			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;

			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Set every value in the vector to the given value
			pointer end = vecData + vecSize;
			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type(val);
		}
		/// @brief Sets the vector's contents to the contents of the given list.
		/// @param list The initializer list to copy from.
		void assign(std::initializer_list<value_type> list) {
			// Free the vector's data (if it exists)
			if(vecData) {
				// Destruct every object in the vector
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					end->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}

			// Set the vector's new values
			vecSize = list.size();
			vecCapacity = 1;

			// Assert that the vector's size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's size must be lower than the maximum possible size!")

			// Set the capacity to the lowest power of 2 higher than the vector's size            
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((vecCapacity << step) < vecSize)
					vecCapacity <<= step;
			vecCapacity <<= 1;

			// Allocate the vector's data
			vecData = (pointer)AllocMemory(vecCapacity * sizeof(value_type));

			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");

			// Copy every value in the vector from the initializer list
			pointer end = vecData + vecSize;
			const_pointer listIter = list.begin();

			for(pointer ptr = vecData; ptr != end; ++ptr)
				new(ptr) value_type(*listIter++);
		}
		/// @brief Places the given value at the end of the vector.
		/// @param val The value to place at the end.
		void push_back(const_reference val) {
			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Copy the value into the new space
			new(vecData + vecSize - 1) value_type(val);
		}
		/// @brief Places the given value at the end of the vector.
		/// @param val The value to place at the end.
		void push_back(value_type&& val) {
			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move the value into the new space
			new(vecData + vecSize - 1) value_type(val);
		}
		/// @brief Removes the last element from the vector.
		void pop_back() {
			// Resize the vector
			--vecSize;

			// Destruct the previous last element
			(vecData + vecSize)->~value_type();
		}
		/// @brief Inserts the given value at the given position.
		/// @param pos The position to insert at.
		/// @param val The value to insert.
		/// @return A pointer to the inserted value.
		pointer insert(const_pointer pos, const_reference val) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Get the position to insert at relative to the vector's data
			size_type posInd = pos - vecData;

			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move part of the vector forward to make room for the new value
			memmove(vecData + posInd + 1, vecData + posInd, (vecSize - posInd - 1) * sizeof(value_type));

			// Copy the value into the new space
			new(vecData + posInd) value_type(val);

			return vecData + posInd;
		}
		/// @brief Inserts the given value at the given position.
		/// @param pos The position to insert at.
		/// @param val The value to insert.
		/// @return A pointer to the inserted value.
		pointer insert(const_pointer pos, value_type&& val) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Get the position to insert at relative to the vector's data
			size_type posInd = pos - vecData;

			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move part of the vector forward to make room for the new value
			memmove(vecData + posInd + 1, vecData + posInd, (vecSize - posInd - 1) * sizeof(value_type));

			// Move the value into the new space
			new(vecData + posInd) value_type(val);

			return vecData + posInd;
		}
		/// @brief Inserts the specified amount of consecutive copies of the given value at the given position.
		/// @param pos The position to insert at.
		/// @param n The number of consecutive copies to insert.
		/// @param val The value whose consecutive copies to insert.
		/// @return A pointer to the first inserted copy.
		pointer insert(const_pointer pos, size_type n, const_reference val) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Get the position to insert at relative to the vector's data
			size_type posInd = pos - vecData;

			// Save the vector's old size
			size_type oldSize = vecSize;

			// Resize the vector
			vecSize += n;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move part of the vector forward to make room for the new value
			memmove(vecData + posInd + n, vecData + posInd, (oldSize - posInd) * sizeof(value_type));

			// Fill the new space with the given value
			pointer end = vecData + posInd + n;
			for(pointer ptr = vecData + posInd; ptr != end; ++ptr)
				new(ptr) value_type(val);
			
			return vecData + posInd;
		}
		/// @brief Inserts the items from the given list at the given position.
		/// @param pos The position to insert at.
		/// @param list The list to insert from.
		/// @return A pointer to the first inserted value.
		pointer insert(const_pointer pos, std::initializer_list<value_type> list) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Get the position to insert at relative to the vector's data
			size_type posInd = pos - vecData;

			// Save the vector's old size
			size_type oldSize = vecSize;

			// Resize the vector
			vecSize += list.size();

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move part of the vector forward to make room for the new value
			memmove(vecData + posInd + list.size(), vecData + posInd, (oldSize - posInd) * sizeof(value_type));

			// Fill the new space with the given value
			pointer end = vecData + posInd + list.size();
			const_pointer listPtr = list.begin();

			for(pointer ptr = vecData + posInd; ptr != end; ++ptr)
				new(ptr) value_type(*listPtr++);
			
			return vecData + posInd;
		}
		/// @brief Erases the value at the given position.
		/// @param pos The position to erase from.
		/// @return A pointer to the space occupied by the erased value.
		pointer erase(const_pointer pos) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Resize the vector
			--vecSize;
			
			// Destruct the value at the given position
			pos->~value_type();

			// Move part of the vector back to remove the destructed value
			memmove((pointer)pos, pos + 1, (vecData + vecSize + 1 - pos) * sizeof(value_type));

			return (pointer)pos;
		}
		/// @brief Erases the values between the two given pointers.
		/// @param first A pointer to the first erased value.
		/// @param last A pointer to right after the last erased value.
		/// @return A pointer to the space occupied by the first erased value.
		pointer erase(const_pointer first, const_pointer last) {
			// Assert that the given pointers must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last < end(), "The given pointers must be in range!");

			// Assert that the first pointer must be shaller than the last pointer
			WFE_ASSERT(first < last, "The first pointer must be shaller than the last pointer!")

			// Get the size of the erased block
			size_type erasedSize = last - first;

			// Save the vector's old size
			size_type oldSize = vecSize;

			// Resize the vector
			vecSize -= erasedSize;

			// Destruct every value in the given region
			for(pointer ptr = (pointer)first; ptr != last; ++ptr)
				ptr->~value_type();
			
			// Move part of the vector back to remove the destructed values
			memmove((pointer)first, last, (vecData + oldSize - last) * sizeof(value_type));

			return (pointer)first;
		}
		/// @brief Swaps the two vector's contents.
		/// @param other The vector to swap with.
		void swap(vector& other) {
			// Exit the function if the two vectors are the same
			if(&other == this)
				return;
			
			// Save the vector's values
			size_type auxSize = vecSize;
			size_type auxCapacity = vecCapacity;
			pointer auxData = vecData;

			// Set this vector's new values
			vecSize = other.vecSize;
			vecCapacity = other.vecCapacity;
			vecData = other.vecData;

			// Set the other vector's new values
			other.vecSize = auxSize;
			other.vecCapacity = auxCapacity;
			other.vecData = auxData;
		}
		/// @brief Clears the vector.
		void clear() {
			// Destruct every value in the vector (if they exist)
			if(vecData) {
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					ptr->~value_type();
			}

			// Set the vector's size to 0
			vecSize = 0;
		}
		/// @brief Emplaces a value at the given position.
		/// @tparam ...Args The arg types to create the value with.
		/// @param pos The position to emplace the value at.
		/// @param ...args The args to create the value with.
		/// @return A pointer to the emplaced value.
		template<class... Args>
		pointer emplace(const_pointer pos, Args&&... args) {
			// Assert that the given pointer must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The given pointer must be in range!")

			// Get the position to insert at relative to the vector's data
			size_type posInd = pos - vecData;

			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Move part of the vector forward to make room for the new value
			memmove(vecData + posInd + 1, vecData + posInd, (vecSize - posInd - 1) * sizeof(value_type));

			// Construct the value into the new space
			new(vecData + posInd) value_type(args...);

			return vecData + posInd;
		}
		/// @brief Emplaces a value at the end of the vector.
		/// @tparam ...Args The arg types to create the value with.
		/// @param ...args The args to create the value with.
		template<class... Args>
		void emplace_back(Args&&... args) {
			// Resize the vector
			++vecSize;

			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(vecSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Check if memory needs to be reallocated
			if(vecSize > vecCapacity) {
				// Set the capacity to the lowest power of 2 higher than the vector's size
				size_type newCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((newCapacity << step) < vecSize)
						newCapacity <<= step;
				newCapacity <<= 1;
				
				// Allocate the new space in the vector
				reserve(newCapacity);
			}

			// Construct the value into the new space
			new(vecData + vecSize - 1) value_type(args...);
		}

		/// @brief Gets the beginning of the vector.
		/// @return A pointer to the beginning of the vector.
		pointer begin() {
			return vecData;
		}
		/// @brief Gets the beginning of the vector.
		/// @return A const pointer to the beginning of the vector.
		const_pointer begin() const {
			return vecData;
		}
		/// @brief Gets the end of the vector.
		/// @return A pointer to the end of the vector.
		pointer end() {
			return vecData + vecSize;
		}
		/// @brief Gets the end of the vector.
		/// @return A const pointer to the end of the vector.
		const_pointer end() const {
			return vecData + vecSize;
		}
		/// @brief Gets the const beginning of the vector.
		/// @return A const pointer to the beginning of the vector.
		const_pointer cbegin() const {
			return vecData;
		}
		/// @brief Gets the const end of the vector.
		/// @return A const pointer to the end of the vector.
		const_pointer cend() const {
			return vecData + vecSize;
		}

		/// @brief Gets the size of the vector.
		/// @return The size of the vector.
		size_type size() const {
			return vecSize;
		}
		/// @brief Gets the maximum possible size of the vector.
		/// @return The maximum possible size of the vector.
		size_type max_size() const {
			return SIZE_T_MAX / sizeof(value_type);
		}
		/// @brief Resizes the vector.
		/// @param n The new size of the vector.
		void resize(size_type newSize) {
			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(newSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Exit the function if the new size is the same as the current size
			if(vecSize == newSize)
				return;
			
			if(vecSize < newSize) {
				// Save the vector's old size
				size_type oldSize = vecSize;

				// Resize the vector
				vecSize = newSize;

				// Check if memory needs to be reallocated
				if(vecSize > vecCapacity) {
					// Set the capacity to the lowest power of 2 higher than the vector's size
					size_type newCapacity = 1;

					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((newCapacity << step) < vecSize)
							newCapacity <<= step;
					newCapacity <<= 1;
					
					// Allocate the new space in the vector
					reserve(newCapacity);
				}

				// Construct the new values
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData + oldSize; ptr != end; ++ptr)
					new(ptr) value_type();
			} else {
				// Destruct the old values
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData + newSize; ptr != end; ++ptr)
					ptr->~value_type();

				// Resize the vector
				vecSize = newSize;
			}
		}
		/// @brief Resizes the vector and fills the new space with the given element.
		/// @param newSize The new size of the vector.
		/// @param val THe elements to fill the new space with.
		void resize(size_type newSize, const_reference val) {
			// Assert that the vector's new size must be lower than the maximum possible size
			WFE_ASSERT(newSize < max_size(), "The vector's new size must be lower than the maximum possible size!");

			// Exit the function if the new size is the same as the current size
			if(vecSize == newSize)
				return;
			
			if(vecSize < newSize) {
				// Save the vector's old size
				size_type oldSize = vecSize;

				// Resize the vector
				vecSize = newSize;

				// Check if memory needs to be reallocated
				if(vecSize > vecCapacity) {
					// Set the capacity to the lowest power of 2 higher than the vector's size
					size_type newCapacity = 1;

					for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
						if((newCapacity << step) < vecSize)
							newCapacity <<= step;
					newCapacity <<= 1;
					
					// Allocate the new space in the vector
					reserve(newCapacity);
				}

				// Copy the new values
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData + oldSize; ptr != end; ++ptr)
					new(ptr) value_type(val);
			} else {
				// Destruct the old values
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData + newSize; ptr != end; ++ptr)
					ptr->~value_type();

				// Resize the vector
				vecSize = newSize;
			}
		}
		/// @brief Gets the capacity of the vector.
		/// @return The vector's capacity.
		size_type capacity() const {
			return vecCapacity;
		}
		/// @brief Tests if the vector is empty.
		/// @return True if the vector is empty, otherwise false.
		bool8_t empty() const {
			return !vecSize;
		}
		/// @brief Changes the vector's capacity.
		/// @param newCapacity The new capacity of the vector.
		void reserve(size_type newCapacity = 0) {
			// Make the capacity higher than or equal to the vector's size
			newCapacity = (newCapacity < vecSize) ? vecSize : newCapacity;

			if(vecData) {
				// Reallocate the vector's data
				vecData = (pointer)ReallocMemory(vecData, newCapacity * sizeof(value_type));
			} else {
				// Allocate the vector's data
				vecData = (pointer)AllocMemory(newCapacity * sizeof(value_type));
			}

			// Check if the memory was allocated correctly
			if(!vecData)
				throw BadAllocException("Failed to allocate vector data!");
			
			// Set the vector's new capacity
			vecCapacity = newCapacity;
		}
		/// @brief Reduces the vector's capacity to fit its size.
		void shrink_to_fit() {
			// Save as calling resize with the capacity set to 0
			reserve(0);
		}

		/// @brief Destroys the vector.
		~vector() {
			// Destruct every value in the vector (if they exist)
			if(vecData) {
				pointer end = vecData + vecSize;
				for(pointer ptr = vecData; ptr != end; ++ptr)
					ptr->~value_type();
				
				// Free the vector's data
				FreeMemory(vecData);
			}
		}
	private:
		size_type vecSize = 0;
		size_type vecCapacity = 0;
		pointer vecData = nullptr;
	};

	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if both vectors are equal, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator==(const vector<T>& vec1, const vector<T>& vec2) {
		// Exit the function if their sizes are different
		if(vec1.size() != vec2.size())
			return false;
		
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1; ++ptr1)
			if(!(*ptr1 == *ptr2++))
				return false;
		
		return true;
	}
	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the vectors are different, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator!=(const vector<T>& vec1, const vector<T>& vec2) {
		// Exit the function if their sizes are different
		if(vec1.size() != vec2.size())
			return true;
		
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1; ++ptr1)
			if(*ptr1 == *ptr2++)
				return false;
		
		return true;
	}
	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the first vector is smaller than the second, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator< (const vector<T>& vec1, const vector<T>& vec2) {
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* end2 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1 && ptr2 != end2; ++ptr1)
			if(*ptr1 < *ptr2++)
				return true;
		
		// Compare the vector's sizes
		return vec1.size() < vec2.size();
	}
	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the first vector is smaller than or equal to the second, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator<=(const vector<T>& vec1, const vector<T>& vec2) {
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* end2 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1 && ptr2 != end2; ++ptr1)
			if(*ptr2++ < *ptr1)
				return false;
		
		// Compare the vector's sizes
		return vec1.size() <= vec2.size();
	}
	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the first vector is bigger than the second, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator> (const vector<T>& vec1, const vector<T>& vec2) {
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* end2 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1 && ptr2 != end2; ++ptr1)
			if(*ptr2++ < *ptr1)
				return true;
		
		// Compare the vector's sizes
		return vec1.size() > vec2.size();
	}
	/// @brief Compares the two given vectors.
	/// @tparam T The type of the vector's value.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the first vector is bigger than or equal to the second, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator>=(const vector<T>& vec1, const vector<T>& vec2) {
		// Compare every element in both vectors
		const T* end1 = vec1.end();
		const T* end2 = vec1.end();
		const T* ptr2 = vec2.begin();

		for(const T* ptr1 = vec1.begin(); ptr1 != end1 && ptr2 != end2; ++ptr1)
			if(*ptr1 < *ptr2++)
				return false;
		
		// Compare the vector's sizes
		return vec1.size() >= vec2.size();
	}
}