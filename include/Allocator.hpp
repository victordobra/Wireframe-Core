#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief Specifies what a block of memory is used for.
	typedef enum {
		/// @brief The memory usage type for a running command that will finish execution soon.
		MEMORY_USAGE_TYPE_COMMAND,
		/// @brief The memory usage type for the program's loaded assets.
		MEMORY_USAGE_TYPE_ASSETS,
		/// @brief The memory usage type for the program's components' data.
		MEMORY_USAGE_TYPE_COMPONENTS,
		/// @brief The memory usage type for the program's renderer.
		MEMORY_USAGE_TYPE_RENDERER,
		/// @brief The memory usage type for other purposes not fit by the other options.
		MEMORY_USAGE_TYPE_OTHER,
		/// @brief The number of memory usage types.
		MEMORY_USAGE_TYPE_COUNT
	} MemoryUsageType;

	/// @brief Allocates a memory block.
	/// @param size The size of the memory block.
	/// @return A pointer to the resulting memory block, or a nullptr if the block couldn't be allocated.
	void* AllocMemory(size_t size);
	/// @brief Reallocates the given memory block.
	/// @param mem The memory block to reallocate.
	/// @param newSize The new size of the memory block.
	/// @return A pointer to the reallocated memory block.
	void* ReallocMemory(void* mem, size_t newSize);
	/// @brief Frees the given memory block.
	/// @param mem The memory block to free.
	void FreeMemory(void* mem);
	/// @brief Gets the size of the given heap memory block.
	/// @param mem The memory block whose size to get.
	/// @return The size, in bytes, of the memory block.
	size_t GetMemorySize(void* mem);

	/// @brief Pushes the memory usage type to the stack to be used by every following allocation.
	/// @param memoryUsageType The memory usage type to push.
	void PushMemoryUsageType(MemoryUsageType memoryUsageType);
	/// @brief Pops the last memory usage type from the stack.
	void PopMemoryUsageType();
	/// @brief Gets the current memory usage type.
	/// @return The memory usage type at the top of the stack.
	MemoryUsageType GetMemoryUsageType();
	/// @brief Gets the memory usage of the entire application.
	/// @return A pointer to a size_t array of size MEMORY_USAGE_COUNT, specifying the memory usage for every single type.
	volatile atomic_size_t* GetMemoryUsage();
	/// @brief Gets the memory usage for the given type.
	/// @param memoryUsageType The memory usage type to check for.
	/// @return The total memory usage for the given type.
	size_t GetMemoryUsage(MemoryUsageType memoryUsageType);
}
