#pragma once

#include "Defines.hpp"
#include <stdarg.h>
#include <stdio.h>

namespace wfe {
	/// @brief A class that represents an exception throw by the program.
	class Exception {
	public:
		Exception() = delete;
		/// @brief Copies the given exception.
		/// @param other The exception to copy.
		Exception(const Exception& other) = default;
		/// @brief Moves the given exception's contents.
		/// @param other The exception to move.
		Exception(Exception&& other) noexcept = default;
		/// @brief Creates an exception with the given formatted message.
		/// @param format The format to use for the message.
		explicit Exception(const char_t* format, ...) {
			// Get the va list
			va_list args;
			va_start(args, format);

			// Format the message
			vsnprintf(str, MAX_MESSAGE_LEN, format, args);

			// End the va list
			va_end(args);
		}

		/// @brief Copies the given exception's contents into this exception.
		/// @param other The exception to copy.
		/// @return A reference to this exception.
		Exception& operator=(const Exception& other) = default;
		/// @brief Moves the given exception's contents into this exception.
		/// @param other The exception to move.
		/// @return A reference to this exception.
		Exception& operator=(Exception&& other) = default;

		/// @brief Gets the exception's message.
		/// @return The exception's message.
		const char_t* GetMessage() const {
			return str;
		}

		/// @brief Destroys the exception.
		virtual ~Exception() = default;
	protected:
		static const size_t MAX_MESSAGE_LEN = 256;

		char_t str[MAX_MESSAGE_LEN];
	};

	/// @brief A class that represents an exception caused by a failed assertion.
	class AssertException : public Exception {
	public:
		AssertException() = delete;
		/// @brief Copies the given exception.
		/// @param other The exception to copy.
		AssertException(const AssertException& other) = default;
		/// @brief Moves the given exception's contents.
		/// @param other The exception to move.
		AssertException(AssertException&& other) noexcept = default;
		/// @brief Creates an exception with the given formatted message.
		/// @param format The format to use for the message.
		explicit AssertException(const char_t* format, ...) : Exception("") {
			// Get the va list
			va_list args;
			va_start(args, format);

			// Format the message
			vsnprintf(str, MAX_MESSAGE_LEN, format, args);

			// End the va list
			va_end(args);
		}

		/// @brief Copies the given exception's contents into this exception.
		/// @param other The exception to copy.
		/// @return A reference to this exception.
		AssertException& operator=(const AssertException& other) = default;
		/// @brief Moves the given exception's contents into this exception.
		/// @param other The exception to move.
		/// @return A reference to this exception.
		AssertException& operator=(AssertException&& other) = default;

		/// @brief Destroys the exception.
		~AssertException() = default;
	};

/// @brief Asserts an expresion and throws an exception if that assertion is not true.
/// @param cond The condition to check.
/// @param message The message used if the assertion fails.
/// @param file The file name to include with the message.
/// @param line The line to include with the message.
#define WFE_ASSERT_FULL(cond, message, file, line) if(cond) { } else { throw wfe::AssertException("Assertion failure in " file ", line " WFE_MACRO_STRING(line) ": " message); }
/// @brief Asserts an expresion and throws an exception if that assertion is not true. Uses the calling source file name and line.
/// @param cond The condition to check.
/// @param message The message used if the assertion fails.
#define WFE_ASSERT(cond, message) WFE_ASSERT_FULL(cond, message, __FILE__,  __LINE__);

	/// @brief A class that represents an exception caused by a failed memory allocation.
	class BadAllocException : public Exception {
	public:
		BadAllocException() = delete;
		/// @brief Copies the given exception.
		/// @param other The exception to copy.
		BadAllocException(const BadAllocException& other) = default;
		/// @brief Moves the given exception's contents.
		/// @param other The exception to move.
		BadAllocException(BadAllocException&& other) noexcept = default;
		/// @brief Creates an exception with the given formatted message.
		/// @param format The format to use for the message.
		explicit BadAllocException(const char_t* format, ...) : Exception("") {
			// Get the va list
			va_list args;
			va_start(args, format);

			// Format the message
			vsnprintf(str, MAX_MESSAGE_LEN, format, args);

			// End the va list
			va_end(args);
		}

		/// @brief Copies the given exception's contents into this exception.
		/// @param other The exception to copy.
		/// @return A reference to this exception.
		BadAllocException& operator=(const BadAllocException& other) = default;
		/// @brief Moves the given exception's contents into this exception.
		/// @param other The exception to move.
		/// @return A reference to this exception.
		BadAllocException& operator=(BadAllocException&& other) = default;

		/// @brief Destroys the exception.
		~BadAllocException() = default;
	};

	/// @brief A class that represents an exception caused by attempting to acces data that is out of range.
	class OutOfRangeException : public Exception {
	public:
		OutOfRangeException() = delete;
		/// @brief Copies the given exception.
		/// @param other The exception to copy.
		OutOfRangeException(const OutOfRangeException& other) = default;
		/// @brief Moves the given exception's contents.
		/// @param other The exception to move.
		OutOfRangeException(OutOfRangeException&& other) noexcept = default;
		/// @brief Creates an exception with the given formatted message.
		/// @param format The format to use for the message.
		explicit OutOfRangeException(const char_t* format, ...) : Exception("") {
			// Get the va list
			va_list args;
			va_start(args, format);

			// Format the message
			vsnprintf(str, MAX_MESSAGE_LEN, format, args);

			// End the va list
			va_end(args);
		}

		/// @brief Copies the given exception's contents into this exception.
		/// @param other The exception to copy.
		/// @return A reference to this exception.
		OutOfRangeException& operator=(const OutOfRangeException& other) = default;
		/// @brief Moves the given exception's contents into this exception.
		/// @param other The exception to move.
		/// @return A reference to this exception.
		OutOfRangeException& operator=(OutOfRangeException&& other) = default;

		/// @brief Destroys the exception.
		~OutOfRangeException() = default;
	};
}