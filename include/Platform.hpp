#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief Allocates memory on the heap using the platform's heap management functions.
	/// @param size The size, in bytes of the allocated memory.
	/// @return A pointer to the allocated memory.
	void* PlatformAllocateMemory(size_t size);
	/// @brief Reallocates the given heap memory, changing its size, using the platform's heap management functions.
	/// @param mem The memory to be reallocated.
	/// @param newSize The new size, in bytes, of the reallocated memory.
	/// @return A pointer to the new, reallocated memory.
	void* PlatformReallocateMemory(void* mem, size_t newSize);
	/// @brief Frees the given heap memory using the platform's heap management functions.
	/// @param mem The memory to be freed.
	void PlatformFreeMemory(void* mem);
	/// @brief Gets the size of the given heap memory block using the platform's heap management functions.
	/// @param mem The memory block whose size to get.
	/// @return The size, in bytes, of the memory block.
	size_t PlatformGetMemorySize(void* mem);
}