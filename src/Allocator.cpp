#include "Allocator.hpp"
#include "Platform.hpp"
#include "Debug.hpp"
#include <atomic>

#ifdef WFE_PLATFORM_LINUX
#include <malloc.h>
#endif

namespace wfe {
	// Constants
	static const size_t MEMORY_USAGE_TYPE_STACK_MAX_SIZE = 128;

	// Internal variables
	volatile atomic_size_t memoryUsage[MEMORY_USAGE_TYPE_COUNT];

	thread_local size_t memoryUsageTypeStackSize = 0;
	thread_local MemoryUsageType memoryUsageTypeStack[MEMORY_USAGE_TYPE_STACK_MAX_SIZE];

	void* AllocMemory(size_t size) {
		// Retrieve the current memory usage type
		MemoryUsageType memoryUsageType;
		if(!memoryUsageTypeStackSize)
			memoryUsageType = MEMORY_USAGE_TYPE_OTHER;
		else
			memoryUsageType = memoryUsageTypeStack[memoryUsageTypeStackSize - 1];
		
		// Increase the requested size to fit the memory usage and try to allocate the memory
		void* mem = PlatformAllocateMemory(size + sizeof(MemoryUsageType));
		if(!mem)
			return nullptr;
		
		// Set the memory's usage type
		MemoryUsageType* memoryUsageLocation = (MemoryUsageType*)((char_t*)mem + size);
		*memoryUsageLocation = memoryUsageType;

		// Increase the current memory usage
		memoryUsage[memoryUsageType] += size + sizeof(MemoryUsageType);

		return mem;
	}
	void* ReallocMemory(void* mem, size_t newSize) {
		// Get the memory's old size
		size_t oldSize = PlatformGetMemorySize(mem) - sizeof(MemoryUsageType);

		// Get the memory's usage type
		MemoryUsageType* memoryUsageLocation = (MemoryUsageType*)((char_t*)mem + oldSize);
		MemoryUsageType memoryUsageType = *memoryUsageLocation;

		// Reallocate the memory
		void* newMem = PlatformReallocateMemory(mem, newSize + sizeof(MemoryUsageType));
		if(!newMem)
			return nullptr;
		
		// Set the memory's usage type
		memoryUsageLocation = (MemoryUsageType*)((char_t*)newMem + newSize);
		*memoryUsageLocation = memoryUsageType;

		// Update the current memory usage
		memoryUsage[memoryUsageType] += newSize - oldSize;

		return newMem;
	}
	void FreeMemory(void* mem) {
		// Get the memory's size
		size_t size = PlatformGetMemorySize(mem) - sizeof(MemoryUsageType);

		// Get the memory's usage type
		MemoryUsageType* memoryUsageLocation = (MemoryUsageType*)((char_t*)mem + size);
		MemoryUsageType memoryUsageType = *memoryUsageLocation;

		// Decrease the current memory usage
		memoryUsage[memoryUsageType] -= size;

		// Free the memory
		PlatformFreeMemory(mem);
	}
	size_t GetMemorySize(void* mem) {
		return PlatformGetMemorySize(mem) - sizeof(MemoryUsageType);
	}
	
	void PushMemoryUsageType(MemoryUsageType memoryUsageType) {
		// Push the memory usage type at the top of the stack
		memoryUsageTypeStack[memoryUsageTypeStackSize++] = memoryUsageType;
	}
	void PopMemoryUsageType() {
		// Decrement the stack's size
		--memoryUsageTypeStackSize;
	}
	MemoryUsageType GetMemoryUsageType() {
		return memoryUsageTypeStack[memoryUsageTypeStackSize - 1];
	}
}