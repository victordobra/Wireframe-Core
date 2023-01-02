#include "String.hpp"
#include "Memory.hpp"
#include "Debug.hpp"

namespace wfe {
	string::string(const string& other) : strSize(other.strSize), strCapacity(other.strCapacity), strData((pointer)malloc(strCapacity, MEMORY_USAGE_STRING)) {
		// Check if the memory has been allocated successfully
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");
		
		// Copy the string's contents
		memcpy(strData, other.strData, strSize + 1);
	}
	string::string(string&& other) noexcept : strSize(other.strSize), strCapacity(other.strCapacity), strData(other.strData) {
		// Unlink the data from the other string
		other.strData = nullptr;
	}
	string::string(const_pointer str) : strSize(strlen(str)), strCapacity(1), strData(nullptr) {
		// Assert that the string's size must be lower than the maximum size
		WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

		// Set the capacity to the lowest power of 2 higher than the string's size
		for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
			if((strCapacity << step) < strSize)
				strCapacity <<= step;
		strCapacity <<= 1;
		
		// Allocate the string's data
		strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Copy the string's contents
		memcpy(strData, str, strSize + 1);
	}
	string::string(const_pointer ptr, size_type size) : strSize(size), strCapacity(1), strData(nullptr) {
		// Assert that the string's size must be lower than the maximum size
		WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

		// Set the capacity to the lowest power of 2 higher than the string's size
		for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
			if((strCapacity << step) < strSize)
				strCapacity <<= step;
		strCapacity <<= 1;
		
		// Allocate the string's data
		strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Copy the string's contents
		memcpy(strData, ptr, strSize);

		// Place a null termination character at the end
		strData[strSize] = 0;
	}
	string::string(size_type size, value_type c) : strSize(size), strCapacity(1), strData(nullptr) {
		// Assert that the string's size must be lower than the maximum size
		WFE_ASSERT(strSize < max_size(), "The string's size must be lower than max_size()!")

		// Set the capacity to the lowest power of 2 higher than the string's size
		for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
			if((strCapacity << step) < strSize)
				strCapacity <<= step;
		strCapacity <<= 1;
		
		// Allocate the string's data
		strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Set the string's contents
		memset(strData, c, strSize);

		// Place a null termination character at the end
		strData[strSize] = 0;
	}
	string::string(const std::string& str) : strSize(str.size()), strCapacity(str.capacity()), strData((pointer)malloc(strCapacity, MEMORY_USAGE_STRING)) {
		// Check if the memory has been allocated successfully
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Copy the string's contents
		memcpy(strData, str.c_str(), strSize + 1);
	}

	string& string::operator=(const string& other) {
		// If the given string is the same as this string, exit the function.
		if(&other == this)
			return *this;
		
		// Set the new string values
		strSize = other.strSize;

		if(strData)
			strData = (pointer)realloc(strData, strCapacity, other.strCapacity, MEMORY_USAGE_STRING);
		else
			strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);

		strCapacity = other.strCapacity;

		// Check if the memory has been allocated successfully
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Copy the string's contents
		memcpy(strData, other.strData, strSize + 1);

		return *this;
	}
	string& string::operator=(string&& other) noexcept {
		// Delete the string's contents (if they exist)
		if(strData)
			free(strData, strSize, MEMORY_USAGE_STRING);

		// Set the new string's values
		strSize = other.strSize;
		strCapacity = other.strCapacity;
		strData = other.strData;

		// Unlink the data from the other string
		other.strData = nullptr;

		return *this;
	}
	string& string::operator=(const_pointer other) {
		// Calculate the given string's size
		size_type givenStrSize = strlen(other);

		// Resize the string
		resize(givenStrSize);

		// Copy the string's contents
		memcpy(strData, other, strSize + 1);

		return *this;
	}
	string& string::operator=(value_type c) {
		// Resize the string
		resize(1);
		
		// Generate a uint16_t to hold what needs to be added
		uint16_t segment = ((uint8_t)c) << 8;

		// Append the segment to this string
		*(uint16_t*)(strData + strSize - 1) = segment;

		return *this;
	}

	string::operator std::string() const {
		// Create a new string with the contents of this string
		return std::string(strData, strSize);
	}

	string::reference string::operator[](size_type ind) {
		// Assert that the wanted character's index must be lower than the string's size
		WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

		// Return a reference to the wanted character
		return strData[ind];
	}
	string::const_reference string::operator[](size_type ind) const {
		// Assert that the wanted character's index must be lower than the string's size
		WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

		// Return a const reference to the wanted character
		return strData[ind];
	}
	string::reference string::at(size_type ind) {
		// Assert that the wanted character's index must be lower than the string's size
		WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

		// Return a reference to the wanted character
		return strData[ind];
	}
	string::const_reference string::at(size_type ind) const {
		// Assert that the wanted character's index must be lower than the string's size
		WFE_ASSERT(ind < strSize, "The wanted character's index must be lower than the string's size!");

		// Return a const reference to the wanted character
		return strData[ind];
	}
	string::reference string::front() {
		// Return a reference to the first character in the string
		return *strData;
	}
	string::const_reference string::front() const {
		// Return a reference to the first character in the string
		return *strData;
	}
	string::reference string::back() {
		// Return a reference to the last character in the string
		return strData[strSize - 1];
	}
	string::const_reference string::back() const {
		// Return a reference to the last character in the string
		return strData[strSize - 1];
	}

	string& string::operator+=(const string& str) {
		// Save the string's old size
		size_type oldSize = strSize;

		// Resize the string
		resize(strSize + str.strSize);
	
		// Append the given string's contents to this string
		memcpy(strData + oldSize, str.strData, str.strSize + 1);

		return *this;
	}
	string& string::operator+=(string&& str) {
		// Save the string's old size
		size_type oldSize = strSize;

		// Resize the string
		resize(strSize + str.strSize);
	
		// Append the given string's contents to this string
		memcpy(strData + oldSize, str.strData, str.strSize + 1);

		return *this;
	}
	string& string::operator+=(const_pointer str) {
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
	string& string::operator+=(value_type c) {
		// Resize the string
		resize(strSize + 1);
		
		// Add the given character at the end of the string
		strData[strSize - 1] = c;

		// Add a null termination character at the end
		strData[strSize] = 0;

		return *this;
	}
	string& string::append(const string& str) {
		// Save the string's old size
		size_type oldSize = strSize;

		// Resize the string
		resize(strSize + str.strSize);
	
		// Append the given string's contents to this string
		memcpy(strData + oldSize, str.strData, str.strSize + 1);

		return *this;
	}
	string& string::append(const string& str, size_type subpos, size_type sublen) {
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
	string& string::append(const_pointer str) {
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
	string& string::append(const_pointer str, size_type size) {
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
	string& string::append(size_type n, value_type c) {
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
	void string::push_back(value_type c) {
		// Resize the string
		resize(strSize + 1);
		
		// Add the given character at the end of the string
		strData[strSize - 1] = c;

		// Add a null termination character at the end
		strData[strSize] = 0;
	}
	string& string::assign(const string& str) {
		// If the given string is the same as this string, exit the function.
		if(&str == this)
			return *this;
		
		// Set the new string values
		strSize = str.strSize;

		if(strData)
			strData = (pointer)realloc(strData, strCapacity, str.strCapacity, MEMORY_USAGE_STRING);
		else
			strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);

		strCapacity = str.strCapacity;

		// Check if the memory has been allocated successfully
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Copy the string's contents
		memcpy(strData, str.strData, strSize + 1);

		return *this;
	}
	string& string::assign(string&& other) noexcept {
		// Delete the string's contents (if they exist)
		if(strData)
			free(strData, strSize, MEMORY_USAGE_STRING);
		
		// Set the new string's values
		strSize = other.strSize;
		strCapacity = other.strCapacity;
		strData = other.strData;

		// Unlink the data from the other string
		other.strData = nullptr;

		return *this;
	}
	string& string::assign(const string& str, size_type subpos, size_type sublen) {
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
	string& string::assign(const_pointer str) {
		// Calculate the given string's size
		size_type givenStrSize = strlen(str);

		// Resize the string
		resize(givenStrSize);

		// Copy the string's contents
		memcpy(strData, str, strSize + 1);

		return *this;
	}
	string& string::assign(const_pointer str, size_type size) {
		// Resize the string
		resize(size);

		// Copy the string's contents
		memcpy(strData, str, strSize);

		// Place a null termination character at the end
		strData[strSize] = 0;

		return *this;
	}
	string& string::assign(size_type n, value_type c) {
		// Resize the string
		resize(n);

		// Set the string's contents
		memset(strData, c, strSize);

		// Place a null termination character at the end
		strData[strSize] = 0;

		return *this;
	}
	string& string::insert(size_type pos, const string& str) {
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
	string& string::insert(size_type pos, const string& str, size_type subpos, size_type sublen) {
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
	string& string::insert(size_type pos, const_pointer str) {
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
	string& string::insert(size_type pos, const_pointer str, size_type size) {
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
	string& string::insert(size_type pos, size_type n, value_type c) {
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
	string::pointer string::insert(const_pointer pos, size_type n, value_type c) {
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
	string::pointer string::insert(const_pointer pos, value_type c) {
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
	string& string::erase(size_type pos, size_type len) {
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
	string::pointer string::erase(const_pointer pos) {
		// Assert that the position to erase from must be in range
		WFE_ASSERT(pos >= begin() && pos < end(), "The position to erase from must be in range!")

		// Move part of the string backwards to erase the wanted character
		memcpy((pointer)pos, pos + 1, end() - pos + 1);

		// Resize the string
		resize(strSize - 1);

		return (pointer)pos;
	}
	string::pointer string::erase(const_pointer first, const_pointer last) {
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
	string& string::replace(size_type pos, size_type len, const string& str) {
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
	string& string::replace(const_pointer first, const_pointer last, const string& str) {
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
	string& string::replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen) {
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
	string& string::replace(size_type pos, size_type len, const_pointer str) {
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
	string& string::replace(const_pointer first, const_pointer last, const_pointer str) {
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
	string& string::replace(size_type pos, size_type len, const_pointer str, size_type size) {
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
	string& string::replace(const_pointer first, const_pointer last, const_pointer str, size_type size) {
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
	string& string::replace(size_type pos, size_type len, size_type n, value_type c) {
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
	string& string::replace(const_pointer first, const_pointer last, size_type n, value_type c) {
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
	void string::swap(string& str) {
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
	void string::pop_back() {
		// Exit the function if the stirng is empty
		if(!strSize)
			return;
		
		// Resize the string
		resize(strSize - 1);

		// Set the last character to a null termination character
		strData[strSize] = 0;
	}

	string::pointer string::begin() {
		return strData;
	}
	string::const_pointer string::begin() const {
		return strData;
	}
	string::pointer string::end() {
		return strData + strSize;
	}
	string::const_pointer string::end() const {
		return strData + strSize;
	}
	string::const_pointer string::cbegin() const {
		return strData;
	}
	string::const_pointer string::cend() const {
		return strData + strSize;
	}
	
	string::size_type string::size() const {
		return strSize;
	}
	string::size_type string::length() const {
		return strSize;
	}
	string::size_type string::max_size() const {
		return SIZE_T_MAX;
	}
	void string::resize(size_type newSize) {
		// Assert that the given size must be lower than the maximum size
		WFE_ASSERT(newSize < max_size(), "The string's size must be lower than max_size()!")

		// Save the string's old size
		size_type oldSize = strSize;

		// Set the string's new size
		strSize = newSize;

		// Check if memory should be allocated
		if(strCapacity <= strSize) {
			// Save the string's old capacity
			size_type oldCapacity = strCapacity;

			// Find the lowest power of two higher than the string's size
			strCapacity = 1;

			for(size_type step = sizeof(size_type) << 2; step; step >>= 1)
				if((strCapacity << step) < strSize)
					strCapacity <<= step;
			strCapacity <<= 1;

			if(strData) { 
				// Reallocate the string
				strData = (pointer)realloc(strData, oldCapacity, strCapacity, MEMORY_USAGE_STRING);
			} else {
				// Allocate the string
				strData = (pointer)malloc(strCapacity, MEMORY_USAGE_STRING);
			}

			// Check if the memory has been allocated successfully
			if(!strData)
				WFE_LOG_FATAL("Failed to allocate string memory!");
		}

		if(strSize > oldSize) {
			// Add a null termination character at the end
			strData[strSize] = 0;
		}
	}
	void string::resize(size_type newSize, value_type c) {
		// Save the string's old size
		size_type oldSize = strSize;

		// Resize the string
		resize(newSize);

		if(strSize > oldSize) {
			// Fill the new space with the given character
			memset(strData + oldSize, c, strSize - oldSize);
		}
	}
	string::size_type string::capacity() const {
		return strCapacity;
	}
	void string::reserve(size_type newCapacity) {
		// Make the capacity higher than the string's size
		newCapacity = (newCapacity <= strSize) ? (strSize + 1) : newCapacity;

		if(strData) { 
			// Reallocate the string
			strData = (pointer)realloc(strData, strCapacity, newCapacity, MEMORY_USAGE_STRING);
		} else {
			// Allocate the string
			strData = (pointer)malloc(newCapacity, MEMORY_USAGE_STRING);
		}

		// Check if the memory has been allocated successfully
		if(!strData)
			WFE_LOG_FATAL("Failed to allocate string memory!");

		// Set the string's new capacity
		strCapacity = newCapacity;
	}
	void string::clear() {
		if(strData) {
			// Set the string's size to 0
			strSize = 0;

			// Place a null termination character at the beginning
			*strData = 0;
		}
	}
	bool8_t string::empty() const {
		return !strSize;
	}
	void string::shrink_to_fit() {
		// Reserving with a new capacity of 0 will shrink the string's capacity down to the string's size
		reserve();
	}

	string::const_pointer string::c_str() const {
		return strData;
	}
	string::pointer string::data() {
		return strData;
	}
	string::const_pointer string::data() const {
		return strData;
	}
	string::size_type string::copy(pointer str, size_type len, size_type pos) const {
		// Assert that the position to copy from must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to copy from must be lower than the string's size!")

		// Set the actual length of the substring
		len = (len > (strSize - pos)) ? (strSize - pos) : len;

		// Copy to the given buffer
		memcpy(str, strData + pos, len);

		return len;
	}
	string::size_type string::find(const string& str, size_type pos) const {
		// Assert that the position to start searching from must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

		const_pointer wanted = str.strData;
		size_type wantedLength = str.strSize;

		// Generate the KMP table
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
	string::size_type string::find(const_pointer str, size_type pos) const {
		// Assert that the position to start searching from must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

		const_pointer wanted = str;
		size_type wantedLength = strlen(str);

		// Generate the KMP table
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
				}
			}

			// Increment the string pointer by the size of uint16_t
			ptr += sizeof(uint16_t);
			
			// Decrement the string's size by the size of uint16_t
			size -= sizeof(uint16_t);
		}

		// Free the KMP table
		free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

		// Check if there is a 1 byte region left to look in
		if(size) {
			if(wanted[position] == *ptr && position == wantedLength - 1)
				return strSize - 1 - wantedLength;
		}

		// No instance of the wanted string was found; return SIZE_T_MAX
		return SIZE_T_MAX;
	}
	string::size_type string::find(const_pointer str, size_type pos, size_type n) const {
		// Assert that the position to start searching from must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start searching from must be lower than the string's size!")

		const_pointer wanted = str;
		size_type wantedLength = n;

		// Generate the KMP table
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return strSize - size + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						++i;
						++position;
					}
				}
			}

			// Increment the string pointer by the size of uint16_t
			ptr += sizeof(uint16_t);
			
			// Decrement the string's size by the size of uint16_t
			size -= sizeof(uint16_t);
		}

		// Free the KMP table
		free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

		// Check if there is a 1 byte region left to look in
		if(size) {
			if(wanted[position] == *ptr && position == wantedLength - 1)
				return strSize - 1 - wantedLength;
		}

		// No instance of the wanted string was found; return SIZE_T_MAX
		return SIZE_T_MAX;
	}
	string::size_type string::find(value_type c, size_type pos) const {
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
	string::size_type string::rfind(const string& str, size_type pos) const {
		// Set the actual position to start searching at.
		pos = (pos >= strSize) ? (strSize - 1) : pos;

		const_pointer wanted = str.strData;
		size_type wantedLength = str.strSize;

		// Generate the KMP table in reverse
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
	string::size_type string::rfind(const_pointer str, size_type pos) const {
		// Set the actual position to start searching at.
		pos = (pos >= strSize) ? (strSize - 1) : pos;

		const_pointer wanted = str;
		size_type wantedLength = strlen(str);

		// Generate the KMP table in reverse
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
	string::size_type string::rfind(const_pointer str, size_type pos, size_type n) const {
		// Set the actual position to start searching at.
		pos = (pos >= strSize) ? (strSize - 1) : pos;

		const_pointer wanted = str;
		size_type wantedLength = n;

		// Generate the KMP table in reverse
		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		ptrdiff_t position = 1, candidate = 0;
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
						free(table, (wantedLength + 1) * sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);

						return size + i;
					}
				} else {
					// Start over at the highest match possible
					position = table[position];
					if(position < 0) {
						--i;
						++position;
					}
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
	string::size_type string::rfind(value_type c, size_type pos) const {
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
	string::size_type string::find_first_of(const string& str, size_type pos) const {
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
	string::size_type string::find_first_of(const_pointer str, size_type pos) const {
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
	string::size_type string::find_first_of(const_pointer str, size_type pos, size_type n) const {
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
	string::size_type string::find_first_of(value_type c, size_type pos) const {
		// The find function does the same thing; call that function instead
		return find(c, pos);
	}
	string::size_type string::find_last_of(const string& str, size_type pos) const {
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
	string::size_type string::find_last_of(const_pointer str, size_type pos) const {
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
	string::size_type string::find_last_of(const_pointer str, size_type pos, size_type n) const {
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
	string::size_type string::find_last_of(value_type c, size_type pos) const {
		// The rfind function does the same thing; call that function instead
		return rfind(c, pos);
	}
	string::size_type string::find_first_not_of(const string& str, size_type pos) const {
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
	string::size_type string::find_first_not_of(const_pointer str, size_type pos) const {
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
	string::size_type string::find_first_not_of(const_pointer str, size_type pos, size_type n) const {
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
	string::size_type string::find_first_not_of(value_type c, size_type pos) const {
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
	string::size_type string::find_last_not_of(const string& str, size_type pos) const {
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
	string::size_type string::find_last_not_of(const_pointer str, size_type pos) const {
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
	string::size_type string::find_last_not_of(const_pointer str, size_type pos, size_type n) const {
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
	string::size_type string::find_last_not_of(value_type c, size_type pos) const {
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
	string string::substr(size_type pos, size_type len) const {
		// Assert that the position to start comparing at must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
		// Set the actual length of the substring
		len = (len > (strSize - pos)) ? (strSize - pos) : len;

		return string(strData + pos, len);
	}
	int32_t string::compare(const string& str) const {
		// Set the smallest length of both strings		
		size_type minLength = (strSize < str.strSize) ? strSize : str.strSize;		

		// Compare the two strings
		return memcmp(strData, str.strData, minLength + 1);
	}
	int32_t string::compare(size_type pos, size_type len, const string& str) const {
		// Assert that the position to start comparing at must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
		// Set the actual length of the substring
		len = (len > (strSize - pos)) ? (strSize - pos) : len;

		// Set the smallest length of both strings		
		size_type minLength = (len < str.strSize) ? len : str.strSize;		

		// Compare the two strings
		return memcmp(strData + pos, str.strData, minLength + 1);
	}
	int32_t string::compare(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen) const {
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
	int32_t string::compare(const_pointer str) const {
		// Compare the two strings
		return memcmp(strData, str, strSize + 1);
	}
	int32_t string::compare(size_type pos, size_type len, const_pointer str) const {
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
	int32_t string::compare(size_type pos, size_type len, const_pointer str, size_type n) const {
		// Assert that the position to start comparing at must be lower than the string's size
		WFE_ASSERT(pos < strSize, "The position to start comparing at must be lower than the string's size!")
	
		// Set the actual length of the substring
		len = (len > (strSize - pos)) ? (strSize - pos) : len;

		// Set the smallest length of both strings
		size_type minLength = (len + 1 < n) ? (len + 1) : n;

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

	string::~string() {
		// Delete the string's contents (if they exist)
		if(strData)
			free(strData, strSize, MEMORY_USAGE_STRING);
	}

	string operator+(const string&  str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(const string&  str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(      string&& str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(      string&& str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(const string&  str1, const char_t*  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(      string&& str1, const char_t*  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(const char_t*  str1, const string&  str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(const char_t*  str1,       string&& str2) {
		string result = str1;
		result.append(str2);
		return result;
	}
	string operator+(const string&  str1,       char_t   str2) {
		string result = str1;
		result.push_back(str2);
		return result;
	}
	string operator+(      string&& str1,       char_t   str2) {
		string result = str1;
		result.push_back(str2);
		return result;
	}
	string operator+(      char_t   str1, const string&  str2) {
		string result{1, str1};
		result.append(str2);
		return result;
	}
	string operator+(      char_t   str1,       string&& str2) {
		string result{1, str1};
		result.append(str2);
		return result;
	}

	bool8_t operator==(const string& str1, const string& str2) {
		return !str1.compare(str2);
	}
	bool8_t operator==(const string& str1, const char_t* str2) {
		return !str1.compare(str2);
	}
	bool8_t operator==(const char_t* str1, const string& str2) {
		return !str2.compare(str1);
	}
	bool8_t operator!=(const string& str1, const string& str2) {
		return !str1.compare(str2);
	}
	bool8_t operator!=(const string& str1, const char_t* str2) {
		return !str1.compare(str2);
	}
	bool8_t operator!=(const char_t* str1, const string& str2) {
		return !str2.compare(str1);
	}
	bool8_t operator< (const string& str1, const string& str2) {
		return str1.compare(str2) < 0;
	}
	bool8_t operator< (const string& str1, const char_t* str2) {
		return str1.compare(str2) < 0;
	}
	bool8_t operator< (const char_t* str1, const string& str2) {
		return str2.compare(str1) > 0;
	}
	bool8_t operator<=(const string& str1, const string& str2) {
		return str1.compare(str2) <= 0;
	}
	bool8_t operator<=(const string& str1, const char_t* str2) {
		return str1.compare(str2) <= 0;
	}
	bool8_t operator<=(const char_t* str1, const string& str2) {
		return str2.compare(str1) >= 0;
	}
	bool8_t operator> (const string& str1, const string& str2) {
		return str1.compare(str2) > 0; 
	}
	bool8_t operator> (const string& str1, const char_t* str2) {
		return str1.compare(str2) > 0;
	}
	bool8_t operator> (const char_t* str1, const string& str2) {
		return str2.compare(str1) < 0;
	}
	bool8_t operator>=(const string& str1, const string& str2) {
		return str1.compare(str2) >= 0;
	}
	bool8_t operator>=(const string& str1, const char_t* str2) {
		return str1.compare(str2) >= 0;
	}
	bool8_t operator>=(const char_t* str1, const string& str2) {
		return str2.compare(str1) <= 0;
	}
}