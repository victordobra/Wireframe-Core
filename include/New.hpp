#pragma once

#include "Defines.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "Memory.hpp"

namespace wfe {
	/// @brief Creates a new object.
	/// @tparam T The type of object to create.
	/// @tparam ...Args The arg types to create the object with.
	/// @param memoryUsage The memory usage type of the object's allocated memory.
	/// @param ...args The args to create the object with.
	/// @return A pointer to the new object.
	template<class T, class... Args>
	WFE_INLINE T* NewObject(MemoryUsage memoryUsage, Args&&... args) {
		// Allocate memory for the new object
		T* newObject = (T*)malloc(sizeof(T), memoryUsage);
		
		// Check if the memory was allocated correctly
		if(!newObject)
			throw BadAllocException("Failed to allocate object memory!");
		
		// Construct the object in the new memory block
		new(newObject) T;

		return newObject;
	}
	/// @brief Destroys the given object.
	/// @tparam T The type of object to destroy.
	/// @param object A pointer to the object to destroy.
	/// @param memoryUsage The memory usage type of the object's allocated memory.
	template<class T>
	WFE_INLINE void DestroyObject(T* object, MemoryUsage memoryUsage) {
		// Destruct the object
		object->~T();

		// Free the object's memory
		free(object, MEMORY_USAGE_OTHER);
	}
	/// @brief Creates a new object array.
	/// @tparam T The array's object type.
	/// @param size The number of elements in the array.
	/// @param memoryUsage The memory usage type of the array's allocated memory.
	/// @return A pointer to the new array.
	template<class T>
	WFE_INLINE T* NewArray(size_t size, MemoryUsage memoryUsage) {
		// Allocate memory for the new array
		T* newArray = (T*)malloc(size * sizeof(T), memoryUsage);

		// Check if the memory was allocated correctly
		if(!newArray)
			throw BadAllocException("Failed to allocate array memory!");
		
		// Construct the objects in the new memory block
		T* end = newArray + size;
		for(T* object = newArray; object != end; ++object)
			new(object) T;
		
		return newArray;
	}
	/// @brief Resizes the given object array.
	/// @tparam T The array's object type.
	/// @param array The array to resize.
	/// @param oldSize The old number of elements in the array.
	/// @param newSize The new number of elements in the array.
	/// @param memoryUsage The memory usage type of the array's allocated memory.
	/// @return A pointer to the resized array.
	template<class T>
	WFE_INLINE T* ResizeArray(T* array, size_t oldSize, size_t newSize, MemoryUsage memoryUsage) {
		// Exit the function if the old size is equal to the new size
		if(oldSize == newSize)
			return array;

		if(oldSize < newSize) {
			// Reallocate the array's memory
			array = (T*)realloc(array, newSize * sizeof(T), memoryUsage);

			// Check if the memory was reallocated correctly
			if(!array)
				throw BadAllocException("Failed to reallocate array memory!");
			
			// Construct the new objects
			T* end = array + newSize;
			for(T* object = array + oldSize; object != end; ++object)
				new(object) T;
		} else {
			// Destruct the old objects
			T* end = array + oldSize;
			for(T* object = array + newSize; object != end; ++object)
				object->~T();
			
			// Reallocate the array's memory
			array = (T*)realloc(array, newSize * sizeof(T), memoryUsage);

			// Check if the memory was reallocated correctly
			if(!array)
				throw BadAllocException("Failed to reallocate array memory!");
		}

		return array;
	}
	/// @brief Destroys the given object array.
	/// @tparam T The array's object type.
	/// @param array The array to destroy.
	/// @param size The number of elements in the array.
	/// @param memoryUsage The memory usage type of the array's allocated memory.
	template<class T>
	WFE_INLINE void DestroyArray(T* array, size_t size, MemoryUsage memoryUsage) {
		// Destruct the array's objects
		T* end = array + size;
		for(T* object = array; object != end; ++object)
			object->~T();
		
		// Free the array's memory
		free(array, memoryUsage);
	}
}