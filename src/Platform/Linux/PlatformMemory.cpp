#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Platform.hpp"
#include <stdlib.h>
#include <malloc.h>

namespace wfe {
	void* PlatformAllocateMemory(size_t size) {
		// Allocate the memory using malloc
		return ::malloc(size);
	}
	void* PlatformReallocateMemory(void* mem, size_t newSize) {
		// Reallocate the memory using realloc
		return ::realloc(mem, newSize);
	}
	void PlatformFreeMemory(void* mem) {
		// Free the memory using free
		::free(mem);
	}
	size_t PlatformGetMemorySize(void* mem) {
		// Get the memory's size using malloc_usable_size
		return malloc_usable_size(mem);
	}
}

#endif