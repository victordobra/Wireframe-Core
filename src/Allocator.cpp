#include "Memory.hpp"
#include "Platform.hpp"
#include "Debug.hpp"

namespace wfe {
	// Internal variables
	size_t totalMemoryUsage[MEMORY_USAGE_COUNT];

	// Public functions
	void CreateAllocator() {
		
	}
	void DeleteAllocator() {
		
	}

	void* malloc(size_t size, MemoryUsage memoryUsage) {
		return nullptr;
	}
	void* calloc(size_t nmemb, size_t size, MemoryUsage memoryUsage) {
		return nullptr;
	}
	void* realloc(void* ptr, size_t newSize, MemoryUsage memoryUsage) {
		return nullptr;
	}
	void free(void* ptr, MemoryUsage memoryUsage) {
		
	}

	size_t* GetMemoryUsage() {
		return totalMemoryUsage;
	}
	size_t GetMemoryUsage(MemoryUsage memoryUsage) {
		return totalMemoryUsage[memoryUsage];
	}

}