#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief Specifies what a block of memory is used for.
	typedef enum {
		/// @brief Default memory usage type for when no memory type is specified.
		MEMORY_USAGE_OTHER,
		/// @brief Array memory usage type.
		MEMORY_USAGE_ARRAY,
		/// @brief String memory usage type.
		MEMORY_USAGE_STRING,
		/// @brief Object allocated on the heap usage type.
		MEMORY_USAGE_HEAP_OBJECT,
		/// @brief The number of possible memory usages.
		MEMORY_USAGE_COUNT
	} MemoryUsage;

	/// @brief Allocates a memory block.
	/// @param size The size of the memory block.
	/// @return A pointer to the resulting memory block, or a nullptr if the block couldn't be allocated.
	void* malloc(size_t size, MemoryUsage memoryUsage);
	/// @brief Allocates a memory block with nmemb members of size bytes each. Same as malloc, however all bytes in the memory block are set to 0.
	/// @param nmemb The number of members to allocate.
	/// @param size The size of the memory block.
	/// @return A pointer to the resulting memory block, or a nullptr if the block couldn't be allocated.
	void* calloc(size_t nmemb, size_t size, MemoryUsage memoryUsage);
	/// @brief Reallocates the given memory block.
	/// @param ptr The memory block to reallocate.
	/// @param oldSize The old size of the memory block.
	/// @param newSize The new size of the memory block.
	/// @return A pointer to the reallocated memory block.
	void* realloc(void* ptr, size_t oldSize, size_t newSize, MemoryUsage memoryUsage);
	/// @brief Frees the given memory block.
	/// @param ptr The memory block to free.
	void free(void* ptr, size_t size, MemoryUsage memoryUsage);
	/// @brief Gets the memory usage of the entire application.
	/// @return A pointer to a size_t array of size MEMORY_USAGE_COUNT, specifying the memory usage for every single type.
	size_t* GetMemoryUsage();
	/// @brief Gets the memory usage for the given type.
	/// @param memoryUsage The memory usage type to check for.
	/// @return The total memory usage for the given type.
	size_t GetMemoryUsage(MemoryUsage memoryUsage);

	/// @brief Copies from one specified memory block to another.
	/// @param dest The target memory block.
	/// @param src The source memory block.
	/// @param size The size, in bytes, of the memory blocks.
	/// @return A pointer to the target memory block.
	void* memcpy(void* dest, const void* src, size_t size);
	/// @brief Copies from one specified memory block to another, guaranteeing correct behaviour with overlapping memory.
	/// @param dest The target memory block.
	/// @param src The source memory block.
	/// @param size The size, in bytes, of the memory blocks.
	/// @return A pointer to the target memory block.
	void* memmove(void* dest, const void* src, size_t size);
	/// @brief Copies from one specified memory block to another until a specific character is met or the block has been fully copied.
	/// @param dest The target memory block.
	/// @param src The source memory block.
	/// @param c The termination character.
	/// @param n The size, in bytes, of the memory blocks.
	/// @return The first instance of c in the target block, or a nullptr if it doesn't exist.
	void* memccpy(void* dest, const void* src, int32_t c, size_t size);
	/// @brief Sets every byte in the specified memory block to the specified character.
	/// @param ptr The target memory block.
	/// @param c The character to fill the memory block with.
	/// @param size The size of the memory block.
	/// @return A pointer to the target memory block.
	void* memset(void* ptr, int32_t c, size_t size);

	/// @brief Compares the two given memory blocks.
	/// @param ptr1 The first memory block.
	/// @param ptr2 The second memory block.
	/// @param size The size of the memory blocks.
	/// @return An integer representing the relationship between the two memory blocks: < 0 if the first memory block is smaller than the second, 0 if the two memory blocks are equal and > 0 if the first memory block if bigger than the second.
	int32_t memcmp(const void* ptr1, const void* ptr2, size_t size);
	/// @brief Finds the first instance of the given character in the memory block.
	/// @param ptr The memory block to search in.
	/// @param c The character to search for.
	/// @param size The size of the memory block.
	/// @return A pointer to the first instance of the character, or a nullptr if it doesn't exist.
	void* memchr(void* ptr, int32_t c, size_t size);
	/// @brief Finds the first instance of the given character in the memory block.
	/// @param ptr The memory block to search in.
	/// @param c The character to search for.
	/// @param size The size of the memory block.
	/// @return A pointer to the first instance of the character, or a nullptr if it doesn't exist.
	const void* memchr(const void* ptr, int32_t c, size_t size);
	/// @brief Finds the first instance of the given character starting from the given pointer, Same as memchr, but with no size limit.
	/// @param ptr The pointer to start from.
	/// @param c The character to search for.
	/// @return A pointer to the first instance of the character.
	void* rawmemchr(void* ptr, int32_t c);
	/// @brief Finds the first instance of the given character starting from the given pointer, Same as memchr, but with no size limit.
	/// @param ptr The pointer to start from.
	/// @param c The character to search for.
	/// @return A pointer to the first instance of the character.
	const void* rawmemchr(const void* ptr, int32_t c);
	/// @brief Finds the last instance of the given character in the memory block.
	/// @param ptr The memory block to search in.
	/// @param c The character to search for.
	/// @param size The size of the memory block.
	/// @return A pointer to the last instance of the character, or a nullptr if it doesn't exist.
	void* memrchr(void* ptr, int32_t c, size_t size);
	/// @brief Finds the last instance of the given character in the memory block.
	/// @param ptr The memory block to search in.
	/// @param c The character to search for.
	/// @param size The size of the memory block.
	/// @return A pointer to the last instance of the character, or a nullptr if it doesn't exist.
	const void* memrchr(const void* ptr, int32_t c, size_t size);

	/// @brief Copies from one specified string to another.
	/// @param dest The target string.
	/// @param src The source string.
	/// @return The target string.
	char_t* strcpy(char_t* dest, const char_t* src);
	/// @brief Copies at most size characters from one specified string to another.
	/// @param dest The target string.
	/// @param src The source string.
	/// @param size The maximum number of characters to copy.
	/// @return The target string.
	char_t* strncpy(char_t* dest, const char_t* src, size_t size);
	/// @brief Appends one given string to another.
	/// @param dest The target string.
	/// @param src The source string.
	/// @return The target string.
	char_t* strcat(char_t* dest, const char_t* src);
	/// @brief Appends at most size characters from one given string to another.
	/// @param dest The target string.
	/// @param src The source string.
	/// @param size The maximum number of characters to append.
	/// @return The target string.
	char_t* strncat(char_t* dest, const char_t* src, size_t size);

	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return An integer representing the relationship between the two strings: < 0 if the first string is smaller than the second, 0 if the two strings are equal and > 0 if the first string if bigger than the second.
	int32_t strcmp(const char_t* str1, const char_t* str2);
	/// @brief Compares the first size characters of the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @param size The number of characters to compare.
	/// @return An integer representing the relationship between the two string sections: < 0 if the first string section is smaller than the second, 0 if the two string sections are equal and > 0 if the first string section if bigger than the second.
	int32_t strncmp(const char_t* str1, const char_t* str2, size_t size);
	/// @brief Finds the first instance of the given character in the given string.
	/// @param str The string to search in.
	/// @param c The character to search for.
	/// @return A pointer to the first instance of the given charcter, or a nullptr if it doesn't exist.
	char_t* strchr(char_t* str, int32_t c);
	/// @brief Finds the first instance of the given character in the given string.
	/// @param str The string to search in.
	/// @param c The character to search for.
	/// @return A pointer to the first instance of the given charcter, or a nullptr if it doesn't exist.
	const char_t* strchr(const char_t* str, int32_t c);
	/// @brief Finds the last instance of the given character in the given string.
	/// @param str The string to search in.
	/// @param c The character to search for.
	/// @return A pointer to the last instance of the given charcter, or a nullptr if it doesn't exist.
	char_t* strrchr(char_t* str, int32_t c);
	/// @brief Finds the last instance of the given character in the given string.
	/// @param str The string to search in.
	/// @param c The character to search for.
	/// @return A pointer to the last instance of the given charcter, or a nullptr if it doesn't exist.
	const char_t* strrchr(const char_t* str, int32_t c);
	/// @brief Finds the length of the initial segment of the given string that doesn't contain any of the characters in reject.
	/// @param str The string to search in.
	/// @param reject The string with every character that should be avoided.
	/// @return The length of the wanted segment.
	size_t strcspn(const char_t* str, const char_t* reject);
	/// @brief Finds the length of the initial segment of the given string that only contains the characters in accept.
	/// @param str The string to search in.
	/// @param reject The string with every character that should be in the segment.
	/// @return The length of the wanted segment.
	size_t strspn(const char_t* str, const char_t* accept);
	/// @brief Find the first occurence of any character found in accept in the given string.
	/// @param str The string to search in.
	/// @param accept The string with every character to look for.
	/// @return A pointer to the first wanted character, or a nullptr if it doesn't exist.
	char_t* strpbrk(char_t* str, const char_t* accept);
	/// @brief Find the first occurence of any character found in accept in the given string.
	/// @param str The string to search in.
	/// @param accept The string with every character to look for.
	/// @return A pointer to the first wanted character, or a nullptr if it doesn't exist.
	const char_t* strpbrk(const char_t* str, const char_t* accept);
	/// @brief Finds the first occurence of the wanted string in the given string.
	/// @param str The string to search in.
	/// @param wanted The string to search for.
	/// @return A pointer to the first occurence of the wanted string, or a nullptr if it doesn't exist.
	char_t* strstr(char_t* str, const char_t* wanted);
	/// @brief Finds the first occurence of the wanted string in the given string.
	/// @param str The string to search in.
	/// @param wanted The string to search for.
	/// @return A pointer to the first occurence of the wanted string, or a nullptr if it doesn't exist.
	const char_t* strstr(const char_t* str, const char_t* wanted);

	/// @brief Divides the given string into segments seperated by characters in delim.
	/// @param str The string to divide; pass in a nullptr to continue with the last string.
	/// @param delim The string with the separating characters.
	/// @return A pointer to the new string.
	char_t* strtok(char_t* str, const char_t* delim);

	/// @brief Gets the length of the given string.
	/// @param str The string.
	/// @return The length of the string.
	size_t strlen(const char_t* str);
	/// @brief Gets the length of the given string, while scaning at most size characters.
	/// @param str The string.
	/// @param size The maximum number of characters to compare.
	/// @return The length of the string.
	size_t strnlen(const char_t* str, size_t size);
}