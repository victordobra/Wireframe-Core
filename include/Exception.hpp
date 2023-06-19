#pragma once

#include "String.hpp"

namespace wfe {
    /// @brief A struct that represents an exception throw by the program.
    struct Exception {
    public:
        Exception() = delete;
        /// @brief Copies the given exception.
        /// @param other The exception to copy.
        Exception(const Exception& other) = default;
        /// @brief Moves the given exception's contents.
        /// @param other The exception to move.
        Exception(Exception&& other) noexcept = default;
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit Exception(const string& message) : message(message) { }
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit Exception(const char_t* message) : message(message) { }

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
        string GetMessage() {
            return message;
        }

        /// @brief Deletes the exception.
        virtual ~Exception() = default;
    protected:
        string message;
    };

    /// @brief A struct that represents an exception caused by a failed memory allocation.
    struct BadAllocException : public Exception {
    public:
        BadAllocException() = delete;
        /// @brief Copies the given exception.
        /// @param other The exception to copy.
        BadAllocException(const BadAllocException& other) = default;
        /// @brief Moves the given exception's contents.
        /// @param other The exception to move.
        BadAllocException(BadAllocException&& other) noexcept = default;
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit BadAllocException(const string& message) : Exception(message) { }
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit BadAllocException(const char_t* message) : Exception(message) { }

        /// @brief Copies the given exception's contents into this exception.
        /// @param other The exception to copy.
        /// @return A reference to this exception.
        BadAllocException& operator=(const BadAllocException& other) = default;
        /// @brief Moves the given exception's contents into this exception.
        /// @param other The exception to move.
        /// @return A reference to this exception.
        BadAllocException& operator=(BadAllocException&& other) = default;

        /// @brief Deletes the exception.
        ~BadAllocException() = default;
    };

    /// @brief A struct that represents an exception caused by attempting to acces data that is out of range.
    struct OutOfRangeException : public Exception {
    public:
        OutOfRangeException() = delete;
        /// @brief Copies the given exception.
        /// @param other The exception to copy.
        OutOfRangeException(const OutOfRangeException& other) = default;
        /// @brief Moves the given exception's contents.
        /// @param other The exception to move.
        OutOfRangeException(OutOfRangeException&& other) noexcept = default;
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit OutOfRangeException(const string& message) : Exception(message) { }
        /// @brief Creates an exception with the given message.
        /// @param message The exception's message.
        explicit OutOfRangeException(const char_t* message) : Exception(message) { }

        /// @brief Copies the given exception's contents into this exception.
        /// @param other The exception to copy.
        /// @return A reference to this exception.
        OutOfRangeException& operator=(const OutOfRangeException& other) = default;
        /// @brief Moves the given exception's contents into this exception.
        /// @param other The exception to move.
        /// @return A reference to this exception.
        OutOfRangeException& operator=(OutOfRangeException&& other) = default;

        /// @brief Deletes the exception.
        ~OutOfRangeException() = default;
    };
}