#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Platform.hpp"
#include <stdlib.h>

namespace wfe {
	void* PlatformAllocateMemory(size_t size) {
		// Allocate the memory using malloc
		return ::malloc(size);
	}
	void* PlatformAllocateZeroMemory(size_t size) {
		// Allocate the memory using calloc
		return ::calloc(1, size);
	}
	void* PlatformReallocateMemory(void* oldMem, size_t newSize) {
		// Reallocate the memory using realloc
		return ::realloc(oldMem, newSize);
	}
	void PlatformFreeMemory(void* mem) {
		// Free the memory using free
		::free(mem);
	}
}

#endif