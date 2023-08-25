#include "Memory.hpp"
#include "Platform.hpp"
#include "Debug.hpp"
#include <atomic>
#include <unistd.h>

namespace wfe {
	// Constants
	const size_t ALLOC_ARENA_SIZE = 1048576;
	const size_t SMALL_ARENA_MIN_SIZE_LOG2 = 4;
	const size_t SMALL_ARENA_SIZE_COUNT = 32;
	const size_t LARGE_ARENA_SIZE_COUNT = 10;
	const size_t SMALL_ARENA_MIN_SIZE = (size_t)1 << SMALL_ARENA_MIN_SIZE_LOG2;
	const size_t SMALL_ARENA_MAX_SIZE = SMALL_ARENA_MIN_SIZE * SMALL_ARENA_SIZE_COUNT;
	const size_t LARGE_ARENA_MAX_SIZE = SMALL_ARENA_MAX_SIZE << LARGE_ARENA_SIZE_COUNT;

	// Structs
	struct SmallFreeListItem {
		SmallFreeListItem* next;
	};

	struct ArenaListItem {
		ArenaListItem* prev;
		ArenaListItem* next;
	};
	struct ArenaInfo {
		ArenaListItem listItem;
		atomic_size_t arenaMutex;

		std::atomic<SmallFreeListItem*> smallFreeLists[SMALL_ARENA_SIZE_COUNT];

		size_t stackTop;
	};
	struct AllocMetadata {
		size_t size;
		size_t reallocCount;
	};

	// Internal variables
	volatile atomic_size_t totalMemoryUsage[MEMORY_USAGE_COUNT];

	size_t pageSize = 0;
	size_t arenaCount = 0;
	size_t maxArenaCount;
	void* arenaVirtualMemoryStart;
	
	ArenaListItem arenaListHead;

	// Internal functions
	static ArenaInfo* CreateAllocArena(size_t initialAllocSize) {
		// Get the memory address of the arena at the current index
		ArenaInfo* arenaMemory = (ArenaInfo*)((char_t*)arenaVirtualMemoryStart + arenaCount * ALLOC_ARENA_SIZE);

		// Commit the current arena's memory
		bool8_t result = CommitVirtualAddressSpace(arenaMemory, ALLOC_ARENA_SIZE);
		if(!result) {
#ifdef WFE_BUILD_MODE_DEBUG
			// Log the error
			WFE_LOG_ERROR("Failed to commit virtual address space required for alloc arena!")
#endif
			return nullptr;
		}

		// Set the arena's info
		arenaMemory->listItem.prev = &arenaListHead;
		arenaMemory->listItem.next = arenaListHead.next;

		arenaMemory->arenaMutex = 0;

		for(size_t i = 0; i != SMALL_ARENA_SIZE_COUNT; ++i)
			arenaMemory->smallFreeLists[i] = nullptr;

		arenaMemory->stackTop = sizeof(ArenaInfo) + initialAllocSize;

		// Increment the arena count
		++arenaCount;

		return arenaMemory;
	}

	// Public functions
	void CreateAllocator() {
		// Retrieve the required performance info
		PerformanceInfo performanceInfo;
		GetPerformanceInformation(performanceInfo);

		// Save the memory page size
		pageSize = performanceInfo.pageSize;

		// Reserve a quarter of the virtual address space for arenas
		maxArenaCount = (performanceInfo.availableVirtualMemorySize >> 2) / ALLOC_ARENA_SIZE;
		
		arenaVirtualMemoryStart = ReserveVirtualAddressSpace(nullptr, maxArenaCount * ALLOC_ARENA_SIZE);
		if(!arenaVirtualMemoryStart)
			WFE_LOG_FATAL("Failed to reserve required virtual address space for allocator!")
		
		// Set the arena list head's initial values
		arenaListHead.prev = &arenaListHead;
		arenaListHead.next = &arenaListHead;
	}
	void DeleteAllocator() {
		// Free all reserved virtual address space for arenas
		FreeVirtualAddressSpace(arenaVirtualMemoryStart);
	}

	void* malloc(size_t size, MemoryUsage memoryUsage) {
		// Check if the allocator was initialized by checking if the page size was set
		if(!pageSize)
			CreateAllocator();
		
		// Increase the given size by the size of the allocation's metadata
		size += sizeof(AllocMetadata);

		// Check if the given allocation is small
		if(size <= SMALL_ARENA_MAX_SIZE) {
			// Round the size up to the nearest multiple of the small arena min size and calculate the index of the block's small arena free list
			size_t freeListIndex = (size + SMALL_ARENA_MIN_SIZE - 1) >> SMALL_ARENA_MIN_SIZE_LOG2;
			size = freeListIndex * SMALL_ARENA_MIN_SIZE;
			--freeListIndex;

			// Retrieve the first free small arena and remove it from its free list
			ArenaListItem* currentArenaListItem = arenaListHead.next;
			AllocMetadata* allocatedMemory = nullptr;
			while(currentArenaListItem != &arenaListHead) {
				// Get a pointer to the current arena's info
				ArenaInfo* currentArena = (ArenaInfo*)currentArenaListItem;

				// Try to use a free list block with the same size as the requested block
				std::atomic<SmallFreeListItem*>* freeList = currentArena->smallFreeLists + freeListIndex;
				SmallFreeListItem* originalFreeList = *freeList;
				bool8_t succeeded = false;

				while(!succeeded) {
					// Check if the list is now empty
					if(!originalFreeList)
						break;
					
					// Try to set the new free list head
					SmallFreeListItem* newFreeList = originalFreeList->next;
					succeeded = freeList->compare_exchange_weak(originalFreeList, newFreeList);
				}
				// Check if a free list block was found
				if(succeeded) {
					// Set the allocated block's address and exit the loop
					allocatedMemory = (AllocMetadata*)originalFreeList;
					break;
				}

				// Check if there is enough room at the top of the stack for the requested block
				size_t stackTop = currentArena->stackTop;
				if(stackTop + size <= ALLOC_ARENA_SIZE) {
					// Move the stack's top up
					currentArena->stackTop += size;

					// Set the address of the allocated memory and exit the loop
					allocatedMemory = (AllocMetadata*)((char_t*)currentArena + stackTop);
				}
				
				// Move on to the next item
				currentArenaListItem = currentArenaListItem->next;
			}

			// Create a new arena if no arena that can fit the current block was found
			if(!allocatedMemory) {
				// Create a new arena
				currentArenaListItem = (ArenaListItem*)CreateAllocArena(size);
				if(!currentArenaListItem)
					return nullptr;
				
				// Set the address of the allocated memory
				allocatedMemory = (AllocMetadata*)((char_t*)currentArenaListItem + sizeof(ArenaInfo));

				// Add the arena to the arena list
				currentArenaListItem->prev->next = currentArenaListItem;
				currentArenaListItem->next->prev = currentArenaListItem;
			}

			// Increase the current memory usage by the final size
			totalMemoryUsage[memoryUsage] += size;

			// Store the allocation's metadata
			allocatedMemory->size = size;
			allocatedMemory->reallocCount = 0;

			return allocatedMemory + 1;
		}

		return nullptr;
	}
	void* calloc(size_t nmemb, size_t size, MemoryUsage memoryUsage) {
		// Use malloc to allocate the required memory and zero it out
		void* mem = malloc(nmemb * size, memoryUsage);
		if(!mem)
			return nullptr;
		
		memset(mem, 0, nmemb * size);

		return mem;
	}
	void* realloc(void* ptr, size_t newSize, MemoryUsage memoryUsage) {
		// Free and reallocate the memory
		free(ptr, memoryUsage);
		ptr = malloc(newSize, memoryUsage);

		return ptr;
	}
	void free(void* ptr, MemoryUsage memoryUsage) {
		// Exit the function if a nullptr was given
		if(!ptr)
			return;

		// Load the current allocation's size
		AllocMetadata* allocation = (AllocMetadata*)ptr - 1;
		size_t size = allocation->size;

		// Decrease the current memory usage by the allocation's size
		totalMemoryUsage[memoryUsage] -= size;

		// Retrieve the allocation's arena
		size_t offset = (size_t)allocation - (size_t)arenaVirtualMemoryStart;
		offset &= ~(ALLOC_ARENA_SIZE - 1);

		ArenaInfo* currentArena = (ArenaInfo*)((char_t*)arenaVirtualMemoryStart + offset);

		// Check if the current block is small
		if(size <= SMALL_ARENA_MAX_SIZE) {
			// Calculate the index of the current block's free list
			size_t freeListIndex = size >> SMALL_ARENA_MIN_SIZE_LOG2;
			--freeListIndex;

			// Insert a new free node into the free list
			std::atomic<SmallFreeListItem*>* freeList = currentArena->smallFreeLists + freeListIndex;
			SmallFreeListItem* originalFreeList = *freeList;
			SmallFreeListItem* freeListItem = (SmallFreeListItem*)allocation;
			bool8_t succeeded = false;

			while(!succeeded) {
				// Set the free list item's new next pointer
				freeListItem->next = originalFreeList;

				// Try to set the new free list head
				succeeded = freeList->compare_exchange_weak(originalFreeList, freeListItem);
			}
		}
	}

	volatile atomic_size_t* GetMemoryUsage() {
		return totalMemoryUsage;
	}
	size_t GetMemoryUsage(MemoryUsage memoryUsage) {
		return totalMemoryUsage[memoryUsage];
	}

}