#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform.hpp"
#include <heapapi.h>

namespace wfe {
	void* PlatformAllocateMemory(size_t size) {
		// Allocate the memory using HeapAlloc
		return (void*)HeapAlloc(GetProcessHeap(), 0, size);
	}
	void* PlatformAllocateZeroMemory(size_t size) {
		// Allocate the memory using HeapAlloc and use the HEAP_ZERO_MEMORY flag
		return (void*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	}
	void* PlatformReallocateMemory(void* oldMem, size_t newSize) {
		// Reallocate the memory using HeapReAlloc
		return (void*)HeapReAlloc(GetProcessHeap(), 0, oldMem, newSize);
	}
	void PlatformFreeMemory(void* mem) {
		// Free the memory using HeapFree
		auto result = HeapFree(GetProcessHeap(), 0, mem);
	}
}

#endif