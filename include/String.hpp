#include "Defines.hpp"
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
		string(const string& other);
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		string(string&& other) noexcept;
		/// @brief Copies the given string.
		/// @param str The string to copy.
		string(const_pointer str);
		/// @brief Copies from the given buffer.
		/// @param ptr The buffer to copy from.
		/// @param n The size of the given buffer.
		string(const_pointer ptr, size_type size);
		/// @brief Fills the string with the given character.
		/// @param size The wanted length of the string.
		/// @param c The character to fill it with.
		string(size_type size, value_type c);
		/// @brief Copies the given std library string.
		/// @param str The string to copy.
		string(const std::string& str);

		/// @brief Copies the given string.
		/// @param other The string to copy.
		/// @return A reference to this string.
		string& operator=(const string& other);
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		/// @return A reference to this string.
		string& operator=(string&& other) noexcept;
		/// @brief Copies the given string.
		/// @param other The string to copy.
		/// @return A reference to this string.
		string& operator=(const_pointer other);
		/// @brief Sets the string's contents to the given character.
		/// @param c The character to replace the string's contents with.
		/// @return A reference to this string.
		string& operator=(value_type c);

		/// @brief Convers the string to an std library string.
		operator std::string() const;

		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A reference to the wanted character.
		reference operator[](size_type ind);
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A const reference to the wanted character.
		const_reference operator[](size_type ind) const;
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A reference to the wanted character.
		reference at(size_type ind);
		/// @brief Gets the wanted character of the string.
		/// @param ind The index of the wanted character.
		/// @return A const reference to the wanted character.
		const_reference at(size_type ind) const;
		/// @brief Gets the first character of the string.
		/// @return A reference to the first character.
		reference front();
		/// @brief Gets the first character of the string.
		/// @return A const reference to the first character.
		const_reference front() const;
		/// @brief Gets the last character of the string.
		/// @return A reference to the last character.
		reference back();
		/// @brief Gets the last character of the string.
		/// @return A const reference to the last character.
		const_reference back() const;

		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(const string& str);
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(string&& str);
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& operator+=(const_pointer str);
		/// @brief Appends the given character to this string.
		/// @param c The character to append.
		/// @return A reference to this string.
		string& operator+=(value_type c);
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& append(const string& str);
		/// @brief Appends a substring of the given string to the string.
		/// @param str The string whose substring to append.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& append(const string& str, size_type subpos, size_type sublen = SIZE_T_MAX);
		/// @brief Appends the given string to this string.
		/// @param str The string to append.
		/// @return A reference to this string.
		string& append(const_pointer str);
		/// @brief Appends the specified amount of characters from the given character array to this string.
		/// @param str The character array to append from.
		/// @param size The numbers to characters to append from the character array.
		/// @return A reference to this string.
		string& append(const_pointer str, size_type size);
		/// @brief Appends the given character the wanted number of times to this string.
		/// @param n The number of times the character should be appended.
		/// @param c The character to append.
		/// @return A reference to this string.
		string& append(size_type n, value_type c);
		/// @brief Appends the given character to the string.
		/// @param c The character to append.
		void push_back(value_type c);
		/// @brief Copies the given string into this string.
		/// @param str The string to copy from.
		/// @return A reference to this string.
		string& assign(const string& str);
		/// @brief Moves the contents from one string to another.
		/// @param other The string to move from.
		/// @return A reference to this string.
		string& assign(string&& other) noexcept;
		/// @brief Copies a substring of the given string into this string.
		/// @param str The string whose substring to copy from.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& assign(const string& str, size_type subpos, size_type sublen = SIZE_T_MAX);
		/// @brief Copies the given string into this string.
		/// @param str The string to copy from.
		/// @return A reference to this string.
		string& assign(const_pointer str);
		/// @brief Copies the specified amount of characters from the given character array into this string.
		/// @param str The character array to copy from.
		/// @param size The number of characters to copy from the character array.
		/// @return A reference to this string.
		string& assign(const_pointer str, size_type size);
		/// @brief Replaces this string with the specified amount of consecutive copies of the given character.
		/// @param n The number of consecutive characters to replace the string with.
		/// @param c The character to fill the string with.
		/// @return A reference to this string.
		string& assign(size_type n, value_type c);
		/// @brief Inserts the given string into this string.
		/// @param pos The position to insert the stirng at.
		/// @param str The string to insert.
		/// @return A reference to this string.
		string& insert(size_type pos, const string& str);
		/// @brief Inserts a substring of the given string into this string.
		/// @param pos The position to insert the substring at.
		/// @param str The string whose substring to insert.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& insert(size_type pos, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX);
		/// @brief Inserts the given string into this string.
		/// @param pos The position to insert the stirng at.
		/// @param str The string to insert.
		/// @return A reference to this string.
		string& insert(size_type pos, const_pointer str);
		/// @brief Inserts the specified amount of characters from the given character array into this string.
		/// @param pos The position to insert the character from the array at.
		/// @param str The character array to insert from.
		/// @param size The number of character to insert from the character array.
		/// @return A reference to this string.
		string& insert(size_type pos, const_pointer str, size_type size);
		/// @brief Inserts the specified amount of consecutive copies of this given character into this string.
		/// @param pos The position to insert the characters at.
		/// @param n The number of consecutive characters to replace the string with.
		/// @param c The character to fill the string with.
		/// @return A reference to this string.
		string& insert(size_type pos, size_type n, value_type c);
		/// @brief Inserts the specified amount of consecutive copies of this given character into this string.
		/// @param pos The position to insert the characters at.
		/// @param n The number of consecutive characters to insert into this string.
		/// @param c The number of characters to insert.
		/// @return A pointer to the first character inserted.
		pointer insert(const_pointer pos, size_type n, value_type c);
		/// @brief Insert the given character into this string.
		/// @param pos The position to insert the character at.
		/// @param c The character to insert.
		/// @return A pointer to the first character inserted.
		pointer insert(const_pointer pos, value_type c);
		/// @brief Erases the given substring from this string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @return A reference to this string.
		string& erase(size_type pos = 0, size_type len = SIZE_T_MAX);
		/// @brief Erases the given character.
		/// @param ptr A pointer to the character to be erased.
		/// @return A pointer to the character that now occupies this character's position.
		pointer erase(const_pointer pos);
		/// @brief Erases every character of the given interval.
		/// @param first A pointer to the first character to be erased.
		/// @param last A pointer right after the last character to be erased.
		/// @return A pointer to the character that now occupies the first erased character's position.
		pointer erase(const_pointer first, const_pointer last);
		/// @brief Replaces the given region of this string with the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const string& str);
		/// @brief Replaces the region between the two given pointer with the given string.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const string& str);
		/// @brief Replaces the given region to this string with a substring of the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string whose substring to replace the region with.
		/// @param subpos The starting position of the substring.
		/// @param sublen The length of the substring.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX);
		/// @brief Replaces the given region of this string with the given string.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const_pointer str);
		/// @brief Replaces the region between the two given pointer with the given string.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The string to replace the region with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const_pointer str);
		/// @brief Replaces the given region of this string with characters from the given character array.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param str The character whose elements to replace the region with.
		/// @param size The number of characters to replace the region with from the character array.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, const_pointer str, size_type size);
		/// @brief Replaces the region between the two given pointers with characters from the given character array.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param str The character whose elements to replace the region with.
		/// @param size The number of characters to replace the region with from the character array.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, const_pointer str, size_type size);
		/// @brief Replace the given region of this string with the given number of consecutive copies of the given character.
		/// @param pos The starting position of the region to replace.
		/// @param len The length of the region to erase.
		/// @param n The number of consecutive copies of the given character to replace the region with.
		/// @param c The character whose copies to replace the string with.
		/// @return A reference to this string.
		string& replace(size_type pos, size_type len, size_type n, value_type c);
		/// @brief Replaces the region between the two given pointers with the given number of consecutive copies of the given character.
		/// @param first A pointer to the first character of the region.
		/// @param last A pointer right after the last character of the region.
		/// @param n The number of consecutive copies of the given character to replace the region with.
		/// @param c The character whose copies to replace the string with.
		/// @return A reference to this string.
		string& replace(const_pointer first, const_pointer last, size_type n, value_type c);
		/// @brief Swaps the two string's contents.
		/// @param str The string to swap with.
		void swap(string& str);
		/// @brief Removes the last element of the string.
		void pop_back();

		/// @brief Gets the beginning of the string.
		/// @return A pointer to the beginning of the string.
		pointer begin();
		/// @brief Gets the beginning of the string.
		/// @return A pointer to the beginning of the string.
		const_pointer begin() const;
		/// @brief Gets the end of the string.
		/// @return A pointer to the end of the string.
		pointer end();
		/// @brief Gets the end of the string.
		/// @return A pointer to the end of the string.
		const_pointer end() const;
		/// @brief Gets the const beginning of the string.
		/// @return A const pointer to the beginning of the string.
		const_pointer cbegin() const;
		/// @brief Gets the const end of the string.
		/// @return A const pointer to the end of the string.
		const_pointer cend() const;

		/// @brief Gets the length of the string.
		/// @return The length of the string.
		size_type size() const;
		/// @brief Gets the length of the string.
		/// @return The length of the string.
		size_type length() const;
		/// @brief Gets the maximum possible size of the string.
		/// @return The max length of the string.
		size_type max_size() const;
		/// @brief Resizes the string.
		/// @param newSize The new size of the string.
		void resize(size_type newSize);
		/// @brief Resizes the string, filling it with the given character.
		/// @param newSize The new size of the string.
		/// @param c The character to fill the string with.
		void resize(size_type newSize, value_type c);
		/// @brief Gets the capacity of the string.
		/// @return The new capacity of the string.
		size_type capacity() const;
		/// @brief Changes the string's capacity.
		/// @param newCapacity The new capacity of the string.
		void reserve(size_type newCapacity = 0);
		/// @brief Clears the string.
		void clear();
		/// @brief Tests if the string is empty.
		/// @return True if the string is empry, or false otherwise.
		bool8_t empty() const;
		/// @brief Reduces the string's capacity to fit its size.
		void shrink_to_fit();

		/// @brief Gets this string's C string.
		/// @return A const pointer to the C string.
		const_pointer c_str() const;
		/// @brief Gets a pointer to this string's data.
		/// @return A pointer to this string's data.
		pointer data();
		/// @brief Gets a pointer to this string's data.
		/// @return A const pointer to this string's data.
		const_pointer data() const;
		/// @brief Copies the wanted substring of this string into the given buffer.
		/// @param str The buffer to copy info.
		/// @param len The length of the substring.
		/// @param pos The starting position of the substring.
		/// @return The number of characters that have been copied.
		size_type copy(pointer str, size_type len, size_type pos = 0) const;
		/// @brief Looks for the first instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the string, or SIZE_T_MAX if none was found.
		size_type find(const string& str, size_type pos = 0) const;
		/// @brief Looks for the first instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the string, or SIZE_T_MAX if none was found.
		size_type find(const_pointer str, size_type pos = 0) const;
		/// @brief Looks for the first instance of the given buffer in this string.
		/// @param str A pointer to the buffer to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of the buffer, or SIZE_T_MAX if none was found.
		size_type find(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the first instance of the given character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the character, or SIZE_T_MAX if none was found.
		size_type find(value_type c, size_type pos = 0) const;
		/// @brief Looks for the last instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the string, or SIZE_T_MAX if none was found.
		size_type rfind(const string& str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last instance of the given string in this string.
		/// @param str The string to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the string, or SIZE_T_MAX if none was found.
		size_type rfind(const_pointer str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last instance of the given buffer in this string.
		/// @param str A pointer to the buffer to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of the buffer, or SIZE_T_MAX if none was found.
		size_type rfind(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the last instance of the given character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the character, or SIZE_T_MAX if none was found.
		size_type rfind(value_type c, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the first instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const string& str, size_type pos = 0) const;
		/// @brief Looks for the first instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const_pointer str, size_type pos = 0) const;
		/// @brief Looks for the first instance of any of the characters in the given buffer in this string.
		/// @param str The buffer whose characters to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the first instance of the wanted character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of the wanted character, or SIZE_T_MAX if none was found.
		size_type find_first_of(value_type c, size_type pos = 0) const;
		/// @brief Looks for the last instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const string& str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last instance of any of the characters in the given string in this string.
		/// @param str The string whose characters to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const_pointer str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last instance of any of the characters in the given buffer in this string.
		/// @param str The buffer whose characters to look for.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of a wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the last instance of the wanted character in this string.
		/// @param c The character to look for.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of the wanted character, or SIZE_T_MAX if none was found.
		size_type find_last_of(value_type c, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the first character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const string& str, size_type pos = 0) const;
		/// @brief Looks for the first character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const_pointer str, size_type pos = 0) const;
		/// @brief Looks for the first character in this string that isn't in the given buffer.
		/// @param str The buffer whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the first character in this string that isn't the given character.
		/// @param c The character to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the first instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_first_not_of(value_type c, size_type pos = 0) const;
		/// @brief Looks for the last character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const string& str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last character in this string that isn't in the given string.
		/// @param str The string whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const_pointer str, size_type pos = SIZE_T_MAX) const;
		/// @brief Looks for the last character in this string that isn't in the given buffer.
		/// @param str The buffer whose characters to avoid.
		/// @param pos The position to start searching at.
		/// @param n The size of the given buffer.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(const_pointer str, size_type pos, size_type n) const;
		/// @brief Looks for the last character in this string that isn't the given character.
		/// @param c The character to avoid.
		/// @param pos The position to start searching at.
		/// @return The position of the last instance of a character that isn't unwanted, or SIZE_T_MAX if none was found.
		size_type find_last_not_of(value_type c, size_type pos = SIZE_T_MAX) const;
		/// @brief Generates the wanted substring of this string.
		/// @param pos The starting position of the wanted substring.
		/// @param len The length of the wanted substring.
		/// @return The wanted substring.
		string substr(size_type pos = 0, size_type len = SIZE_T_MAX) const;
		/// @brief Compares this string with the given string.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(const string& str) const;
		/// @brief Compare the wanted substring of this string with the given string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const string& str) const;
		/// @brief Compares the wanted substring of this string with the wanted substring of the given string.
		/// @param pos The starting position of this string's substring.
		/// @param len The length of this string's substring.
		/// @param str The string whose substring to compare with.
		/// @param subpos The starting position of the given string's substring.
		/// @param sublen The length of the given string's substring.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const string& str, size_type subpos, size_type sublen = SIZE_T_MAX) const;
		/// @brief Compares this string with the given string.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(const_pointer str) const;
		/// @brief Compare the wanted substring of this string with the given string.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The string to compare with.
		/// @return An integer representing the relationship between the two strings: < 0 if this string is smaller than the given string, 0 if the two strings are equal and > 0 if this string is bigger than the given string.
		int32_t compare(size_type pos, size_type len, const_pointer str) const;
		/// @brief Compares the wanted substring of this string with the given buffer.
		/// @param pos The starting position of the substring.
		/// @param len The length of the substring.
		/// @param str The buffer to compare with.
		/// @param n The length of the buffer.
		/// @return An integer representing the relationship between this string and the given buffer: < 0 if this string is smaller than the given buffer, 0 if this string and the given buffer are equal and > 0 if this string is bigger than the given buffer.
		int32_t compare(size_type pos, size_type len, const_pointer str, size_type n) const;

		/// @brief Deletes the string.
		~string();
	private:
		size_type strSize = 0;
		size_type strCapacity = 0;
		pointer strData = nullptr;
	};

	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(const string&  str1, const string&  str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(const string&  str1,       string&& str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(      string&& str1, const string&  str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(      string&& str1,       string&& str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(const string&  str1, const char_t*  str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(      string&& str1, const char_t*  str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(const char_t*  str1, const string&  str2);
	/// @brief Concatenates the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return The resulting string.
	string operator+(const char_t*  str1,       string&& str2);
	/// @brief Appends the given character to the string.
	/// @param str1 The string.
	/// @param str2 The character.
	/// @return The resulting string.
	string operator+(const string&  str1,       char_t   str2);
	/// @brief Appends the given character to the string.
	/// @param str1 The string.
	/// @param str2 The character.
	/// @return The resulting string.
	string operator+(      string&& str1,       char_t   str2);
	/// @brief Places the given character at the front of the string.
	/// @param str1 The character.
	/// @param str2 The string.
	/// @return The resulting string.
	string operator+(      char_t   str1, const string&  str2);
	/// @brief Places the given character at the front of the string.
	/// @param str1 The character.
	/// @param str2 The string.
	/// @return The resulting string.
	string operator+(      char_t   str1,       string&& str2);

	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	bool8_t operator==(const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	bool8_t operator==(const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if both strings are equal, otherwise false.
	bool8_t operator==(const char_t* str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	bool8_t operator!=(const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	bool8_t operator!=(const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the string are different, otherwise false.
	bool8_t operator!=(const char_t* str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	bool8_t operator< (const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	bool8_t operator< (const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than the second, otherwise false.
	bool8_t operator< (const char_t* str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	bool8_t operator<=(const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	bool8_t operator<=(const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is smaller than to equal to the second, otherwise false.
	bool8_t operator<=(const char_t* str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	bool8_t operator> (const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	bool8_t operator> (const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than the second, otherwise false.
	bool8_t operator> (const char_t* str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	bool8_t operator>=(const string& str1, const string& str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	bool8_t operator>=(const string& str1, const char_t* str2);
	/// @brief Compares the two given strings.
	/// @param str1 The first string.
	/// @param str2 The second string.
	/// @return True if the first string is bigger than or equal to the second, otherwise false.
	bool8_t operator>=(const char_t* str1, const string& str2);
}