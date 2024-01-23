#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform.hpp"

#include <heapapi.h>

namespace wfe {
	void* PlatformAllocateMemory(size_t size) {
		// Allocate the memory using HeapAlloc
		return (void*)HeapAlloc(GetProcessHeap(), 0, size);
	}
	void* PlatformReallocateMemory(void* oldMem, size_t newSize) {
		// Reallocate the memory using HeapReAlloc
		return (void*)HeapReAlloc(GetProcessHeap(), 0, oldMem, newSize);
	}
	void PlatformFreeMemory(void* mem) {
		// Free the memory using HeapFree
		HeapFree(GetProcessHeap(), 0, mem);
	}
	size_t PlatformGetMemorySize(void* mem) {
		// Get the memory's size using HeapSize
		return (size_t)HeapSize(GetProcessHeap(), 0, mem);
	}
}

#endif