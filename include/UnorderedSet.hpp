#pragma once

#include "Defines.hpp"
#include "Debug.hpp"
#include "Math.hpp"
#include "Memory.hpp"
#include "Pair.hpp"
#include <initializer_list>
#include <unordered_set>

namespace wfe {
	/// @brief Holds multiple buckets of unique elements
	/// @tparam T The type of the unordered set's values.
	template<class T>
	class unordered_set {    
	public:
		/// @brief The unordered set's key.
		typedef T key_type;
		/// @brief The unordered set's value.
		typedef T value_type;
		/// @brief A reference to the set's value.
		typedef T& reference;
		/// @brief A const reference to the set's value.
		typedef const T& const_reference;
		/// @brief A pointer to the set's value.
		typedef T* pointer;
		/// @brief A const pointer to the set's value.
		typedef const T* const_pointer;
	private:
		/// @brief The unordered set's node type.
		struct node_type {
			value_type val;
			node_type* next;
		};
	public:
		/// @brief An iterator to the set's value.
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

			friend unordered_set;
		};
		/// @brief A const iterator to the set's value.
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

			friend unordered_set;
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

			friend unordered_set;
		};
		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Constructs an empty unordered set.
		/// @param n The number of buckets.
		unordered_set(size_type n = 16) : usetBucketCount(n), usetBuckets((node_type**)malloc(usetBucketCount * sizeof(node_type**), MEMORY_USAGE_ARRAY)), usetSize(0), usetCapacity(0), usetData(nullptr) {
			// Check if the bucket list was allocated correctly
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Set all the bucket pointers to 0
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type**));
		}
		/// @brief Copies the given unordered set.
		/// @param other The unordered set to copy.
		unordered_set(const unordered_set& other) : usetBucketCount(other.usetBucketCount), usetBuckets((node_type**)malloc(usetBucketCount * sizeof(node_type**), MEMORY_USAGE_ARRAY)), usetSize(other.usetSize), usetCapacity(other.usetCapacity), usetData((node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY)), usetMaxLoadFactor(other.usetMaxLoadFactor) {
			// Check if the bucket list was allocated correctly
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Check if the data was allocated correctly
			if(!usetData)
				WFE_LOG_FATAL("Failed to allocate unordered set data!")
			
			// Copy every bucket relative to this set
			for(size_type i = 0; i != usetBucketCount; ++i) {
				// Set the bucket to nullptr if the other bucket is also nullptr
				if(!other.usetBuckets[i]) {
					usetBuckets[i] = nullptr;
					continue;
				}

				// Get the bucket's index
				size_type index = other.usetBuckets[i] - other.usetData;

				// Set the bucket based on the index
				usetBuckets[i] = usetData + index;
			}

			// Copy every node relative to this set
			for(size_type i = 0; i != usetSize; ++i) {
				// Copy the node's value
				new(&(usetData[i].val)) value_type(other.usetData[i].val);

				// Set the next pointer to nullptr if the other next pointer is also nullptr
				if(!other.usetData[i].next) {
					usetData[i].next = nullptr;
					continue;
				}

				// Get the next pointer's index
				size_type index = other.usetData[i].next - other.usetData;

				// Set the next pointer based on the index
				usetData[i].next = usetData + index;
			}
		}
		/// @brief Moves the given unordered set's contents.
		/// @param other The unordered set to move from.
		unordered_set(unordered_set&& other) noexcept : usetBucketCount(other.usetBucketCount), usetBuckets(other.usetBuckets), usetSize(other.usetSize), usetCapacity(other.usetCapacity), usetData(other.usetData), usetMaxLoadFactor(other.usetMaxLoadFactor) {
			// Unlink all data from the other unordered set
			other.usetBuckets = nullptr;
			other.usetData = nullptr;
		}
		/// @brief Constructs an unordered set with all the values in the given list.
		/// @param list The list to copy from.
		/// @param n The number of buckets.
		unordered_set(std::initializer_list<value_type> list, size_type n = 16) : usetBucketCount(n), usetBuckets((node_type**)malloc(usetBucketCount * sizeof(node_type**) , MEMORY_USAGE_ARRAY)), usetSize(list.size()), usetCapacity(1), usetData(nullptr), usetMaxLoadFactor(1.f) {
			// Check if the bucket list was allocated correctly
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Set all bucket pointer to nullptr
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type**));

			// Set the capacity to the lowest power of 2 higher than the size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((usetCapacity << step) < list.size())
					usetCapacity <<= step;
			usetCapacity <<= 1;

			// Allocate the unordered set's data
			usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			if(!usetData)
				WFE_LOG_FATAL("Failed to allocate unordered set data!")
			
			// Insert every value from the list
			for(size_type i = 0; i != usetSize; ++i) {
				// Construct the node's value
				new(&(usetData[i].val)) value_type(list.begin()[i]);

				// Get the bucket for the value
				size_type valBucket = bucket(usetData[i].val);

				// Set the node's next pointer
				usetData[i].next = usetBuckets[valBucket];

				// Set the bucket's pointer
				usetBuckets[valBucket] = usetData + i;
			}
		}
		/// @brief Copies the given std library unordered set.
		/// @param other The std library unordered set to copy from.
		unordered_set(const std::unordered_set<value_type>& other) : usetBucketCount(other.bucket_count()), usetBuckets((node_type**)malloc(usetBucketCount * sizeof(node_type**), MEMORY_USAGE_ARRAY)), usetSize(other.size()), usetCapacity(1), usetData(nullptr), usetMaxLoadFactor(other.max_load_factor()) {
			// Check if the bucket list was allocated correctly
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Set all bucket pointer to nullptr
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type**));

			// Set the capacity to the lowest power of 2 higher than the size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((usetCapacity << step) < other.size())
					usetCapacity <<= step;
			usetCapacity <<= 1;

			// Allocate the unordered set's data
			usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			if(!usetData)
				WFE_LOG_FATAL("Failed to allocate unordered set data!")
			
			// Insert every value from the other set
			auto otherIter = other.begin();

			for(size_type i = 0; i != usetSize; ++i) {
				// Construct the node's value
				new(&(usetData[i].val)) value_type(*otherIter++);

				// Get the bucket for the value
				size_type valBucket = bucket(usetData[i].val);

				// Set the node's next pointer
				usetData[i].next = usetBuckets[valBucket];

				// Set the bucket's pointer
				usetBuckets[valBucket] = usetData + i;
			}
		}

		/// @brief Copies the given unordered set's contents into this unordered set.
		/// @param other The unordered set to copy from.
		/// @return A reference to this unordered set.
		unordered_set& operator=(const unordered_set& other) {
			// Exit the function if the given unordered set is the same as this unordered set
			if(&other == this)
				return *this;
			
			// Delete this unordered set's old contents
			if(usetData) {
				// Destruct every value
				node_type* end = usetData + usetSize;
				for(node_type* ptr = usetData; ptr != end; ++ptr)
					ptr->~node_type();
				
				// Free the unordered set's data
				free(usetData, usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);

				// Free the unordered set's buckets
				free(usetBuckets, usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered set's new values
			usetBucketCount = other.usetBucketCount;
			usetBuckets = (node_type**)malloc(usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			usetSize = other.usetSize;
			usetCapacity = other.usetCapacity;
			usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			usetMaxLoadFactor = other.usetMaxLoadFactor;

			// Check if the bucket list was allocated correctly
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Check if the data was allocated correctly
			if(!usetData)
				WFE_LOG_FATAL("Failed to allocate unordered set data!")

			// Copy every bucket relative to this set
			for(size_type i = 0; i != usetBucketCount; ++i) {
				// Set the bucket to nullptr if the other bucket is also nullptr
				if(!other.usetBuckets[i]) {
					usetBuckets[i] = nullptr;
					continue;
				}

				// Get the bucket's index
				size_type index = other.usetBuckets[i] - other.usetData;

				// Set the bucket based on the index
				usetBuckets[i] = usetData + index;
			}

			// Copy every node relative to this set
			for(size_type i = 0; i != usetSize; ++i) {
				// Copy the node's value
				new(&(usetData[i].val)) value_type(other.usetData[i].val);

				// Set the next pointer to nullptr if the other next pointer is also nullptr
				if(!other.usetData[i].next) {
					usetData[i].next = nullptr;
					continue;
				}

				// Get the next pointer's index
				size_type index = other.usetData[i].next - other.usetData;

				// Set the next pointer based on the index
				usetData[i].next = usetData + index;
			}

			return *this;
		}
		/// @brief Moves the given unordered set's contents into this unordered set.
		/// @param other The unordered set to move from.
		/// @return A reference to this unordered set.
		unordered_set& operator=(unordered_set&& other) {
			// Delete this unordered set's old contents
			if(usetData) {
				// Destruct every value
				node_type* end = usetData + usetSize;
				for(node_type* ptr = usetData; ptr != end; ++ptr)
					ptr->~node_type();
				
				// Free the unordered set's data
				free(usetData, usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);

				// Free the unordered set's buckets
				free(usetBuckets, usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered set's new values
			usetBucketCount = other.usetBucketCount;
			usetBuckets = other.usetBuckets;
			usetSize = other.usetSize;
			usetCapacity = other.usetCapacity;
			usetData = other.usetData;
			usetMaxLoadFactor = other.usetMaxLoadFactor;

			// Unlink all data from the other unordered set
			other.usetBuckets = nullptr;
			other.usetData = nullptr;

			return *this;
		}
		/// @brief Copies the given list's values into this unordered set.
		/// @param list The list to copy from.
		/// @return A reference to this set.
		unordered_set& operator=(std::initializer_list<value_type> list) {
			// Delete this unordered set's old contents
			if(usetData) {
				// Destruct every value
				node_type* end = usetData + usetSize;
				for(node_type* ptr = usetData; ptr != end; ++ptr)
					ptr->~node_type();
				
				// Free the unordered set's data
				free(usetData, usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);

				// Free the unordered set's buckets
				free(usetBuckets, usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			}

			// Set the unordered set's new values
			usetSize = list.size();
			usetCapacity = 1;
			usetMaxLoadFactor = 1;

			// Set the bucket count and the capacity to the lowest power of 2 higher than or equal to the size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((usetCapacity << step) < usetSize)
					usetCapacity <<= step;
			usetCapacity <<= 1;
			usetBucketCount = usetCapacity;

			// Allocate the unordered set's buckets
			usetBuckets = (node_type**)malloc(usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			if(!usetBuckets)
				WFE_LOG_FATAL("Failed to allocate unordered set buckets!")
			
			// Set all bucket pointer to nullptr
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type**));

			// Allocate the unordered set's data
			usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			if(!usetData)
				WFE_LOG_FATAL("Failed to allocate unordered set data!")

            // Insert every value from the list
            for(size_type i = 0; i != usetSize; ++i) {
                // Construct the node's value
                new(&(usetData[i].value)) value_type(list.begin()[i]);

                // Get the bucket for the value
                size_type valBucket = bucket(usetData[i].value);

                // Set the node's next pointer
                usetData[i].next = usetBuckets[valBucket];

                // Set the bucket's pointer
                usetBuckets[valBucket] = usetData + i;
            }
			
			return *this;
		}

		/// @brief Converts the unordered set to an std library unordered set.
		operator std::unordered_set<value_type>() const {
			// Set the std library unordered set's bucket count and max load factor
			std::unordered_set<value_type> uset(usetBucketCount);

			uset.max_load_factor(usetMaxLoadFactor);
			
			// Insert every value from this set into the std lebrary unordered set
			node_type* end = usetData + usetSize;
			for(node_type* ptr = usetData; ptr != end; ++ptr)
				uset.insert(*ptr);
			
			return uset;
		}

		/// @brief Finds the wanted value in the unordered set.
		/// @param val The value to find.
		/// @return An iterator to the wanted value if it exists, otherwise unordered_set::end().
		iterator find(const_reference val) {
			// Get the bucket for the value
			size_type valBucket = bucket(val);

			node_type* elem = usetBuckets[valBucket];

			// Look for the given value in the bucket
			while(elem) {
				// Compare the values
				if(val == elem->val)
					return elem;
				
				// Move on to the next element
				elem = elem->next;
			}

			return end();
		}
		/// @brief Finds the wanted value.
		/// @param val The value to find.
		/// @return A const iterator to the wanted value.
		const_iterator find(const_reference val) const {
			// Get the bucket for the value
			size_type valBucket = bucket(val);

			const node_type* elem = usetBuckets[valBucket];

			// Look for the given value in the bucket
			while(elem) {
				// Compare the values
				if(val == elem->val)
					return elem;
				
				// Move on to the next element
				elem = elem->next;
			}

			return end();
		}
		/// @brief Gets the number of times the given value appears in the unordered set.
		/// @param val The value to look for.
		/// @return The number of times the given value appears in the unordered set.
		size_type count(const_reference val) const {
			// Get the bucket for the value
			size_type valBucket = bucket(val);

			const node_type* elem = usetBuckets[valBucket];

			// Look for the given value in the bucket
			while(elem) {
				// Compare the values
				if(val == elem->val)
					return 1;
				
				// Move on to the next element
				elem = elem->next;
			}

			return 0;
		}
		/// @brief Finds the range that includes all elements in the unordered set that are equal to the given value.
		/// @param val The value to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<iterator, iterator> equal_range(const_reference val) {
			// Get the bucket for the value
			size_type valBucket = bucket(val);

			node_type* elem = usetBuckets[valBucket];

			// Look for the given value in the bucket
			while(elem) {
				// Compare the values
				if(val == elem->val)
					return { elem, elem + 1 };
				
				// Move on to the next element
				elem = elem->next;
			}

			return { end(), end() };
		}
		/// @brief Finds the range that includes all elements in the unordered set that are equal to the given value.
		/// @param val The value to compare with.
		/// @return A pair containing the lower and upper bounds of the wanted range.
		pair<const_iterator, const_iterator> equal_range(const_reference val) const {
			// Get the bucket for the value
			size_type valBucket = bucket(val);

			const node_type* elem = usetBuckets[valBucket];

			// Look for the given value in the bucket
			while(elem) {
				// Compare the values
				if(val == elem->val)
					return { elem, elem + 1 };
				
				// Move on to the next element
				elem = elem->next;
			}

			return { end(), end() };
		}

		/// @brief Emplaces a value in the unordered set.
		/// @tparam ...Args The arg types to create the value with.
		/// @param ...args The args to create the value with.
		/// @return A pair containing an iterator to the emplaced value or an identical value that was already in the unordered set and a bool that is set to true if a new element was emplaced, otherwise false.
		template<class... Args>
		pair<iterator, bool8_t> emplace(Args&&... args) {
			// Construct the value using the given args
			value_type val(args...);

			// Insert the constructed value
			return insert(val);
		}
		/// @brief Emplaces a value in the unordered set using the given hint.
		/// @tparam ...Args The arg types to create the value with.
		/// @param pos A hint for the new value's position that will be used to optimize the insertion.
		/// @param ...args The args to create the value with.
		/// @return An iterator to the emplaced value or an identical value that was already in the unordered set.
		template<class... Args>
		iterator emplace_hint(const_iterator pos, Args&&... args) {
			// Construct the value using the given args
			value_type val(args...);

			// Insert the constructed value using the given hint
			return insert(pos, val);
		}
		/// @brief Inserts the given value in the unordered set.
		/// @param val The value to insert.
		/// @return A pair containing an iterator to the inserted value or an identical value that was already in the unordered set and a bool that is set to true if a new element was inserted, otherwise false.
		pair<iterator, bool8_t> insert(const_reference val) {
			// Get the given value's bucket
			size_type valBucket = bucket(val);

			node_type** elem = usetBuckets + valBucket;

			// Look for the given value in the bucket
			while(*elem) {
				// Compare the values
				if(val == (*elem)->val)
					return { *elem, false };
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// Increment the unordered set's size
			++usetSize;

			// Check if memory needs to be reallocated
			if(usetSize > usetCapacity) {
				// Save the old capacity
				size_type oldCapacity = usetCapacity;

				// Set the unordered set's capacity to the lowest power of 2 higher than the set's size
				usetCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if(usetCapacity << step < usetSize)
						usetCapacity <<= step;
				usetCapacity <<= 1;

				// Reallocate the unordered set's data
				if(usetData)
					usetData = (node_type*)realloc(usetData, oldCapacity * sizeof(node_type), usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
				else
					usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!usetData)
					WFE_LOG_FATAL("Failed to allocate unordered set memory!")
			}

			// Check if a rehash is required
			if(usetSize > usetBucketCount * max_load_factor()) {
				// Set the unordered set's new bucket count to the lowest power of 2 for which the unordered set's load factor is lower than or equal to the max
				size_type newBucketCount = 1;
				for(uint32_t step = sizeof(size_type) << 2; step; step >>= 1)
					if((newBucketCount << step) * max_load_factor() < usetSize)
						newBucketCount <<= step;
				newBucketCount <<= 1;

				// Rehash the unordered set
				rehash(newBucketCount);
			}

			// Create the new value
			new(&(usetData[usetSize - 1].val)) value_type(val);

			usetData[usetSize - 1].next = nullptr;

			// Set the next pointer to the created value
			*elem = usetData + usetSize - 1;

			return { usetData + usetSize - 1, true };
		}
		/// @brief Inserts the given value in the unordered set.
		/// @param val The value to insert.
		/// @return A pair containing an iterator to the inserted value or an identical value that was already in the unordered set and a bool that is set to true if a new element was inserted, otherwise false.
		pair<iterator, bool8_t> insert(value_type&& val) {
			// Get the given value's bucket
			size_type valBucket = bucket(val);

			node_type** elem = usetBuckets + valBucket;

			// Look for the given value in the bucket
			while(*elem) {
				// Compare the values
				if(val == (*elem)->val)
					return { *elem, false };
				
				// Move on to the next value
				elem = &((*elem)->next);
			}

			// Increment the unordered set's size
			++usetSize;

			// Check if memory needs to be reallocated
			if(usetSize > usetCapacity) {
				// Save the old capacity
				size_type oldCapacity = usetCapacity;

				// Set the unordered set's capacity to the lowest power of 2 higher than the set's size
				usetCapacity = 1;
				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if(usetCapacity << step < usetSize)
						usetCapacity <<= step;
				usetCapacity <<= 1;

				// Reallocate the unordered set's data
				if(usetData)
					usetData = (node_type*)realloc(usetData, oldCapacity * sizeof(node_type), usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
				else
					usetData = (node_type*)malloc(usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
				
				// Check if the memory was allocated correctly
				if(!usetData)
					WFE_LOG_FATAL("Failed to allocate unordered set memory!")
			}

			// Check if a rehash is required
			if(usetSize > usetBucketCount * max_load_factor()) {
				// Set the unordered set's new bucket count to the lowest power of 2 for which the unordered set's load factor is lower than or equal to the max
				size_type newBucketCount = 1;
				for(uint32_t step = sizeof(size_type) << 2; step; step >>= 1)
					if((newBucketCount << step) * max_load_factor() < usetSize)
						newBucketCount <<= step;
				newBucketCount <<= 1;

				// Rehash the unordered set
				rehash(newBucketCount);
			}

			// Create the new value
			new(&(usetData[usetSize - 1].val)) value_type(val);

			usetData[usetSize - 1].next = nullptr;

			// Set the next pointer to the created value
			*elem = usetData + usetSize - 1;

			return { usetData + usetSize - 1, true };
		}
		/// @brief Inserts the given value in the unordered set using the given hint.
		/// @param pos A hint for the new value's position that will be used to optimize the insertion.
		/// @param val The value to insert.
		/// @return An iterator to the inserted value or an identical value that was already in the unordered set.
		iterator insert(const_iterator pos, const_reference val) {
			// Simply insert the value, as the hint cannot be used to optimize the insertion in any way
			return insert(val).first;
		}
		/// @brief Inserts the given value in the unordered set using the given hint.
		/// @param pos A hint for the new value's position that will be used to optimize the insertion.
		/// @param val The value to insert.
		/// @return An iterator to the inserted value or an identical value that was already in the unordered set.
		iterator insert(const_iterator pos, value_type&& val) {
			// Simply insert the value, as the hint cannot be used to optimize the insertion in any way
			return insert(val).first;
		}
		/// @brief Inserts the values from the given list.
		/// @param list The list to insert from.
		void insert(std::initializer_list<value_type> list) {
			// Insert every value from the list
			const_pointer end = list.end();
			for(const_pointer ptr = list.begin(); ptr != end; ++ptr)
				insert(*ptr);
		}
		/// @brief Erases the value at the given position from the unordered set.
		/// @param pos The position to erase from.
		/// @return An iterator to the space occupied by the erased value.
		iterator erase(const_iterator pos) {
			// Decrease the unordered set's size
			--usetSize;

			// Destruct the value at the given position
			node_type* posPtr = (node_type*)pos.node;

			size_type posBucket = bucket(posPtr->val);			

			posPtr->val.~value_type();

			// Find the previous value in the destructed value's bucket
			node_type** posElem = usetBuckets + posBucket;

			while(*posElem != posPtr)
				posElem = &((*posElem)->next);
			
			// Remove the destructed value from the linked list
			*posElem = posPtr->next;

			// Exit the function if the destructed value was the last value in the data
			node_type* lastPtr = usetData + usetSize;

			if(posPtr == lastPtr)
				return posPtr;

			// To keep the unordered set's data consistent, move the last value into the now empty space
			memcpy(posPtr, lastPtr, sizeof(node_type));

			// Find the previous value in the last value's bucket
			size_type lastBucket = bucket(posPtr->val);

			node_type** lastElem = usetBuckets + lastBucket;

			while(*lastElem != lastPtr)
				lastElem = &((*lastElem)->next);
			
			// Set the last value's new memory address
			*lastElem = posPtr;

			return posPtr;
		}
		/// @brief Erases the given value from the unordered set.
		/// @param val The value to erase.
		/// @return The number of erased values.
		size_type erase(const_reference val) {
			// Decrease the unordered set's size
			--usetSize;

			// Find the given value in the unordered set
			size_type valBucket = bucket(val);

			// Exit the function if the value's bucket is empty
			if(!usetBuckets[valBucket])
				return 0;
			
			// Find the given value in the unordered set
			node_type** valElem = usetBuckets + valBucket;
			bool8_t valFound = false;

			while(*valElem) {
				// Check if the value is at the current node
				if((*valElem)->val == val)
					break;
				
				// Move on to the next node
				valElem = &((*valElem)->next);
			}

			// Check if the value is in the unordered set
			if(!*valElem)
				return 0;
			
			// Destruct the value
			(*valElem)->val.~value_type();

			node_type* posPtr = *valElem;

			// Exit the function if the destructed value was the last value in the data
			node_type* lastPtr = usetData + usetSize;

			if(posPtr == lastPtr)
				return 1;

			// To keep the unordered set's data consistent, move the last value into the now empty space
			memcpy(posPtr, lastPtr, sizeof(node_type));

			// Find the previous value in the last value's bucket
			size_type lastBucket = bucket(posPtr->val);

			node_type** lastElem = usetBuckets + lastBucket;

			while(*lastElem != lastPtr)
				lastElem = &((*lastElem)->next);
			
			// Set the last value's new memory address
			*lastElem = posPtr;

			return 1;
		}
		/// @brief Erases the values in the given range from the unordered set.
		/// @param first An inerator to the first erased value.
		/// @param last An iterator to right after the last erased value.
		/// @return An iterator to the space occupied by the first erased value.
		iterator erase(const_iterator first, const_iterator last) {
			// Get the number of erased values
			size_type eraseCount = last.node - first.node;

			// Erase every value in the given range
			for(size_type i = 0; i != eraseCount; ++i)
				erase(first);
			
			return (node_type*)first.node;
		}
		/// @brief Clears the unordered set's contents.
		void clear() {
			// Destruct every value in the unordered set
			node_type* end = usetData + usetSize;
			for(node_type* ptr = usetData; ptr != end; ++ptr) {
				ptr->val.~value_type();
				ptr->next = nullptr;
			}

			// Set the unordered set's size to 0
			usetSize = 0;

			// Set every bucket pointer to nullptr
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type*));
		}
		/// @brief Swaps the two unordered set's contents.
		/// @param other The unordered set to swap with.
		void swap(unordered_set& other) {
			// Exit the function if the two unordered sets are the same
			if(&other == this)
				return;
			
			// Save this unordered set's values
			size_type auxBucketCount = usetBucketCount;
			node_type** auxBuckets = usetBuckets;
			size_type auxSize = usetSize;
			size_type auxCapacity = usetCapacity;
			node_type* auxData = usetData;
			float32_t auxMaxLoadFactor = usetMaxLoadFactor;

			// Set this unordered set's new values
			usetBucketCount = other.usetBucketCount;
			usetBuckets = other.usetBuckets;
			usetSize = other.usetSize;
			usetCapacity = other.usetCapacity;
			usetData = other.usetData;
			usetMaxLoadFactor = other.usetMaxLoadFactor;

			// Set the other unordered set's new values
			other.usetBucketCount = auxBucketCount;
			other.usetBuckets = auxBuckets;
			other.usetSize = auxSize;
			other.usetCapacity = auxCapacity;
			other.usetData = auxData;
			other.usetMaxLoadFactor = auxMaxLoadFactor;
		}

		/// @brief Returns an iterator to the beginning of the unordered set.
		/// @return An iterator to the beginning of the unordered set.
		iterator begin() {
			return usetData;
		}
		/// @brief Returns a const iterator to the beginning of the unordered set.
		/// @return A const iterator to the beginning of the unordered set.
		const_iterator begin() const {
			return usetData;
		}
		/// @brief Returns an iterator to the end of the unordered set.
		/// @return An iterator to the end of the unordered set.
		iterator end() {
			return usetData + usetSize;
		}
		/// @brief Returns a const iterator to the end of the unordered set.
		/// @return A const iterator to the end of the unordered set.
		const_iterator end() const {
			return usetData + usetSize;
		}
		/// @brief Returns a const iterator to the beginning of the unordered set.
		/// @return A const iterator to the beginning of the unordered set.
		const_iterator cbegin() const {
			return usetData;
		}
		/// @brief Returns a const iterator to the end of the unordered set.
		/// @return A const iterator to the end of the unordered set.
		const_iterator cend() const {
			return usetData + usetSize;
		}

		/// @brief Tests if the unordered set is empty.
		/// @return True if the unordered set is empty, otherwise false.
		bool8_t empty() const {
			return !usetSize;
		}
		/// @brief Gets the unordered set's size.
		/// @return The unordered set's size.
		size_type size() const {
			return usetSize;
		}
		/// @brief Gets the unordered set's maximum possible size.
		/// @return The unordered set's maximum possible size.
		size_type max_size() const {
			return SIZE_T_MAX / sizeof(node_type);
		}

		/// @brief Gets the number of buckets in the unordered set.
		/// @return The number of buckets in the unordered set.
		size_type bucket_count() const {
			return usetBucketCount;
		}
		/// @brief Gets the maximum possible number of buckets in the unordered set.
		/// @return The maximum possible number of buckets in the unordered set.
		size_type max_bucket_count() const {
			return SIZE_T_MAX / sizeof(node_type*);
		}
		/// @brief Gets the number of values in the given bucket.
		/// @param n The bucket's index.
		/// @return The number of values in the given bucket.
		size_type bucket_size(size_type n) const {
			// Loop through the given bucket's linked list, counting the number of values
			node_type* ptr = usetBuckets + n;
			size_type count = 0;

			while(ptr) {
				// Increment the count
				++count;

				// Move on to the next element
				ptr = ptr->next;
			}

			return count;
		}
		/// @brief Gets the bucket that the given value is in.
		/// @param value The value to look for.
		/// @return The bucket's index.
		size_type bucket(const_reference value) const {
			// Get the given value's hash
			uint64_t hash = Hash(value);

			return hash % usetBucketCount;
		}

		/// @brief Gets the unordered set's load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @return The unordered set's load factor.
		float32_t load_factor() const {
			return (float32_t)usetSize / (float32_t)usetBucketCount;
		}
		/// @brief Gets the unordered set's max load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @return The unordered set's max load factor.
		float32_t max_load_factor() const {
			return usetMaxLoadFactor;
		}
		/// @brief Sets the unordered set's max load factor, which is equal to the ratio between the total number of values and the number of buckets.
		/// @param z The unordered set's new max load factor.
		void max_load_factor(float32_t z) const {
			usetMaxLoadFactor = z;
		}
		/// @brief Sets the number of buckets in the unordered set.
		/// @param n The new bucket count.
		void rehash(size_type n) {
			// Save the old bucket count
			size_type oldBucketCount = usetBucketCount;

			// Set the new bucket count
			usetBucketCount = n;

			// Reallocate the unordered set's buckets
			if(usetBuckets)
				usetBuckets = (node_type**)realloc(usetBuckets, oldBucketCount * sizeof(node_type*), usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			else
				usetBuckets = (node_type**)malloc(usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			
			// Set every pointer in the bucket list to nullptr
			memset(usetBuckets, 0, usetBucketCount * sizeof(node_type*));

			// Loop through the unordered set's data backwards
			node_type* end = usetData - 1;
			for(node_type* ptr = usetData + usetSize - 1; ptr != end; --ptr) {
				// Get the current value's bucket
				size_type valBucket = bucket(ptr->val);

				// Set the current node's next pointer
				ptr->next = usetBuckets[valBucket];

				// Set the new linked list head
				usetBuckets[valBucket] = ptr;
			}
		}
		/// @brief Sets the number of buckets in the unordered set to the most appropriate to contain the given number of values.
		/// @param n The requested nuber of values.
		void reserve(size_type n) {
			// Set the unordered set's new bucket count to the lowest power of 2 for which the unordered set's load factor is lower than or equal to the max
			size_type newBucketCount = 1;
			for(uint32_t step = sizeof(size_type) << 2; step; step >>= 1)
				if((newBucketCount << step) * max_load_factor() < usetSize)
					newBucketCount <<= step;
			newBucketCount <<= 1;

			// Rehash the unordered set
			rehash(newBucketCount);
		}

		/// @brief Deletes the unordered set.
		~unordered_set() {
			if(usetData) {
				// Destruct every value in the unordered set
				node_type* end = usetData + usetSize;
				for(node_type* ptr = usetData; ptr != end; ++ptr) {
					ptr->val.~value_type();
					ptr->next = nullptr;
				}

				// Free the unordered set's data
				free(usetData, usetCapacity * sizeof(node_type), MEMORY_USAGE_ARRAY);
			}
			if(usetBuckets) {
				// Free the unordered set's bucket list
				free(usetBuckets, usetBucketCount * sizeof(node_type*), MEMORY_USAGE_ARRAY);
			}
		}
	private:
		size_type usetBucketCount;
		node_type** usetBuckets;
		size_type usetSize;
		size_type usetCapacity;
		node_type* usetData;
		float32_t usetMaxLoadFactor = 1.f;
	};

	/// @brief Compares the two given unordered sets.
	/// @tparam T The type of the two unordered set's values.
	/// @param uset1 The first unordered set to compate.
	/// @param uset2 The second unordered set to compate.
	/// @return True if the two unordered sets are equal, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator==(const unordered_set<T>& uset1, const unordered_set<T>& uset2) {
		// Exit the function if the unordered sets have different sizes
		if(uset1.size() != uset2.size())
			return false;

		// Search for every value in the first set in the second set
		for(const auto& val : uset1)
			if(!uset2.count(val))
				return false;
		
		return true;
	}
	/// @brief Compares the two given unordered sets.
	/// @tparam T The type of the two unordered set's values.
	/// @param uset1 The first unordered set to compate.
	/// @param uset2 The second unordered set to compate.
	/// @return True if the two unordered sets are different, otherwise false.
	template<class T>
	WFE_INLINE bool8_t operator!=(const unordered_set<T>& uset1, const unordered_set<T>& uset2) {
		// Exit the function if the unordered sets have different sizes
		if(uset1.size() != uset2.size())
			return true;

		// Search for every value in the first set in the second set
		for(const auto& val : uset1)
			if(uset2.count(val))
				return false;
		
		return true;
	}
}