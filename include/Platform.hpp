#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief Allocates memory on the heap.
	/// @param size The size, in bytes, of the allocated memory.
	/// @return A pointer to the allocated memory.
	void* PlatformAllocateMemory(size_t size);
	/// @brief Allocates memory on the heap with all bytes set to 0.
	/// @param size The size, in bytes, of the allocated memory.
	/// @return A pointer to the allocated memory.
	void* PlatformAllocateZeroMemory(size_t size);
	/// @brief Reallocates the given heap memory, changing its size.
	/// @param oldMem The memory to be reallocated.
	/// @param newSize The new size, in bytes, of the reallocated memory.
	/// @return A pointer to the new, reallocated memory.
	void* PlatformReallocateMemory(void* oldMem, size_t newSize);
	/// @brief Frees the given heap memory.
	/// @param mem The memory to be freed.
	void PlatformFreeMemory(void* mem);
}