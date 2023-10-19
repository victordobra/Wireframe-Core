#include "Memory.hpp"
#include "Platform.hpp"
#include "Debug.hpp"
#include <atomic>

#ifdef WFE_PLATFORM_LINUX
#include <malloc.h>
#endif

namespace wfe {
	// Internal variables
	volatile atomic_size_t totalMemoryUsage[MEMORY_USAGE_COUNT];

	// Public functions
	void* malloc(size_t size, MemoryUsage memoryUsage) {
		// Increase the current memory usage
		totalMemoryUsage[memoryUsage] += size;

		// Allocate the memory using the stdlib malloc
		return ::malloc(size);
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
		// Get the old memory's size
#if defined(WFE_PLATFORM_WINDOWS)
		size_t size = _msize(ptr);
#elif defined(WFE_PLATFORM_LINUX)
		size_t size = malloc_usable_size(ptr);
#endif

		// Modify the memory usage
		totalMemoryUsage[memoryUsage] -= size;
		totalMemoryUsage[memoryUsage] += newSize;

		// Reallocate the memory using the stdlib realloc
		return ::realloc(ptr, newSize);
	}
	void free(void* ptr, MemoryUsage memoryUsage) {
		// Get the memory block's size
#if defined(WFE_PLATFORM_WINDOWS)
		size_t size = _msize(ptr);
#elif defined(WFE_PLATFORM_LINUX)
		size_t size = malloc_usable_size(ptr);
#endif

		// Decrease the current memory usage
		totalMemoryUsage[memoryUsage] -= size;

		// Free the memory using the stdlib free
		::free(ptr);
	}

	volatile atomic_size_t* GetMemoryUsage() {
		return totalMemoryUsage;
	}
	size_t GetMemoryUsage(MemoryUsage memoryUsage) {
		return totalMemoryUsage[memoryUsage];
	}

}