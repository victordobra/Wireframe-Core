#pragma once

#include "Defines.hpp"
#include "Allocator.hpp"
#include "Memory.hpp"
#include "Exception.hpp"
#include <stdarg.h>
#include <stdlib.h>
#include <string>

namespace wfe {
	/// @brief Holds a string of characters.
	class string {
	public:
		/// @brief The string's value.
		typedef char_t value_type;
		/// @brief A reference the string's value.
		typedef char_t& reference;
		/// @brief A const reference the string's value.
		typedef const char_t& const_reference;
		/// @brief A pointer the string's value.
		typedef char_t* pointer;
		/// @brief A const pointer the string's value.
		typedef const char_t* const_pointer;
		/// @brief Used for holding differences.
		typedef ptrdiff_t difference_type;
		/// @brief Used for holding sizes.
		typedef size_t size_type;

		/// @brief Creates an empty string.
		constexpr string() = default;
		/// @brief Copies the given string.
		/// @param other The string to copy.
		string(const string& other) : strSize(other.strSize), strCapacity(other.strCapacity), strData((pointer)AllocMemory(strCapacity)) {
			// Check if the memory has been allocated successfully
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");
			
			// Copy the string's contents
			memcpy(strData, other.strData, strSize + 1);
		}
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		string(string&& other) noexcept : strSize(other.strSize), strCapacity(other.strCapacity), strData(other.strData) {
			// Unlink the data from the other string
			other.strData = nullptr;
		}
		/// @brief Copies the given string.
		/// @param str The string to copy.
		string(const_pointer str) : strSize(strlen(str)), strCapacity(1), strData(nullptr) {
			// Assert that the string's size must be lower than the maximum size
			WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

			// Set the capacity to the lowest power of 2 higher than the string's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((strCapacity << step) <= strSize)
					strCapacity <<= step;
			strCapacity <<= 1;
			
			// Allocate the string's data
			strData = (pointer)AllocMemory(strCapacity);
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Copy the string's contents
			memcpy(strData, str, strSize + 1);
		}
		/// @brief Copies from the given buffer.
		/// @param ptr The buffer to copy from.
		/// @param n The size of the given buffer.
		string(const_pointer ptr, size_type size) : strSize(size), strCapacity(1), strData(nullptr) {
			// Assert that the string's size must be lower than the maximum size
			WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

			// Set the capacity to the lowest power of 2 higher than the string's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((strCapacity << step) <= strSize)
					strCapacity <<= step;
			strCapacity <<= 1;
			
			// Allocate the string's data
			strData = (pointer)AllocMemory(strCapacity);
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Copy the string's contents
			memcpy(strData, ptr, strSize);

			// Place a null termination character at the end
			strData[strSize] = 0;
		}
		/// @brief Fills the string with the given character.
		/// @param size The wanted length of the string.
		/// @param c The character to fill it with.
		string(size_type size, value_type c) : strSize(size), strCapacity(1), strData(nullptr) {
			// Assert that the string's size must be lower than the maximum size
			WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

			// Set the capacity to the lowest power of 2 higher than the string's size
			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((strCapacity << step) <= strSize)
					strCapacity <<= step;
			strCapacity <<= 1;
			
			// Allocate the string's data
			strData = (pointer)AllocMemory(strCapacity);
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Set the string's contents
			memset(strData, c, strSize);

			// Place a null termination character at the end
			strData[strSize] = 0;
		}
		/// @brief Copies the given std library string.
		/// @param str The string to copy.
		string(const std::string& str) : strSize(str.size()), strCapacity(str.capacity()), strData((pointer)AllocMemory(strCapacity)) {
			// Check if the memory has been allocated successfully
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Copy the string's contents
			memcpy(strData, str.c_str(), strSize + 1);
		}

		/// @brief Copies the given string.
		/// @param other The string to copy.
		/// @return A reference to this string.
		string& operator=(const string& other) {
			// If the given string is the same as this string, exit the function.
			if(&other == this)
				return *this;
			
			// Set the new string values
			strSize = other.strSize;

			if(strData)
				strData = (pointer)ReallocMemory(strData, other.strCapacity);
			else
				strData = (pointer)AllocMemory(strCapacity);

			strCapacity = other.strCapacity;

			// Check if the memory has been allocated successfully
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Copy the string's contents
			memcpy(strData, other.strData, strSize + 1);

			return *this;
		}
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		/// @return A reference to this string.
		string& operator=(string&& other) {
			// Free the string's contents (if they exist)
			if(strData)
				FreeMemory(strData);

			// Set the new string's values
			strSize = other.strSize;
			strCapacity = other.strCapacity;
			strData = other.strData;

			// Unlink the data from the other string
			other.strData = nullptr;

			return *this;
		}
		/// @brief Copies the given string.
		/// @param other The string to copy.
		/// @return A reference to this string.
		string& operator=(const_pointer other) {
			// Calculate the given string's size
			size_type givenStrSize = strlen(other);

			// Resize the string
			resize(givenStrSize);

			// Copy the string's contents
			memcpy(strData, other, strSize + 1);

			return *this;
		}
		/// @brief Sets the string's contents to the given character.
		/// @param c The character to replace the string's contents with.
		/// @return A reference to this string.
		string& operator=(value_type c) {
			// Resize the string
			resize(1);
			
			// Generate a uint16_t to hold what needs to be added
			uint16_t segment = ((uint8_t)c) << 8;

			// Append the segment to this string
			*(uint16_t*)(strData + strSize - 1) = segment;

			return *this;
		}

		/// @brief Convers the string to an std library string.
		operator std::string() const {
			// Create a new string with the contents of this string
			return std::string(strData, strSize);
		}

		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A reference to the wanted character.
		reference operator[](size_type ind) {
			// Assert that the wanted character's index must be lower than the string's size
			WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

			// Return a reference to the wanted character
			return strData[ind];
		}
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A const reference to the wanted character.
		const_reference operator[](size_type ind) const {
			// Assert that the wanted character's index must be lower than the string's size
			WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

			// Return a const reference to the wanted character
			return strData[ind];
		}
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A reference to the wanted character.
		reference at(size_type ind) {
			// Assert that the wanted character's index must be lower than the string's size
			WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

			// Return a reference to the wanted character
			return strData[ind];
		}
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A const reference to the wanted character.
		const_reference at(size_type ind) const {
			// Assert that the wanted character's index must be lower than the string's size
			WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

			// Return a const reference to the wanted character
			return strData[ind];
		}
		/// @brief Gets the first character of the string.
		/// @return A reference to the first character.
		reference front() {
			// Return a reference to the first character in the string
			return *strData;
		}
		/// @brief Gets the first character of the string.
		/// @return A const reference to the first character.
		const_reference front() const {
			// Return a reference to the first character in the string
			return *strData;
		}
		/// @brief Gets the last character of the string.
		/// @return A reference to the last character.
		reference back() {
			// Return a reference to the last character in the string
			return strData[strSize - 1];
		}
		/// @brief Gets the last character of the string.
		/// @return A const reference to the last character.
		const_reference back() const {
			// Return a reference to the last character in the string
			return strData[strSize - 1];
		}

		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(const string& str) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize);
		
			// Append the given string's contents to this string
			memcpy(strData + oldSize, str.strData, str.strSize + 1);

			return *this;
		}
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(string&& str) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize);
		
			// Append the given string's contents to this string
			memcpy(strData + oldSize, str.strData, str.strSize + 1);

			return *this;
		}
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(const_pointer str) {
			// Calculate the given string's size
			size_type givenStrSize = strlen(str);

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + givenStrSize);

			// Append the given string's contents to this string
			memcpy(strData + oldSize, str, givenStrSize + 1);

			return *this;
		}
		/// @brief Appends the given character to this string.
		/// @param c The character to append.
		/// @return A reference to this string.
		string& operator+=(value_type c) {
			// Resize the string
			resize(strSize + 1);
			
			// Add the given character at the end of the string
			strData[strSize - 1] = c;

			// Add a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& append(const string& str) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize);
		
			// Append the given string's contents to this string
			memcpy(strData + oldSize, str.strData, str.strSize + 1);

			return *this;
		}
		/// @brief Appends a substring of the given string to the string.
		/// @param str The string whose substring to append.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& append(const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) {
			// Assert that given substring's starting position must be lower than the given string's length
			WFE_ASSERT(subpos < str.strSize, "The given substring's starting position must be lower than the given string's length!");

			// Save the string's old size
			size_type oldSize = strSize;

			// Set the actual length of the substring
			sublen = (sublen > (str.strSize - subpos)) ? (str.strSize - subpos) : sublen;

			// Resize the string
			resize(strSize + sublen);

			// Append the substring's contents to this string.
			memcpy(strData + oldSize, str.strData + subpos, sublen);

			// Add a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& append(const_pointer str) {
			// Calculate the given string's size
			size_type givenStrSize = strlen(str);

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + givenStrSize);

			// Append the given string's contents to this string
			memcpy(strData + oldSize, str, givenStrSize + 1);

			return *this;
		}
		/// @brief Appends the specified amount of characters from the given character array to this string.
		/// @param str The character array to append from.
		/// @param size The numbers to characters to append from the character array.
		/// @return A reference to this string.
		string& append(const_pointer str, size_type size) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + size);

			// Append the given character array's contents to this string
			memcpy(strData + oldSize, str, size);

			// Place a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Appends the given character the wanted number of times to this string.
		/// @param n The number of times the character should be appended.
		/// @param c The character to append.
		/// @return A reference to this string.
		string& append(size_type n, value_type c) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + n);

			// Append the given characters to this string
			memset(strData + oldSize, c, n);

			// Place a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Appends the given character to the string.
		/// @param c The character to append.
		void push_back(value_type c) {
			// Resize the string
			resize(strSize + 1);
			
			// Add the given character at the end of the string
			strData[strSize - 1] = c;

			// Add a null termination character at the end
			strData[strSize] = 0;
		}
		/// @brief Copies the given string into this string.
		/// @param str The string to copy from.
		/// @return A reference to this string.
		string& assign(const string& str) {
			// If the given string is the same as this string, exit the function.
			if(&str == this)
				return *this;
			
			// Set the new string values
			strSize = str.strSize;

			if(strData)
				strData = (pointer)ReallocMemory(strData, str.strCapacity);
			else
				strData = (pointer)AllocMemory(strCapacity);

			strCapacity = str.strCapacity;

			// Check if the memory has been allocated successfully
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Copy the string's contents
			memcpy(strData, str.strData, strSize + 1);

			return *this;
		}
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		/// @return A reference to this string.
		string& assign(string&& other) {
			// Free the string's contents (if they exist)
			if(strData)
				FreeMemory(strData);
			
			// Set the new string's values
			strSize = other.strSize;
			strCapacity = other.strCapacity;
			strData = other.strData;

			// Unlink the data from the other string
			other.strData = nullptr;

			return *this;
		}
		/// @brief Copies a substring of the given string into this string.
		/// @param str The string whose substring to copy from.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& assign(const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) {
			// Assert that given substring's starting position must be lower than the given string's length
			WFE_ASSERT(subpos < str.strSize, "The given substring's starting position must be lower than the given string's length!");

			// Set the actual length of the substring
			sublen = (sublen > (str.strSize - subpos)) ? (str.strSize - subpos) : sublen;

			// Resize the string
			resize(sublen);

			// Copy the substring's contents
			memcpy(strData, str.strData + subpos, sublen);

			// Add a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Copies the given string into this string.
		/// @param str The string to copy from.
		/// @return A reference to this string.
		string& assign(const_pointer str) {
			// Calculate the given string's size
			size_type givenStrSize = strlen(str);

			// Resize the string
			resize(givenStrSize);

			// Copy the string's contents
			memcpy(strData, str, strSize + 1);

			return *this;
		}
		/// @brief Copies the specified amount of characters from the given character array into this string.
		/// @param str The character array to copy from.
		/// @param size The number of characters to copy from the character array.
		/// @return A reference to this string.
		string& assign(const_pointer str, size_type size) {
			// Resize the string
			resize(size);

			// Copy the string's contents
			memcpy(strData, str, strSize);

			// Place a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Replaces this string with the specified amount of consecutive copies of the given character.
		/// @param n The number of consecutive characters to replace the string with.
		/// @param c The character to fill the string with.
		/// @return A reference to this string.
		string& assign(size_type n, value_type c) {
			// Resize the string
			resize(n);

			// Set the string's contents
			memset(strData, c, strSize);

			// Place a null termination character at the end
			strData[strSize] = 0;

			return *this;
		}
		/// @brief Inserts the given string into this string.
		/// @param pos The position to insert the stirng at.
		/// @param str The string to insert.
		/// @return A reference to this string.
		string& insert(size_type pos, const string& str) {
			// Assert that the position to insert at must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to insert at must be lower than the string's length!")

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize);

			// Move part of the string forward to make room for the new string
			memmove(strData + pos + str.strSize, strData + pos, oldSize - pos + 1);

			// Copy the string into the empty space in this string
			memcpy(strData + pos, str.strData, str.strSize);

			return *this;
		}
		/// @brief Inserts a substring of the given string into this string.
		/// @param pos The position to insert the substring at.
		/// @param str The string whose substring to insert.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& insert(size_type pos, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) {
			// Assert that the position to insert at must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to insert at must be lower than the string's length!")

			// Assert that given substring's starting position must be lower than the given string's length
			WFE_ASSERT(subpos < str.strSize, "The given substring's starting position must be lower than the given string's length!");

			// Save the string's old size
			size_type oldSize = strSize;

			// Set the actual length of the substring
			sublen = (sublen > (str.strSize - subpos)) ? (str.strSize - subpos) : sublen;

			// Resize the string
			resize(strSize + sublen);

			// Move part of the string forward to make room for the new string
			memmove(strData + pos + sublen, strData + pos, oldSize - pos + 1);

			// Copy the substring into the empty space in this string
			memcpy(strData + pos, str.strData + subpos, sublen);

			return *this;
		}
		/// @brief Inserts the given string into this string.
		/// @param pos The position to insert the stirng at.
		/// @param str The string to insert.
		/// @return A reference to this string.
		string& insert(size_type pos, const_pointer str) {
			// Assert that the position to insert at must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to insert at must be lower than the string's length!")

			// Save the string's old size
			size_type oldSize = strSize;

			// Calculate the given string's size
			size_type givenStrSize = strlen(str);

			// Resize the string
			resize(strSize + givenStrSize);

			// Move part of the string forward to make room for the new string
			memmove(strData + pos + givenStrSize, strData + pos, oldSize - pos + 1);

			// Copy the string into the empty space in this string
			memcpy(strData + pos, str, givenStrSize);

			return *this;
		}
		/// @brief Inserts the specified amount of characters from the given character array into this string.
		/// @param pos The position to insert the character from the array at.
		/// @param str The character array to insert from.
		/// @param size The number of character to insert from the character array.
		/// @return A reference to this string.
		string& insert(size_type pos, const_pointer str, size_type size) {
			// Assert that the position to insert at must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to insert at must be lower than the string's length!")

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + size);

			// Move part of the string forward to make room for the new string
			memmove(strData + pos + size, strData + pos, oldSize - pos + 1);

			// Copy the character array into the empty space in this string
			memcpy(strData + pos, str, size);

			return *this;
		}
		/// @brief Inserts the specified amount of consecutive copies of this given character into this string.
		/// @param pos The position to insert the characters at.
		/// @param n The number of consecutive characters to replace the string with.
		/// @param c The character to fill the string with.
		/// @return A reference to this string.
		string& insert(size_type pos, size_type n, value_type c) {
			// Assert that the position to insert at must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to insert at must be lower than the string's length!")

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + n);

			// Move part of the string forward to make room for the new string
			memmove(strData + pos + n, strData + pos, oldSize - pos + 1);

			// Set all the characters in the empty space to the given character
			memset(strData + pos, c, n);

			return *this;
		}
		/// @brief Inserts the specified amount of consecutive copies of this given character into this string.
		/// @param pos The position to insert the characters at.
		/// @param n The number of consecutive characters to insert into this string.
		/// @param c The number of characters to insert.
		/// @return A pointer to the first character inserted.
		pointer insert(const_pointer pos, size_type n, value_type c) {
			// Assert that the position to insert at must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The position to insert at must be in range!");

			// Get the position to insert at relative to the beginning of the string
			size_type relPos = pos - begin();

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + n);

			// Move part of the string forward to make room for the new string
			memmove(strData + relPos + n, strData + relPos, oldSize - relPos + 1);

			// Set all the characters in the empty space to the given character
			memset(strData + relPos, c, n);

			return begin() + relPos;
		}
		/// @brief Insert the given character into this string.
		/// @param pos The position to insert the character at.
		/// @param c The character to insert.
		/// @return A pointer to the first character inserted.
		pointer insert(const_pointer pos, value_type c) {
			// Assert that the position to insert at must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The position to insert at must be in range!");

			// Get the position to insert at relative to the beginning of the string
			size_type relPos = pos - begin();

			// Resize the string
			resize(strSize + 1);

			// Move part of the string forward to make room for the new string
			memmove(strData + relPos + 1, strData + relPos, strSize - relPos);

			// Set the character in the empty space to the given character
			strData[relPos] = c;

			return begin() + relPos;
		}
		/// @brief Erases the given substring from this string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @return A reference to this string.
		string& erase(size_type pos = 0, size_type len = SIZE_T_MAX) {
			// Assert that the position to erase from must be lower than the string's length
			WFE_ASSERT(pos < strSize, "The position to erase from must be lower than the string's length!")

			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Move part of the string backwards to erase the wanted substring
			memcpy(strData + pos, strData + pos + len, strSize - pos - len + 1);

			// Resize the string
			resize(strSize - len);

			return *this;
		}
		/// @brief Erases the given character.
		/// @param ptr A pointer to the character to be erased.
		/// @return A pointer to the character that now occupies this character's position.
		pointer erase(const_pointer pos) {
			// Assert that the position to erase from must be in range
			WFE_ASSERT(pos >= begin() && pos < end(), "The position to erase from must be in range!")

			// Move part of the string backwards to erase the wanted character
			memcpy((pointer)pos, pos + 1, end() - pos + 1);

			// Resize the string
			resize(strSize - 1);

			return (pointer)pos;
		}
		/// @brief Erases every character of the given interval.
		/// @param first A pointer to the first character to be erased.
		/// @param last A pointer right after the last character to be erased.
		/// @return A pointer to the character that now occupies the first erased character's position.
		pointer erase(const_pointer first, const_pointer last) {
			// Assert that the delimiters for the substring to erase must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last <= end(), "The delimiters for the substring to erase must be in range!")

			// Assert that the first delimiter must be lower than or equal to the last
			WFE_ASSERT(first <= last, "The first delimiter must be lower than or equal to the last!")

			// Get the size of the erased substring
			size_type sublen = last - first;

			// Move part of the string backwards to erase the wanted character
			memcpy((pointer)first, last, end() + sublen - last);

			// Resize the string
			resize(strSize - sublen);

			return (pointer)first;
		}
		/// @brief Replaces the given region of this string with the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const string& str) {
			// Assert that the position to replace at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to replace at must be lower than the string's size!")

			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + str.strSize, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str.strData, str.strSize);

			return *this;
		}
		/// @brief Replaces the region between the two given pointer with the given string.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const string& str)	{
			// Assert that the delimiters for the substring to replace must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last <= end(), "The delimiters for the substring to replace must be in range!")

			// Assert that the first delimiter must be lower than or equal to the last
			WFE_ASSERT(first <= last, "The first delimiter must be lower than or equal to the last!")
			
			// Get the starting position and size of the replaced substring
			size_type pos = first - strData;
			size_type len = last - first;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + str.strSize - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + str.strSize, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str.strData, str.strSize);

			return *this;
		}
		/// @brief Replaces the given region to this string with a substring of the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string whose substring to replace the region with.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) {
			// Assert that the position to replace at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to replace at must be lower than the string's size!")
		
			// Assert that the starting position of the substring to replace with must be lower than the given string's size
			WFE_ASSERT(subpos < str.strSize, "The starting position of the substring to replace with must be lower than the given string's size!")

			// Set the actual length of the substrings
			len = (len > (strSize - pos)) ? (strSize - pos) : len;
			sublen = (sublen > (str.strSize - subpos)) ? (str.strSize - subpos) : sublen;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + sublen - len);

			// Move part of the string to make room for the inserted substring
			memmove(strData + pos + sublen, strData + pos + len, oldSize - pos - len + 1);

			// Copy the substring into the new empty space
			memcpy(strData + pos, str.strData + subpos, sublen);

			return *this;
		}
		/// @brief Replaces the given region of this string with the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const_pointer str) {
			// Assert that the position to replace at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to replace at must be lower than the string's size!")
		
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Calculate the length of the given string
			size_type givenStrSize = strlen(str);

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + givenStrSize - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + givenStrSize, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str, givenStrSize);

			return *this;
		}
		/// @brief Replaces the region between the two given pointer with the given string.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const_pointer str) {
			// Assert that the delimiters for the substring to replace must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last <= end(), "The delimiters for the substring to replace must be in range!")

			// Get the starting position and size of the replaced substring
			size_type pos = first - strData;
			size_type len = last - first;

			// Calculate the size of the given string
			size_type givenStrSize = strlen(str);

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + givenStrSize - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + givenStrSize, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str, givenStrSize);

			return *this;
		}
		/// @brief Replaces the given region of this string with characters from the given character array.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The character whose elements to replace the region with.
		/// @param size The number of characters to replace the region with from the character array.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const_pointer str, size_type size) {
			// Assert that the position to replace at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to replace at must be lower than the string's size!")
		
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + size - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + size, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str, size);

			return *this;
		}
		/// @brief Replaces the region between the two given pointers with characters from the given character array.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The character whose elements to replace the region with.
		/// @param size The number of characters to replace the region with from the character array.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const_pointer str, size_type size) {
			// Assert that the delimiters for the substring to replace must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last <= end(), "The delimiters for the substring to replace must be in range!")

			// Get the starting position and size of the replaced substring
			size_type pos = first - strData;
			size_type len = last - first;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + size - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + size, strData + pos + len, oldSize - pos - len + 1);

			// Copy the string into the new empty space
			memcpy(strData + pos, str, size);

			return *this;
		}
		/// @brief Replace the given region of this string with the given number of consecutive copies of the given character.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param n The number of consecutive copies of the given character to replace the region with.
		/// @param c The character whose copies to replace the string with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, size_type n, value_type c) {
			// Assert that the position to replace at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to replace at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + n - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + n, strData + pos + len, oldSize - pos - len + 1);

			// Fill the string's new empty space with the given character
			memset(strData + pos, c, n);

			return *this;
		}
		/// @brief Replaces the region between the two given pointers with the given number of consecutive copies of the given character.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param n The number of consecutive copies of the given character to replace the region with.
		/// @param c The character whose copies to replace the string with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, size_type n, value_type c) {
			// Assert that the delimiters for the substring to replace must be in range
			WFE_ASSERT(first >= begin() && first < end() && last >= begin() && last <= end(), "The delimiters for the substring to replace must be in range!")

			// Get the starting position and size of the replaced substring
			size_type pos = first - strData;
			size_type len = last - first;

			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(strSize + n - len);

			// Move part of the string to make room for the inserted string
			memmove(strData + pos + n, strData + pos + len, oldSize - pos - len + 1);

			// Fill the string's new empty space with the given character
			memset(strData + pos, c, n);

			return *this;
		}
		/// @brief Swaps the two string's contents.
		/// @param str The string to swap with.
		void swap(string& str) {
			// If the given string is the same as this string, exit the function.
			if(&str == this)
				return;
		
			// Save all member variables of this string separately
			size_type auxSize = strSize;
			size_type auxCapacity = strCapacity;
			pointer auxData = strData;

			// Copy the data from the given string to this string
			strSize = str.strSize;
			strCapacity = str.strCapacity;
			strData = str.strData;

			// Copy the saved data to the given string
			str.strSize = auxSize;
			str.strCapacity = auxCapacity;
			str.strData = auxData;
		}
		/// @brief Removes the last element of the string.
		void pop_back() {
			// Exit the function if the stirng is empty
			if(!strSize)
				return;
		
			// Resize the string
			resize(strSize - 1);

			// Set the last character to a null termination character
			strData[strSize] = 0;
		}

		/// @brief Gets the beginning of the string.
		/// @return A pointer to the beginning of the string.
		pointer begin() {
			return strData;
		}
		/// @brief Gets the beginning of the string.
		/// @return A pointer to the beginning of the string.
		const_pointer begin() const {
			return strData;
		}
		/// @brief Gets the end of the string.
		/// @return A pointer to the end of the string.
		pointer end() {
			return strData + strSize;
		}
		/// @brief Gets the end of the string.
		/// @return A pointer to the end of the string.
		const_pointer end() const {
			return strData + strSize;
		}
		/// @brief Gets the const beginning of the string.
		/// @return A const pointer to the beginning of the string.
		const_pointer cbegin() const {
			return strData;
		}
		/// @brief Gets the const end of the string.
		/// @return A const pointer to the end of the string.
		const_pointer cend() const {
			return strData + strSize;
		}

		/// @brief Gets the length of the string.
		/// @return The length of the string.
		size_type size() const {
			return strSize;
		}
		/// @brief Gets the length of the string.
		/// @return The length of the string.
		size_type length() const {
			return strSize;
		}
		/// @brief Gets the maximum possible size of the string.
		/// @return The max length of the string.
		size_type max_size() const {
			return SIZE_T_MAX;
		}
		/// @brief Resizes the string.
		/// @param newSize The new size of the string.
		void resize(size_type newSize) {
			// Assert that the given size must be lower than the maximum size
			WFE_ASSERT(newSize < max_size(), "The string's size must be lower than max_size()!")

			// Save the string's old size
			size_type oldSize = strSize;

			// Set the string's new size
			strSize = newSize;

			// Check if memory should be allocated
			if(strCapacity <= strSize) {
				// Find the lowest power of two higher than the string's size
				strCapacity = 1;

				for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
					if((strCapacity << step) <= strSize)
						strCapacity <<= step;
				strCapacity <<= 1;

				if(strData) { 
					// Reallocate the string
					strData = (pointer)ReallocMemory(strData, strCapacity);
				} else {
					// Allocate the string
					strData = (pointer)AllocMemory(strCapacity);
				}

				// Check if the memory has been allocated successfully
				if(!strData)
					throw BadAllocException("Failed to allocate string data!");
			}

			if(strSize > oldSize) {
				// Add a null termination character at the end
				strData[strSize] = 0;
			}
		}
		/// @brief Resizes the string, filling it with the given character.
		/// @param newSize The new size of the string.
		/// @param c The character to fill the string with.
		void resize(size_type newSize, value_type c) {
			// Save the string's old size
			size_type oldSize = strSize;

			// Resize the string
			resize(newSize);

			if(strSize > oldSize) {
				// Fill the new space with the given character
				memset(strData + oldSize, c, strSize - oldSize);
			}
		}
		/// @brief Gets the capacity of the string.
		/// @return The new capacity of the string.
		size_type capacity() const {
			return strCapacity;
		}
		/// @brief Changes the string's capacity.
		/// @param newCapacity The new capacity of the string.
		void reserve(size_type newCapacity = 0) {
			// Make the capacity higher than the string's size
			newCapacity = (newCapacity <= strSize) ? (strSize + 1) : newCapacity;

			if(strData) { 
				// Reallocate the string
				strData = (pointer)ReallocMemory(strData, newCapacity);
			} else {
				// Allocate the string
				strData = (pointer)AllocMemory(newCapacity);
			}

			// Check if the memory has been allocated successfully
			if(!strData)
				throw BadAllocException("Failed to allocate string data!");

			// Set the string's new capacity
			strCapacity = newCapacity;
		}
		/// @brief Clears the string.
		void clear() {
			if(strData) {
				// Set the string's size to 0
				strSize = 0;

				// Place a null termination character at the beginning
				*strData = 0;
			}
		}
		/// @brief Tests if the string is empty.
		/// @return True if the string is empry, or false otherwise.
		bool8_t empty() const {
			return !strSize;
		}
		/// @brief Reduces the string's capacity to fit its size.
		void shrink_to_fit() {
			// Reserving with a new capacity of 0 will shrink the string's capacity down to the string's size
			reserve();
		}

		/// @brief Gets this string's C string.
		/// @return A const pointer to the C string.
		const_pointer c_str() const {
			return strData;
		}
		/// @brief Gets a pointer to this string's data.
		/// @return A pointer to this string's data.
		pointer data() {
			return strData;
		}
		/// @brief Gets a pointer to this string's data.
		/// @return A const pointer to this string's data.
		const_pointer data() const {
			return strData;
		}
		/// @brief Copies the wanted substring of this string into the given buffer.
		/// @param str The buffer to copy info.
		/// @param len The length of the substring.
		/// @param pos The starting position of the substring.
		/// @return The number of characters that have been copied.
		size_type copy(pointer str, size_type len, size_type pos = 0) const {
			// Assert that the position to copy from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to copy from must be lower than the string's size!")

			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Copy to the given buffer
			memcpy(str, strData + pos, len);

			return len;
		}
		/// @brief Looks for the first instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the string, or SIZE_T_MAX if none was found.
		size_type find(const string& str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			const_pointer wanted = str.strData;
			size_type wantedLength = str.strSize;

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_t position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[position] == wanted[candidate])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[position] != wanted[candidate])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(size_type);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint32_t i = 0; i != sizeof(uint32_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint16_t i = 0; i != sizeof(uint16_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[position] == *ptr && position == wantedLength - 1)
					return strSize - 1 - wantedLength;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the string, or SIZE_T_MAX if none was found.
		size_type find(const_pointer str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			const_pointer wanted = str;
			size_type wantedLength = strlen(str);

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_t position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[position] == wanted[candidate])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[position] != wanted[candidate])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(size_type);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint32_t i = 0; i != sizeof(uint32_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint16_t i = 0; i != sizeof(uint16_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}

			// Free the KMP table
			FreeMemory(table);

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[position] == *ptr && position == wantedLength - 1)
					return strSize - 1 - wantedLength;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of the given buffer in this string.
		/// @param str A pointer to the buffer to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of the buffer, or SIZE_T_MAX if none was found.
		size_type find(const_pointer str, size_type pos, size_type n) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			const_pointer wanted = str;
			size_type wantedLength = n;

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_type position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[position] == wanted[candidate])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[position] != wanted[candidate])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(size_type);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint32_t i = 0; i != sizeof(uint32_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(uint16_t i = 0; i != sizeof(uint16_t);) {
					if(wanted[position] == segmentPtr[i]) {
						// Move on to the next character
						++i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return strSize - size + i - wantedLength;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							++i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}

			// Free the KMP table
			FreeMemory(table);

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[position] == *ptr && position == wantedLength - 1)
					return strSize - 1 - wantedLength;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of the given character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the character, or SIZE_T_MAX if none was found.
		size_type find(value_type c, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Use memchr to find the wanted character
			const_pointer ptr = (const_pointer)memchr(strData + pos, c, strSize - pos);

			// Return the character's position relative to the string's data
			if(ptr)
				return ptr - strData;
		
			// No instance of the wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the string, or SIZE_T_MAX if none was found.
		size_type rfind(const string& str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos >= strSize) ? (strSize - 1) : pos;

			const_pointer wanted = str.strData;
			size_type wantedLength = str.strSize;

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_type position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[wantedLength - position - 1] == wanted[wantedLength - candidate - 1])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[wantedLength - position - 1] != wanted[wantedLength - candidate - 1])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type 
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = pos + 1;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[wantedLength - position - 1] == *ptr && position == wantedLength - 1)
					return 0;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the string, or SIZE_T_MAX if none was found.
		size_type rfind(const_pointer str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos >= strSize) ? (strSize - 1) : pos;

			const_pointer wanted = str;
			size_type wantedLength = strlen(str);

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_type position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[wantedLength - position - 1] == wanted[wantedLength - candidate - 1])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[wantedLength - position - 1] != wanted[wantedLength - candidate - 1])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type 
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = pos + 1;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[wantedLength - position - 1] == *ptr && position == wantedLength - 1)
					return 0;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of the given buffer in this string.
		/// @param str A pointer to the buffer to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of the buffer, or SIZE_T_MAX if none was found.
		size_type rfind(const_pointer str, size_type pos, size_type n) const {
			// Set the actual position to start searching at.
			pos = (pos >= strSize) ? (strSize - 1) : pos;

			const_pointer wanted = str;
			size_type wantedLength = n;

			// Generate the KMP table
			difference_type* table = (difference_type*)AllocMemory((wantedLength + 1) * sizeof(difference_type));
			if(!table)
				throw BadAllocException("Failed to allocate KMP table!");

			table[0] = -1;
			for(size_t i = 1; i != wantedLength + 1; ++i)
				table[i] = 0;

			size_type position = 1;
			difference_type candidate = 0;
			while(position < wantedLength) {
				if(wanted[wantedLength - position - 1] == wanted[wantedLength - candidate - 1])
					table[position] = table[candidate];
				else {
					table[position] = candidate;
					while(candidate >= 0 && wanted[wantedLength - position - 1] != wanted[wantedLength - candidate - 1])
						candidate = table[candidate];
				}

				++position;
				++candidate;
			}
			table[position] = candidate;

			// Look for the wanted string in segments of size_type 
			position = 0;
			const_pointer ptr = strData + pos;
			size_type size = pos + 1;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);
			
				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);
			
				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save an uint32_t segment
				uint32_t segment = *(uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);
			
				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save an uint16_t segment
				uint16_t segment = *(uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX;) {
					if(wanted[wantedLength - position - 1] == segmentPtr[i]) {
						// Move on to the next character
						--i;
						++position;

						// Exit the function if a match for the whole string was found
						if(position == wantedLength) {
							// Free the KMP table
							FreeMemory(table);

							return size + i;
						}
					} else {
						// Start over at the highest match possible
						difference_type signedPosition = table[position];
						if(signedPosition < 0) {
							--i;
							position = 0;
						} else
							position = (size_type)signedPosition;
					}
				}
			}

			// Check if there is a 1 byte region left to look in
			if(size) {
				if(wanted[wantedLength - position - 1] == *ptr && position == wantedLength - 1)
					return 0;
			}

			// No instance of the wanted string was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of the given character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the character, or SIZE_T_MAX if none was found.
		size_type rfind(value_type c, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos >= strSize) ? (strSize - 1) : pos;

			// Use memrchr to find the wanted character
			const_pointer ptr = (const_pointer)memrchr(strData, c, pos + 1);

			// Return the character's position relative to the string's data
			if(ptr)
				return ptr - strData;
		
			// No instance of the wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const string& str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str.strData;
			size_type size = str.strSize;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << (uint8_t)(*ptr) & 7))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const_pointer str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			while(true) {
				// Save a size_type segment
				size_type segment = *(const size_type*)str;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the null termination character was found
					if(!*segmentPtr) {
						// Move the given string pointer up to the null termination character and exit the loop
						str += i;
						break;
					}

					// Add the current character to the appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}
				// Exit the loop if the null termination character was found
				if(!*str)
					break;
			
				// Increment the given string pointer by the size of size_type
				str += sizeof(size_type);
			}

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << (uint8_t)(*ptr) & 7))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of any of the characters in the given buffer in this string.
		/// @param str The buffer whose characters to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const_pointer str, size_type pos, size_type n) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str;
			size_type size = n;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << (uint8_t)(*ptr) & 7))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first instance of the wanted character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(value_type c, size_type pos = 0) const {
			// The find function does the same thing; call that function instead
			return find(c, pos);
		}
		/// @brief Looks for the last instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const string& str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str.strData;
			size_type size = str.strSize;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7)))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const_pointer str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			while(true) {
				// Save a size_type segment
				size_type segment = *(const size_type*)str;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the null termination character was found
					if(!*segmentPtr) {
						// Move the given string pointer up to the null termination character and exit the loop
						str += i;
						break;
					}

					// Add the current character to the appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}
				// Exit the loop if the null termination character was found
				if(!*str)
					break;
			
				// Increment the given string pointer by the size of size_type
				str += sizeof(size_type);
			}

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7)))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of any of the characters in the given buffer in this string.
		/// @param str The buffer whose characters to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const_pointer str, size_type pos, size_type n) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str;
			size_type size = n;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
						return size + i;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7)))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last instance of the wanted character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(value_type c, size_type pos = SIZE_T_MAX) const {
			// The rfind function does the same thing; call that function instead
			return rfind(c, pos);
		}
		/// @brief Looks for the first character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const string& str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str.strData;
			size_type size = str.strSize;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7))))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const_pointer str, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			while(true) {
				// Save a size_type segment
				size_type segment = *(const size_type*)str;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the null termination character was found
					if(!*segmentPtr) {
						// Move the given string pointer up to the null termination character and exit the loop
						str += i;
						break;
					}

					// Add the current character to the appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}
				// Exit the loop if the null termination character was found
				if(!*str)
					break;
			
				// Increment the given string pointer by the size of size_type
				str += sizeof(size_type);
			}

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << (uint8_t)(*ptr) & 7)))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first character in this string that isn't in the given buffer.
		/// @param str The buffer whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const_pointer str, size_type pos, size_type n) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str;
			size_type size = n;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Check every character in the segment
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << (uint8_t)(*ptr) & 7)))
					return strSize - 1;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the first character in this string that isn't the given character.
		/// @param c The character to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(value_type c, size_type pos = 0) const {
			// Assert that the position to start searching from must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = strSize - pos;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return ptr + i - strData;
				
					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type); 
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return ptr + i - strData;
				
					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t); 
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return ptr + i - strData;
				
					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t); 
			}

			// Check if there is a 1 byte region left to look in
			if(size)
				if(*ptr != c)
					return strSize - 1;
		
			// No instance of a wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const string& str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str.strData;
			size_type size = str.strSize;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7))))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const_pointer str, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			while(true) {
				// Save a size_type segment
				size_type segment = *(const size_type*)str;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Check if the null termination character was found
					if(!*segmentPtr) {
						// Move the given string pointer up to the null termination character and exit the loop
						str += i;
						break;
					}

					// Add the current character to the appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}
				// Exit the loop if the null termination character was found
				if(!*str)
					break;
			
				// Increment the given string pointer by the size of size_type
				str += sizeof(size_type);
			}

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7))))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last character in this string that isn't in the given buffer.
		/// @param str The buffer whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const_pointer str, size_type pos, size_type n) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Create an appearance vecotr for every character in the given string
			uint8_t appears[32];
			memset(appears, 0, 32 * sizeof(uint8_t));

			// Look through the given string in segments of size_type
			const_pointer ptr = str;
			size_type size = n;

			while(size >= sizeof(size_type)) {
				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(size_type); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of size_type
				ptr += sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to add
			if(size >= sizeof(uint32_t)) {
				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint32_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint32_t
				ptr += sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
	#endif
			// Check if there is a 2 byte region left to add
			if(size >= sizeof(uint16_t)) {
				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment;

				// Add the characters in the segment to the appearence vector
				for(size_type i = 0; i != sizeof(uint16_t); ++i) {
					// Add the current character to te appearence vector
					appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

					// Increment the segment pointer
					++segmentPtr;
				}

				// Increment the string pointer by the size of uint16_t
				ptr += sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to add
			if(size) {
				appears[(uint8_t)(*ptr) >> 3] |= 1 << ((uint8_t)*ptr & 7);
			}

			// Look through this string in segments of size_type
			ptr = strData + pos;
			size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type);

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t);

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t);

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				// Check every character in the segment
				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if the current character is in the appearence vector
					if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
						return ptr + i - strData;

					// Decrement the segment pointer
					--segmentPtr;
				}
			}
			// Check if there is a 1 byte region left to check in
			if(size) {
				if(!(appears[(uint8_t)(*ptr) >> 3] & (1 << ((uint8_t)(*ptr) & 7))))
					return 0;
			}

			// No instance of any wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Looks for the last character in this string that isn't the given character.
		/// @param c The character to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(value_type c, size_type pos = SIZE_T_MAX) const {
			// Set the actual position to start searching at.
			pos = (pos > strSize) ? strSize : pos;

			// Look through this string in segments of size_type
			const_pointer ptr = strData + pos;
			size_type size = pos;

			while(size >= sizeof(size_type)) {
				// Decrement the string pointer by the size of size_type
				ptr -= sizeof(size_type);

				// Decrement the string's size by the size of size_type
				size -= sizeof(size_type); 

				// Save a size_type segment
				size_type segment = *(const size_type*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(size_type) - 1;

				for(size_type i = sizeof(size_type) - 1; i != SIZE_T_MAX; --i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return size + i;
				
					// Decrement the segment pointer
					--segmentPtr;
				}
			}

	#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to look in
			if(size >= sizeof(uint32_t)) {
				// Decrement the string pointer by the size of uint32_t
				ptr -= sizeof(uint32_t);

				// Decrement the string's size by the size of uint32_t
				size -= sizeof(uint32_t); 

				// Save a uint32_t segment
				uint32_t segment = *(const uint32_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint32_t) - 1;

				for(size_type i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return size + i;
				
					// Dncrement the segment pointer
					--segmentPtr;
				}
			}
	#endif
			// Check if there is a 2 byte region left to look in
			if(size >= sizeof(uint16_t)) {
				// Decrement the string pointer by the size of uint16_t
				ptr -= sizeof(uint16_t);

				// Decrement the string's size by the size of uint16_t
				size -= sizeof(uint16_t); 

				// Save a uint16_t segment
				uint16_t segment = *(const uint16_t*)ptr;

				const_pointer segmentPtr = (const_pointer)&segment + sizeof(uint16_t) - 1;

				for(size_type i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
					// Check if a character different to the wanted character is in this segment
					if(*segmentPtr != c)
						return size + i;
				
					// Decrement the segment pointer
					--segmentPtr;
				}
			}

			// Check if there is a 1 byte region left to look in
			if(size)
				if(*ptr != c)
					return 0;
		
			// No instance of a wanted character was found; return SIZE_T_MAX
			return SIZE_T_MAX;
		}
		/// @brief Generates the wanted substring of this string.
		/// @param pos The starting position of the wanted substring.
		/// @param len The length of the wanted substring.
		/// @return The wanted substring.
		string substr(size_type pos = 0, size_type len = SIZE_T_MAX) const {
			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			return string(strData + pos, len);
		}
		/// @brief Compares this string with the given string.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(const string& str) const {
			// Set the smallest length of both strings		
			size_type minLength = (strSize < str.strSize) ? strSize : str.strSize;		

			// Compare the two strings
			return memcmp(strData, str.strData, minLength + 1);
		}
		/// @brief Compare the wanted substring of this string with the given string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const string& str) const {
			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Set the smallest length of both strings		
			size_type minLength = (len < str.strSize) ? len : str.strSize;		

			// Compare the two strings
			return memcmp(strData + pos, str.strData, minLength + 1);
		}
		/// @brief Compares the wanted substring of this string with the wanted substring of the given string.
		/// @param pos The starting position of this string's substring.
		/// @param len The length of this string's substring.
		/// @param str The string whose substring to compare with.
		/// @param subpos The starting position of the given string's substring.
		/// @param sublen The length of the given string's substring.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) const {
			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(subpos < str.strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			sublen = (sublen > (str.strSize - subpos)) ? (str.strSize - subpos) : sublen;

			// Set the smallest length of both strings		
			size_type minLength = (len < sublen) ? len : sublen;		

			// Compare the two strings
			return memcmp(strData + pos, str.strData + subpos, minLength + 1);
		}
		/// @brief Compares this string with the given string.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(const_pointer str) const {
			// Compare the two strings
			return memcmp(strData, str, strSize + 1);
		}
		/// @brief Compare the wanted substring of this string with the given string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const_pointer str) const {
			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Compare the two strings
			int32_t result = memcmp(strData + pos, str, len);

			if(result)
				return result;

			// Check if the given string is longer than this substring
			if(str[len])
				return -1;
			return 0;
		}
		/// @brief Compares the wanted substring of this string with the given buffer.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The buffer to compare with.
		/// @param n The length of the buffer.
		/// @return An integer representing the relationship between this string and the given buffer: < 0 if this string is smaller than the given buffer, 0 if this string and the given buffer are equal and > 0 if this string is bigger than the given buffer.
		int32_t compare(size_type pos, size_type len, const_pointer str, size_type n) const {
			// Assert that the position to start comparing at must be lower than the string's size
			WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
			// Set the actual length of the substring
			len = (len > (strSize - pos)) ? (strSize - pos) : len;

			// Compare the two strings
			int32_t result = memcmp(strData + pos, str, len);

			if(result)
				return result;

			// Check if the given buffer is longer than this substring
			if(n > len + 1)
				return -1;
		
			// Check if this substring is lenger than the given buffer
			if(n < len)
				return 1;

			return 0;
		}

		/// @brief Destroys the string.
		~string() {
			// Free the string's contents (if they exist)
			if(strData)
				FreeMemory(strData);
		}
	private:
		size_type strSize = 0;
		size_type strCapacity = 0;
		pointer strData = nullptr;
	};

	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(const string&  str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(const string&  str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(      string&& str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(      string&& str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(const string&  str1, const char_t*  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(      string&& str1, const char_t*  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(const char_t*  str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	WFE_INLINE string operator+(const char_t*  str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	/// @brief Appends the given character to the string.
	/// @param str1 The string.
	/// @param str2 The character.
	/// @return The resulting string.
	WFE_INLINE string operator+(const string&  str1,       char_t   str2) {
		string result = str1;
		result.push_back(str2);
		return result;
	}
	/// @brief Appends the given character to the string.
	/// @param str1 The string.
	/// @param str2 The character.
	/// @return The resulting string.
	WFE_INLINE string operator+(      string&& str1,       char_t   str2) {
		string result = str1;
		result.push_back(str2);
		return result;
	}
	/// @brief Places the given character at the front of the string.
	/// @param str1 The character.
	/// @param str2 The string.
	/// @return The resulting string.
	WFE_INLINE string operator+(      char_t   str1, const string&  str2) {
		string result{1, str1};
		result.append(str2);
		return result;
	}
	/// @brief Places the given character at the front of the string.
	/// @param str1 The character.
	/// @param str2 The string.
	/// @return The resulting string.
	WFE_INLINE string operator+(      char_t   str1,       string&& str2) {
		string result{1, str1};
		result.append(str2);
		return result;
	}

	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	WFE_INLINE bool8_t operator==(const string& str1, const string& str2) {
		return !str1.compare(str2);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	WFE_INLINE bool8_t operator==(const string& str1, const char_t* str2) {
		return !str1.compare(str2);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	WFE_INLINE bool8_t operator==(const char_t* str1, const string& str2) {
		return !str2.compare(str1);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	WFE_INLINE bool8_t operator!=(const string& str1, const string& str2) {
		return str1.compare(str2);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	WFE_INLINE bool8_t operator!=(const string& str1, const char_t* str2) {
		return str1.compare(str2);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	WFE_INLINE bool8_t operator!=(const char_t* str1, const string& str2) {
		return str2.compare(str1);
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	WFE_INLINE bool8_t operator< (const string& str1, const string& str2) {
		return str1.compare(str2) < 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	WFE_INLINE bool8_t operator< (const string& str1, const char_t* str2) {
		return str1.compare(str2) < 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	WFE_INLINE bool8_t operator< (const char_t* str1, const string& str2) {
		return str2.compare(str1) > 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	WFE_INLINE bool8_t operator<=(const string& str1, const string& str2) {
		return str1.compare(str2) <= 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	WFE_INLINE bool8_t operator<=(const string& str1, const char_t* str2) {
		return str1.compare(str2) <= 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	WFE_INLINE bool8_t operator<=(const char_t* str1, const string& str2) {
		return str2.compare(str1) >= 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	WFE_INLINE bool8_t operator> (const string& str1, const string& str2) {
		return str1.compare(str2) > 0; 
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	WFE_INLINE bool8_t operator> (const string& str1, const char_t* str2) {
		return str1.compare(str2) > 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	WFE_INLINE bool8_t operator> (const char_t* str1, const string& str2) {
		return str2.compare(str1) < 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	WFE_INLINE bool8_t operator>=(const string& str1, const string& str2) {
		return str1.compare(str2) >= 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	WFE_INLINE bool8_t operator>=(const string& str1, const char_t* str2) {
		return str1.compare(str2) >= 0;
	}
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	WFE_INLINE bool8_t operator>=(const char_t* str1, const string& str2) {
		return str2.compare(str1) <= 0;
	}

	/// @brief Converts the given number to a string.
	/// @param num The number to convert.
	/// @param base The base of the resulting string's number.
	/// @return The string representation of the given number.
	WFE_INLINE string ToString(int64_t num, uint32_t base = 10) {
		// Return a 0 if the number is also 0
		if(!num)
			return "0";

		// Define that characters corresponding for each digit
		const char_t NUMBER_CHARS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		// Create a buffer containing the current string
		char_t buffer[32];
		size_t top = 32;
		
		// Check if the given number is negative and flip the number if so
		bool8_t negative = num < 0;
		if(negative)
			num = -num;

		// Loop through every digit of the given number
		while(num) {
			// Divide the number by the given base and deduce the last digit without using the modulo operator
			int64_t div = num / base;
			int64_t digit = num - div * base;
			num = div;

			// Add the digit to the final string
			buffer[--top] = NUMBER_CHARS[digit];
		}

		// Add a negative sign in front of the number if it is negative
		if(negative)
			buffer[--top] = '-';
		
		// Return the string formed by the characters in the buffer
		return { buffer + top, 32 - top };
	}
	/// @brief Converts the given number to a string.
	/// @param num The number to convert.
	/// @param base The base of the resulting string's number.
	/// @return The string representation of the given number.
	WFE_INLINE string ToString(uint64_t num, uint32_t base = 10) {
		// Return a 0 if the number is also 0
		if(!num)
			return "0";

		// Define that characters corresponding for each digit
		const char_t NUMBER_CHARS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		// Create a buffer containing the current string
		char_t buffer[32];
		size_t top = 32;
		
		// Loop through every digit of the given number
		while(num) {
			// Divide the number by the given base and deduce the last digit without using the modulo operator
			uint64_t div = num / base;
			uint64_t digit = num - div * base;
			num = div;

			// Add the digit to the final string
			buffer[--top] = NUMBER_CHARS[digit];
		}

		// Return the string formed by the characters in the buffer
		return { buffer + top, 32 - top };
	}
	/// @brief Converts the given number to a string.
	/// @param num The number to convert.
	/// @param precision The maximum number of digits of the given number to be written in the resulting string.
	/// @return The string representation of the given number.
	WFE_INLINE string ToString(float32_t num, uint32_t precision = 10) {
		// Create a large enough buffer to store the string.
		char_t buffer[64] = "NaN";

		// Use gcvt to convert the float to a string
		gcvt(num, precision, buffer);

		// Return the new string
		return buffer;
	}
	/// @brief Converts the given number to a string.
	/// @param num The number to convert.
	/// @param precision The maximum number of digits of the given number to be written in the resulting string.
	/// @return The string representation of the given number.
	WFE_INLINE string ToString(float64_t num, uint32_t precision = 10) {
		// Create a large enough buffer to store the string.
		char_t buffer[64] = "NaN";

		// Use gcvt to convert the float to a string
		gcvt(num, precision, buffer);

		// Return the new string
		return buffer;
	}

	/// @brief Converts the first number from the given string to a signed integer.
	/// @param str The string to convert from.
	/// @param outIndex An optional pointer to a size_t variable which will be set to the index of the first character after the number, or to 0 if no conversion was performed.
	/// @param base The base of the number to convert.
	/// @return The resulting integer.
	WFE_INLINE int64_t StrToInt(const char_t* str, size_t* outIndex = nullptr, uint32_t base = 10) {
		// Convert the given string to a int64_t using strtoll
		char* outPtr;
		int64_t val = (int64_t)strtoll(str, &outPtr, (int32_t)base);

		// Write the relative index to the given pointer, if it was set
		if(outIndex)
			*outIndex = (size_t)(outPtr - str);
		
		// Return the final value
		return val;
	}
	/// @brief Converts the first number from the given string to an unsigned integer.
	/// @param str The string to convert from.
	/// @param outIndex An optional pointer to a size_t variable which will be set to the index of the first character after the number, or to 0 if no conversion was performed.
	/// @param base The base of the number to convert.
	/// @return The resulting integer.
	WFE_INLINE uint64_t StrToUint(const char_t* str, size_t* outIndex = nullptr, uint32_t base = 10) {
		// Convert the given string to a int64_t using strtoll
		char* outPtr;
		uint64_t val = (uint64_t)strtoull(str, &outPtr, (int32_t)base);

		// Write the relative index to the given pointer, if it was set
		if(outIndex)
			*outIndex = (size_t)(outPtr - str);
		
		// Return the final value
		return val;
	}
	/// @brief Converts the first number from the given string to a floating point number.
	/// @param str The string to convert from.
	/// @param outIndex An optional pointer to a size_t variable which will be set to the index of the first character after the number, or to 0 if no conversion was performed.
	/// @param base The base of the number to convert.
	/// @return The resulting float.
	WFE_INLINE float32_t StrToFloat(const char_t* str, size_t* outIndex = nullptr) {
		// Convert the given string to a int64_t using strtoll
		char* outPtr;
		float32_t val = (float32_t)strtof(str, &outPtr);

		// Write the relative index to the given pointer, if it was set
		if(outIndex)
			*outIndex = (size_t)(outPtr - str);
		
		// Return the final value
		return val;
	}
	/// @brief Converts the first number from the given string to a double precision floating point number.
	/// @param str The string to convert from.
	/// @param outIndex An optional pointer to a size_t variable which will be set to the index of the first character after the number, or to 0 if no conversion was performed.
	/// @param base The base of the number to convert.
	/// @return The resulting double.
	WFE_INLINE float64_t StrToDouble(const char_t* str, size_t* outIndex = nullptr) {
		// Convert the given string to a int64_t using strtoll
		char* outPtr;
		float64_t val = (float64_t)strtod(str, &outPtr);

		// Write the relative index to the given pointer, if it was set
		if(outIndex)
			*outIndex = (size_t)(outPtr - str);
		
		// Return the final value
		return val;
	}

	/// @brief Formats the given string, using the given args list.
	/// @param dest The target string for the format.
	/// @param maxSize The max size of the target string.
	/// @param format The format to write to the string.
	/// @param args The arguments to use for the format.
	WFE_INLINE void FormatStringArgs(char_t* dest, size_t maxSize, const char_t* format, va_list args) {
		// Call the standard library string format function
		vsnprintf(dest, maxSize, format, args);
	}
	/// @brief Formats the given string.
	/// @param dest The target string for the format.
	/// @param maxSize The max size of the target string.
	/// @param format The format to write to the string.
	WFE_INLINE void FormatString(char_t* dest, size_t maxSize, const char_t* format, ...) {
		// Get the va list
		va_list args;
		va_start(args, format);
		
		// Format the string using the args list
		FormatStringArgs(dest, maxSize, format, args);

		// End the va list
		va_end(args);
	}
}