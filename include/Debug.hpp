#pragma once

#include "Defines.hpp"
#include "Files.hpp"
#include "Vector.hpp"
#include <stdarg.h>

namespace wfe {
	/// @brief A logger that outputs messages to a file or the console.
	class Logger {
	public:
		/// @brief The severity level of a message.
		typedef enum {
			/// @brief A bitmask containing no log levels.
			LOG_LEVEL_NONE = 0x00,
			/// @brief The level of a debug message useful for debugging.
			LOG_LEVEL_DEBUG = 0x01,
			/// @brief The level of an info message that may give the user important information.
			LOG_LEVEL_INFO = 0x02,
			/// @brief The level of a warning message that may indicate incorrect program behaviour.
			LOG_LEVEL_WARNING = 0x04,
			/// @brief The level of an error message that indicates incorrect program behaviour.
			LOG_LEVEL_ERROR = 0x08,
			/// @brief The level of a fatal message that instantly closes the program.
			LOG_LEVEL_FATAL = 0x10,
			/// @brief A bitmask containing all log levels.
			LOG_LEVEL_ALL = LOG_LEVEL_DEBUG | LOG_LEVEL_INFO | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL
		} LogLevel;
		/// @brief The type used for log level type flags.
		typedef uint32_t LogLevelFlags;

		/// @brief A struct containing the required info about a logged message.
		struct Message {
			/// @brief The severity level of the message.
			LogLevel level;
			/// @brief The message string.
			string message;
		};

		/// @brief Converts the given log level to the corresponding output string.
		/// @param level The log level to convert.
		/// @return The corresponding output string.
		static const char_t* GetLogLevelString(LogLevel level);

		/// @brief Creates a debug logger.
		/// @param logFilePath The path of the log output file, or nullptr if file output won't be used.
		/// @param outputConsole True if messages will be logged to the console, otherwise false.
		/// @param logLevelFlags A bitmask signifying which log levels will be written by the logger. Defaulted to LOG_LEVEL_ALL.
		Logger(const char_t* logFilePath, bool8_t outputConsole, LogLevelFlags logLevelFlags = LOG_LEVEL_ALL);
		Logger(const Logger&) = delete;
		Logger(Logger&&) noexcept = delete;

		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;

		/// @brief Logs a message.
		/// @param level The message level.
		/// @param format The message string format.
		void LogMessage(LogLevel level, const char_t* format, ...);
		/// @brief Logs a message, using the given args list to format it.
		/// @param level The message level.
		/// @param format The message string format.
		/// @param args The message string format args.
		void LogMessageArgs(LogLevel level, const char_t* format, va_list args);

		/// @brief Logs a debug level message.
		/// @param format The message string format.
		void LogDebugMessage(const char_t* format, ...);
		/// @brief Logs an info level message.
		/// @param format The message string format.
		void LogInfoMessage(const char_t* format, ...);
		/// @brief Logs a warning level message.
		/// @param format The message string format.
		void LogWarningMessage(const char_t* format, ...);
		/// @brief Logs an error level message.
		/// @param format The message string format.
		void LogErrorMessage(const char_t* format, ...);
		/// @brief Logs a fatal error level message.
		/// @param format The message string format.
		void LogFatalMessage(const char_t* format, ...);
	
		/// @brief Gets the number of messages logged by the current logger.
		/// @return The number of messages logged by the current logger.
		size_t GetMessageCount() const;
		/// @brief Gets the list of messages logged by the current logger.
		/// @return A const pointer to an array containing the logged messages.
		const Message* GetMessages() const;
		/// @brief Clears the logger's message list.
		void ClearMessages();

		/// @brief Destroys the debug logger.
		~Logger();
	private:
		FileOutput fileOutput;
		bool8_t outputConsole;
		LogLevelFlags logLevelFlags;

		vector<Message> messages;
	};
}