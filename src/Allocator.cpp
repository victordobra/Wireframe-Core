#include "Memory.hpp"
#include "Platform.hpp"
#include "Debug.hpp"
#include <WireframeAllocator.h>

namespace wfe {
	// Allocation callbacks
	void* SystemAlloc(size_t size, void* pUserData) {
		return PlatformAllocateMemory(size);
	}
	void* SystemRealloc(void* pMemory, size_t newSize, void* pUserData) {
		return PlatformReallocateMemory(pMemory, newSize);
	}
	void SystemFree(void* pMemory, void* pUserData) {
		PlatformFreeMemory(pMemory);
	}
	
	void* wantedAllocMemory;
	void* WantedAlloc(size_t size, void* pUserData) {
		return wantedAllocMemory;
	}
	void* WantedRealloc(void* pMemory, size_t newSize, void* pUserData) {
		return wantedAllocMemory;
	}
	void WantedFree(void* pMemory, void* pUserData) { }

	// Allocation callbacks
	const WfaAllocationCallbacks SYSTEM_ALLOC_CALLBACKS {
		nullptr,
		SystemAlloc,
		SystemRealloc,
		SystemFree
	};
	const WfaAllocationCallbacks WANTED_ALLOC_CALLBACKS {
		nullptr,
		WantedAlloc,
		WantedRealloc,
		WantedFree
	};

	// Constants
	const size_t TINY_ARENA_BLOCK_SIZE = 16;
	const size_t SMALL_ARENA_BLOCK_SIZE = 64;
	const size_t LARGE_ARENA_BLOCK_SIZE = 4096;
	const size_t LARGE_ARENA_BLOCK_HEIGHT = 8;
	const size_t MAX_LARGE_ARENA_BLOCK_SIZE = LARGE_ARENA_BLOCK_SIZE << (LARGE_ARENA_BLOCK_HEIGHT - 1);
	const size_t MAX_LARGE_ARENA_COUNT = 1024;

	const uint64_t HASH_PRIME = 0x100000001b3;
	const uint64_t HASH_OFFSET_BASIS = 0xcbf29ce484222325;

	// Byte popcount table
	const uint8_t POPCOUNT_TABLE[256] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
	};

	// Internal structs
	struct LargeArena {
		Mutex mutex;
		void** tinyArenaHead;
		void** smallArenaHeads[64];
		uint64_t smallArenaHeadBitmask;
		WfaBuddyMemoryAllocator allocator;
	};

	// Internal variables
	size_t totalMemoryUsage[MEMORY_USAGE_COUNT];
	Mutex totalMemoryUsageMutex{};

	size_t largeArenaCount;
	size_t tinyArenaRequiredSize;
	size_t smallArenaRequiredSize;
	size_t largeArenaSize;

	size_t tinyArenaBlockCount;
	size_t smallArenaBlockCount;

	LargeArena largeArenas[MAX_LARGE_ARENA_COUNT];

	// Internal functions
	uint64_t HashThreadID(Thread::ThreadID id) {
		// Use FNV-la to hash the given thread ID
		uint64_t hash = HASH_OFFSET_BASIS;

		for(size_t i = 0; i != sizeof(uint64_t); ++i) {
			hash ^= id & 0xff;
			hash *= HASH_PRIME;
			id >>= 8;
		}

		return hash;
	}
	uint64_t Pow2BitIndex(uint64_t val) {
		// Decrement the value and return its pop count
		val -= 1;
		uint64_t popcount = POPCOUNT_TABLE[val & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 8) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 16) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 24) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 32) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 40) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 48) & 0xff];
		popcount += POPCOUNT_TABLE[(val >> 56) & 0xff];
		return POPCOUNT_TABLE[val & 0xff] + POPCOUNT_TABLE[(val >> 8) & 0xff] + POPCOUNT_TABLE[(val >> 16) & 0xff] + POPCOUNT_TABLE[(val >> 24) & 0xff] + POPCOUNT_TABLE[(val >> 32) & 0xff] + POPCOUNT_TABLE[(val >> 40) & 0xff] + POPCOUNT_TABLE[(val >> 48) & 0xff] + POPCOUNT_TABLE[(val >> 56) & 0xff];
	}

	// Public functions
	void CreateAllocator() {
		// Get the number of allocation arenas to be created
		largeArenaCount = 4 * GetProcessorCount();

		// Partially set all create infos for all allocator types
		WfaMemoryPoolAllocatorCreateInfo tinyArenaInfo;

		tinyArenaInfo.allocatorBlockSize = TINY_ARENA_BLOCK_SIZE;
		tinyArenaInfo.allocatorBlockCount = 1; // Placeholder; irrelevant for size calculation with current info
		tinyArenaInfo.allocatorIncludeMemory = WFA_FALSE;
		tinyArenaInfo.allocatorManageExistingMemory = WFA_TRUE;

		WfaBitmaskAllocatorCreateInfo smallArenaInfo;

		smallArenaInfo.allocatorBlockSize = SMALL_ARENA_BLOCK_SIZE;
		smallArenaInfo.allocatorBlockCount = 1; // Placeholder; irrelevant for size calculation with current info
		smallArenaInfo.allocatorIncludeMemory = WFA_FALSE;
		
		WfaBuddyMemoryAllocatorCreateInfo largeArenaInfo;

		largeArenaInfo.allocatorBlockSize = LARGE_ARENA_BLOCK_SIZE;
		largeArenaInfo.allocatorBlockHeight = LARGE_ARENA_BLOCK_HEIGHT;
		largeArenaInfo.allocatorIncludeMemory = WFA_TRUE;
		largeArenaInfo.ppOutMemory = nullptr;

		// Calculate the sizes for all arena types
		tinyArenaRequiredSize = wfaGetMemoryPoolRequiredMemorySize(&tinyArenaInfo) + sizeof(void**);
		smallArenaRequiredSize = wfaGetBitmaskAllocatorRequiredMemorySize(&smallArenaInfo) + sizeof(void**);
		largeArenaSize = wfaGetBuddyMemoryAllocatorRequiredMemorySize(&largeArenaInfo);

		// Calculate the required block counts for both tiny and small arenas
		tinyArenaBlockCount = (LARGE_ARENA_BLOCK_SIZE - tinyArenaRequiredSize) / TINY_ARENA_BLOCK_SIZE;
		smallArenaBlockCount = (LARGE_ARENA_BLOCK_SIZE - smallArenaRequiredSize) / SMALL_ARENA_BLOCK_SIZE;
		
		// Round up the tiny and small arena sizes to the nearest block size multiples
		tinyArenaRequiredSize = LARGE_ARENA_BLOCK_SIZE - tinyArenaBlockCount * TINY_ARENA_BLOCK_SIZE;
		smallArenaRequiredSize = LARGE_ARENA_BLOCK_SIZE - smallArenaBlockCount * SMALL_ARENA_BLOCK_SIZE;

		// Create every wanted large arena
		for(size_t i = 0; i != largeArenaCount; ++i) {
			// Set the head of the tiny arena list to nullptr
			largeArenas[i].tinyArenaHead = nullptr;

			// Set the head of every small arena list to nullptr
			for(size_t j = 0; j != 64; ++j)
				largeArenas[i].smallArenaHeads[j] = nullptr;
			largeArenas[i].smallArenaHeadBitmask = 0;

			// Create the large arena
			WfaResult result = wfaCreateBuddyMemoryAllocator(&largeArenaInfo, &SYSTEM_ALLOC_CALLBACKS, &(largeArenas[i].allocator));
			if(result != WFA_SUCCESS)
				WFE_LOG_FATAL("Failed to create large allocation arena! Error code: %s.", wfaResultToString(result))
		}

		// Loop through every other possible large arena, setting all default values
		for(size_t i = largeArenaCount; i != MAX_LARGE_ARENA_COUNT; ++i) {
			// Set the head of the tiny arena list to nullptr
			largeArenas[i].tinyArenaHead = nullptr;

			// Set the head of every small arena list to nullptr
			for(size_t j = 0; j != 64; ++j)
				largeArenas[i].smallArenaHeads[j] = nullptr;
			largeArenas[i].smallArenaHeadBitmask = 0;
		}
	}
	void DeleteAllocator() {
		// Destroy every large arena
		for(size_t i = 0; i != largeArenaCount; ++i)
			wfaDestroyBuddyMemoryAllocator(&SYSTEM_ALLOC_CALLBACKS, largeArenas[i].allocator);
	}

	void* malloc(size_t size, MemoryUsage memoryUsage) {
		// Create the allocator if it hasn't already been created
		if(!largeArenaCount)
			CreateAllocator();

		// Check if a valid size was given
		if(!size)
			return NULL;
		
		// Increment the memory usage for the given size
		totalMemoryUsageMutex.Lock();
		totalMemoryUsage[memoryUsage] += size;
		totalMemoryUsageMutex.Unlock();

		// Increase the size by size_t for storing the size
		size += sizeof(size_t);

		// Check if the requested memory block is too large
		if(size > MAX_LARGE_ARENA_BLOCK_SIZE) {
			// Allocate the memory using the platform callbacks, write the allocation's size and exit the function
			size_t* mem = (size_t*)PlatformAllocateMemory(size);
			if(!mem)
				return nullptr;

			*mem = size;
			return mem + 1;
		}
		
		// Increase the size by size_t for storing the large arena index
		size += sizeof(size_t);
		
		// Get the current block's arena based on its current thread
		size_t largeArenaIndex, allocIndex;
		uint64_t threadIDHash = HashThreadID(GetCurrentThreadID());

		// Set a large arena's create info
		WfaBuddyMemoryAllocatorCreateInfo largeArenaInfo;

		largeArenaInfo.allocatorBlockSize = LARGE_ARENA_BLOCK_SIZE;
		largeArenaInfo.allocatorBlockHeight = LARGE_ARENA_BLOCK_HEIGHT;
		largeArenaInfo.allocatorIncludeMemory = WFA_TRUE;
		largeArenaInfo.ppOutMemory = nullptr;

		// Check if the requested memory block is tiny
		if(size <= TINY_ARENA_BLOCK_SIZE) {
			// Try to find or allocate a tiny arena
			for(; largeArenaCount != MAX_LARGE_ARENA_COUNT; ++largeArenaCount) {
				// Get the current large arena index
				largeArenaIndex = (size_t)(threadIDHash % largeArenaCount);
			
				// Check if the current arena has a tiny arena
				if(largeArenas[largeArenaIndex].tinyArenaHead)
					break;

				// Check if the current large arena can hold a tiny arena
				largeArenas[largeArenaIndex].mutex.Lock();
				WfaResult result = wfaBuddyMemoryAlloc(largeArenas[largeArenaIndex].allocator, LARGE_ARENA_BLOCK_SIZE, &allocIndex);
				largeArenas[largeArenaIndex].mutex.Unlock();

				if(result == WFA_SUCCESS)
					break;
				if(result != WFA_ERROR_NO_ROOM_IN_ALLOCATOR)
					return nullptr;
				
				// Exit the function if no more large arenas can be created
				if(largeArenaCount == MAX_LARGE_ARENA_COUNT - 1)
					return nullptr;
				
				// Create a new arena
				result = wfaCreateBuddyMemoryAllocator(&largeArenaInfo, &SYSTEM_ALLOC_CALLBACKS, &(largeArenas[largeArenaIndex + 1].allocator));
				if(result != WFA_SUCCESS)
					return nullptr;
			}

			// Lock the current large arena's mutex
			largeArenas[largeArenaIndex].mutex.Lock();

			// Create a tiny arena if none exist
			void** tinyArena;
			void** tinyArenaHead = largeArenas[largeArenaIndex].tinyArenaHead;

			WfaMemoryPoolAllocator tinyArenaAllocator;
			void* managedMemory;
			if(!tinyArenaHead) {
				// Set the tiny arena pointer
				char_t* largeArenaIncludedMemory = (char_t*)wfaBuddyMemoryGetIncludedMemory(largeArenas[largeArenaIndex].allocator);
				tinyArena = (void**)(largeArenaIncludedMemory + allocIndex * LARGE_ARENA_BLOCK_SIZE);
				*tinyArena = nullptr;

				// Set the create info
				WfaMemoryPoolAllocatorCreateInfo createInfo;
				managedMemory = largeArenaIncludedMemory + allocIndex * LARGE_ARENA_BLOCK_SIZE + tinyArenaRequiredSize;

				createInfo.allocatorBlockSize = TINY_ARENA_BLOCK_SIZE;
				createInfo.allocatorBlockCount = tinyArenaBlockCount;
				createInfo.allocatorIncludeMemory = WFA_FALSE;
				createInfo.allocatorManageExistingMemory = WFA_TRUE;
				createInfo.ppMemory = &managedMemory;

				// Create the tiny arena
				wantedAllocMemory = tinyArena + 1;
				WfaResult result = wfaCreateMemoryPoolAllocator(&createInfo, &WANTED_ALLOC_CALLBACKS, &tinyArenaAllocator);
				if(result != WFA_SUCCESS)
					return nullptr;
				
				// Add the tiny arena to the free list
				*tinyArena = nullptr;
				largeArenas[largeArenaIndex].tinyArenaHead = tinyArena;
			} else {
				tinyArena = tinyArenaHead;
				tinyArenaAllocator = (WfaMemoryPoolAllocator)(tinyArena + 1);
				managedMemory = (char_t*)tinyArena + tinyArenaRequiredSize;
			}

			// Allocate a memory block in the current arena
			WfaResult result = wfaMemoryPoolAlloc(tinyArenaAllocator, &allocIndex);
			if(result != WFA_SUCCESS)
				return nullptr;
			
			// Check if the current tiny arena if full
			if(wfaMemoryPoolIsFull(tinyArenaAllocator)) {
				// Remove the memory pool from its free list
				largeArenas[largeArenaIndex].tinyArenaHead = (void**)*tinyArena;
			}
			
			// Unlock the current large arena's mutex
			largeArenas[largeArenaIndex].mutex.Unlock();

			// Calculate the addres of the allocated block
			size_t* allocatedAddres = (size_t*)((char_t*)managedMemory + allocIndex * TINY_ARENA_BLOCK_SIZE);

			// Write the large arena index and allocation size to the memory
			*allocatedAddres = largeArenaIndex;
			++allocatedAddres;
			*allocatedAddres = size;
			++allocatedAddres;

			return allocatedAddres;
		}
		// Check if the current block is small
		if(size <= SMALL_ARENA_BLOCK_SIZE * smallArenaBlockCount) {
			// Calculate the block count of the requested allocation
			size_t blockCount = (size + SMALL_ARENA_BLOCK_SIZE - 1) / SMALL_ARENA_BLOCK_SIZE;
			size_t smallArenaHeadIndex = 64;

			// Try to find or allocate small tiny arena
			for(; largeArenaCount != MAX_LARGE_ARENA_COUNT; ++largeArenaCount) {
				// Get the current large arena index
				largeArenaIndex = (size_t)(threadIDHash % largeArenaCount);
			
				// Check if the current arena has a small arena that can hold the requested block
				uint64_t smallArenaHeadBitmask = largeArenas[largeArenaIndex].smallArenaHeadBitmask;
				smallArenaHeadBitmask &= ~(((uint64_t)1 << blockCount) - 1);
				smallArenaHeadBitmask &= ~(smallArenaHeadBitmask - 1);
				if(smallArenaHeadBitmask) {
					smallArenaHeadIndex = (size_t)Pow2BitIndex(smallArenaHeadBitmask);
					break;
				}

				// Check if the current large arena can hold a tiny arena
				largeArenas[largeArenaIndex].mutex.Lock();
				WfaResult result = wfaBuddyMemoryAlloc(largeArenas[largeArenaIndex].allocator, LARGE_ARENA_BLOCK_SIZE, &allocIndex);
				largeArenas[largeArenaIndex].mutex.Unlock();

				if(result == WFA_SUCCESS)
					break;
				if(result != WFA_ERROR_NO_ROOM_IN_ALLOCATOR)
					return nullptr;
				
				// Exit the function if no more large arenas can be created
				if(largeArenaCount == MAX_LARGE_ARENA_COUNT - 1)
					return nullptr;
				
				// Create a new arena
				result = wfaCreateBuddyMemoryAllocator(&largeArenaInfo, &SYSTEM_ALLOC_CALLBACKS, &(largeArenas[largeArenaIndex + 1].allocator));
				if(result != WFA_SUCCESS)
					return nullptr;
			}

			// Lock the current large arena's mutex
			largeArenas[largeArenaIndex].mutex.Lock();

			// Create a small arena if one wasn't found
			void** smallArena;

			WfaBitmaskAllocator smallArenaAllocator;
			void* managedMemory;
			if(smallArenaHeadIndex == 64) {
				// Set the small arena pointer
				char_t* largeArenaIncludedMemory = (char_t*)wfaBuddyMemoryGetIncludedMemory(largeArenas[largeArenaIndex].allocator);
				smallArena = (void**)(largeArenaIncludedMemory + allocIndex * LARGE_ARENA_BLOCK_SIZE);

				// Set the create info
				WfaBitmaskAllocatorCreateInfo createInfo;
				managedMemory = largeArenaIncludedMemory + allocIndex * LARGE_ARENA_BLOCK_SIZE + smallArenaRequiredSize;

				createInfo.allocatorBlockSize = SMALL_ARENA_BLOCK_SIZE;
				createInfo.allocatorBlockCount = smallArenaBlockCount;
				createInfo.allocatorIncludeMemory = WFA_FALSE;
				createInfo.ppOutMemory = nullptr;

				// Create the small arena
				wantedAllocMemory = smallArena + 1;
				WfaResult result = wfaCreateBitmaskAllocator(&createInfo, &WANTED_ALLOC_CALLBACKS, &smallArenaAllocator);
				if(result != WFA_SUCCESS)
					return nullptr;
				
				// Add the small arena to its free list
				*smallArena = nullptr;

				size_t headIndex = smallArenaBlockCount - blockCount;
				largeArenas[largeArenaIndex].smallArenaHeads[headIndex] = smallArena;
				largeArenas[largeArenaIndex].smallArenaHeadBitmask |= (uint64_t)1 << headIndex;

				// Allocate a memory block in the current arena
				result = wfaBitmaskAlloc(smallArenaAllocator, size, &allocIndex);
				if(result != WFA_SUCCESS)
					return nullptr;
			} else {
				// Set the working values to the values for the first small arena in the linked list
				smallArena = (void**)largeArenas[largeArenaIndex].smallArenaHeads[smallArenaHeadIndex];
				smallArenaAllocator = (WfaBitmaskAllocator)(smallArena + 1);
				managedMemory = (char_t*)smallArena + smallArenaRequiredSize;

				// Allocate a memory block in the current arena
				WfaResult result = wfaBitmaskAlloc(smallArenaAllocator, size, &allocIndex);
				if(result != WFA_SUCCESS)
					return nullptr;
				
				// Get the now longest span in the allocator
				size_t newHeadIndex = wfaBitmaskMemoryGetLongestFreeSpan(smallArenaAllocator);

				// Check if the current allocator needs to be moved to another linked list
				if(smallArenaHeadIndex != newHeadIndex) {
					// Remove the arena from its old linked list
					largeArenas[largeArenaIndex].smallArenaHeads[smallArenaHeadIndex] = (void**)*smallArena;
					largeArenas[largeArenaIndex].smallArenaHeadBitmask &= ~((uint64_t)(*smallArena == nullptr) << smallArenaHeadIndex);
					
					// Add the arena to its new linked list
					*smallArena = (void*)largeArenas[largeArenaIndex].smallArenaHeads[newHeadIndex];
					largeArenas[largeArenaIndex].smallArenaHeads[newHeadIndex] = smallArena;
					largeArenas[largeArenaIndex].smallArenaHeadBitmask |= (uint64_t)1 << newHeadIndex;
				}
			}

			// Unlock the current large arena's mutex
			largeArenas[largeArenaIndex].mutex.Unlock();

			// Calculate the addres of the allocated block
			size_t* allocatedAddres = (size_t*)((char_t*)managedMemory + allocIndex * SMALL_ARENA_BLOCK_SIZE);

			// Write the large arena index and allocation size to the memory
			*allocatedAddres = largeArenaIndex;
			++allocatedAddres;
			*allocatedAddres = size;
			++allocatedAddres;

			return allocatedAddres;
		}
		// The current block must be large; find a large arena that has enough room for it
		for(; largeArenaCount != MAX_LARGE_ARENA_COUNT; ++largeArenaCount) {
			// Get the current large arena index
			largeArenaIndex = (size_t)(threadIDHash % largeArenaCount);
			
			// Check if the current large arena can hold the requested memory block
			largeArenas[largeArenaIndex].mutex.Lock();
			WfaResult result = wfaBuddyMemoryAlloc(largeArenas[largeArenaIndex].allocator, size, &allocIndex);
			largeArenas[largeArenaIndex].mutex.Unlock();

			if(result == WFA_SUCCESS)
				break;
			if(result != WFA_ERROR_NO_ROOM_IN_ALLOCATOR)
				return nullptr;
			
			// Exit the function if no more large arenas can be created
			if(largeArenaCount == MAX_LARGE_ARENA_COUNT - 1)
				return nullptr;
			
			// Create a new arena
			result = wfaCreateBuddyMemoryAllocator(&largeArenaInfo, &SYSTEM_ALLOC_CALLBACKS, &(largeArenas[largeArenaIndex + 1].allocator));
			if(result != WFA_SUCCESS)
				return nullptr;
		}

		// Exit the function if the requested block couldn't be allocated
		if(largeArenaCount == MAX_LARGE_ARENA_COUNT)
			return nullptr;

		// Calculate the address of the allocated memory
		char_t* managedMemory = (char_t*)wfaBuddyMemoryGetIncludedMemory(largeArenas[largeArenaIndex].allocator);

		// Calculate the addres of the allocated block
		size_t* allocatedAddres = (size_t*)(managedMemory + allocIndex * LARGE_ARENA_BLOCK_SIZE);

		// Write the large arena index and allocation size to the memory
		*allocatedAddres = largeArenaIndex;
		++allocatedAddres;
		*allocatedAddres = size;
		++allocatedAddres;

		return allocatedAddres;
	}
	void* calloc(size_t nmemb, size_t size, MemoryUsage memoryUsage) {
		// Allocate the requested memory using malloc
		void* mem = malloc(nmemb * size, memoryUsage);
		if(!mem)
			return nullptr;

		// Zero out the memory using memset
		memset(mem, 0, nmemb * size);

		return mem;
	}
	void* realloc(void* ptr, size_t newSize, MemoryUsage memoryUsage) {
		// Load the mmeory's old size
		size_t oldSize = *((size_t*)ptr - 1);

		// Free and allocate the memory
		free(ptr, memoryUsage);
		void* newMem = malloc(newSize, memoryUsage);

		// Check if the allocation failed
		if(!newMem)
			return nullptr;
		
		// Move over the old memory
		size_t minSize = (oldSize < newSize) ? oldSize : newSize;
		memmove(newMem, ptr, minSize);

		return newMem;
	}
	void free(void* ptr, MemoryUsage memoryUsage) {
		// Load the allocated block's size
		size_t* mem = (size_t*)ptr - 1;
		size_t size = *mem;

		// Decrement the memory usage for the given size
		totalMemoryUsageMutex.Lock();
		totalMemoryUsage[memoryUsage] -= size;
		totalMemoryUsageMutex.Unlock();

		// Free the memory block externally if the block is too large
		if(size > MAX_LARGE_ARENA_BLOCK_SIZE) {
			PlatformFreeMemory(mem);
			return;
		}

		// Load the allocated block's large arena index
		--mem;
		size_t largeArenaIndex = *mem;
	
		// Load the current large arena's managed memory and handle
		WfaBuddyMemoryAllocator largeArenaAllocator = largeArenas[largeArenaIndex].allocator;
		char_t* managedMemory = (char_t*)wfaBuddyMemoryGetIncludedMemory(largeArenaAllocator);

		// Check if the given memory block is tiny
		if(size <= TINY_ARENA_BLOCK_SIZE) {
			// Get the block's tiny arena's index in its large arena
			size_t offset = (size_t)((char_t*)mem - managedMemory);
			size_t arenaIndex = offset / LARGE_ARENA_BLOCK_SIZE;

			// Load the arena's data
			void** tinyArena = (void**)(managedMemory + arenaIndex * LARGE_ARENA_BLOCK_SIZE);
			WfaMemoryPoolAllocator tinyArenaAllocator = (WfaMemoryPoolAllocator)(tinyArena + 1);

			// Check if the arena is currently full
			bool8_t arenaWasFull = wfaMemoryPoolIsFull(tinyArenaAllocator);

			// Free the given block from its tiny arena
			size_t blockIndex = (offset - arenaIndex * LARGE_ARENA_BLOCK_SIZE - tinyArenaRequiredSize) / TINY_ARENA_BLOCK_SIZE;

			WfaResult result = wfaMemoryPoolFree(tinyArenaAllocator, blockIndex);
			if(result != WFA_SUCCESS) {
				WFE_LOG_ERROR("Failed to free allocated memory! Error code: %s.", wfaResultToString(result))
				return;
			}

			// Check if the arena was full before freeing the given block
			if(arenaWasFull) {
				// Add the current tiny arena to the large arena's free linked list
				*tinyArena = (void*)largeArenas[largeArenaIndex].tinyArenaHead;
				largeArenas[largeArenaIndex].tinyArenaHead = tinyArena;
			}

			// Exit the function
			return;
		}
		// Check if teh given memory block is small
		if(size <= SMALL_ARENA_BLOCK_SIZE * smallArenaBlockCount) {
			// Get the block's small arena's index in its large arena
			size_t offset = (size_t)((char_t*)mem - managedMemory);
			size_t arenaIndex = offset / LARGE_ARENA_BLOCK_SIZE;

			// Load the arena's data
			void** smallArena = (void**)(managedMemory + arenaIndex * LARGE_ARENA_BLOCK_SIZE);
			WfaBitmaskAllocator smallArenaAllocator = (WfaBitmaskAllocator)(smallArena + 1);

			// Calculate the arena's current longest span length
			size_t oldLongestSpan = wfaBitmaskMemoryGetLongestFreeSpan(smallArenaAllocator);

			// Free the given block from its small arena
			size_t blockIndex = (offset - arenaIndex * LARGE_ARENA_BLOCK_SIZE - smallArenaRequiredSize) / SMALL_ARENA_BLOCK_SIZE;

			WfaResult result = wfaBitmaskFree(smallArenaAllocator, blockIndex, size);
			if(result != WFA_SUCCESS) {
				WFE_LOG_ERROR("Failed to free allocated memory! Error code: %s.", wfaResultToString(result))
				return;
			}

			// Calculate the arena's current longest span length
			size_t longestSpan = wfaBitmaskMemoryGetLongestFreeSpan(smallArenaAllocator);

			// Check if the length of the longest free span has changed
			if(oldLongestSpan != longestSpan) {
				// Remove the arena from its current free list
				void** smallArenaNext = (void**)*smallArena;
				largeArenas[largeArenaIndex].smallArenaHeads[oldLongestSpan] = smallArenaNext;
				largeArenas[largeArenaIndex].smallArenaHeadBitmask &= ~(((uint64_t)(smallArenaNext == nullptr)) << oldLongestSpan);

				// Add the arena to its new free list
				*smallArena = (void*)largeArenas[largeArenaIndex].smallArenaHeads[longestSpan];
				largeArenas[largeArenaIndex].smallArenaHeads[longestSpan] = smallArena;
				largeArenas[largeArenaIndex].smallArenaHeadBitmask |= (uint64_t)1 << longestSpan;
			}

			// Exit the function
			return;
		}

		// The memory block must be large; calculate the block's index
		size_t offset = (size_t)((char_t*)mem - managedMemory);
		size_t blockIndex = offset / LARGE_ARENA_BLOCK_SIZE;

		// Free the given block from its large arena
		WfaResult result = wfaBuddyMemoryFree(largeArenaAllocator, blockIndex, size);
		if(result != WFA_SUCCESS) {
			WFE_LOG_ERROR("Failed to free allocated memory! Error code: %s.", wfaResultToString(result))
			return;
		}
	}

	size_t* GetMemoryUsage() {
		return totalMemoryUsage;
	}
	size_t GetMemoryUsage(MemoryUsage memoryUsage) {
		return totalMemoryUsage[memoryUsage];
	}

}