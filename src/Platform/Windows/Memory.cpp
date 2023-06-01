#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform.hpp"
#include <heapapi.h>

namespace wfe {
	void* PlatformAllocateMemory(size_t size) {
		// Allocate the memory using HeapAlloc
		return (void*)HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, size);
	}
	void* PlatformAllocateZeroMemory(size_t nmemb, size_t size) {
		// Allocate the memory using HeapAlloc and use the HEAP_ZERO_MEMORY flag
		return (void*)HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, nmemb * size);
	}
	void* PlatformReallocateMemory(void* oldMem, size_t oldSize, size_t newSize) {
		// Reallocate the memory using HeapReAlloc
		return (void*)HeapReAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, oldMem, newSize);
	}
	void PlatformFreeMemory(void* mem, size_t size) {
		// Free the memory using HeapFree
		auto result = HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, mem);
	}
}

#endif