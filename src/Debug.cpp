#include "Debug.hpp"
#include "Exception.hpp"
#include "Files.hpp"
#include "Memory.hpp"
#include "Thread.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

namespace wfe {
	// Constants
	static const size_t MAX_MESSAGE_LENGTH = 16384;

	// Public functions
	const char_t* Logger::GetLogLevelString(LogLevel level) {
		switch(level) {
		case LOG_LEVEL_DEBUG:
			return "[LOG_DEBUG]:   ";
		case LOG_LEVEL_INFO:
			return "[LOG_INFO]:    ";
		case LOG_LEVEL_WARNING:
			return "[LOG_WARNING]: ";
		case LOG_LEVEL_ERROR:
			return "[LOG_ERROR]:   ";
		case LOG_LEVEL_FATAL:
			return "[LOG_FATAL]:   ";
		default:
			return "[LOG]:         ";
		};
	}

	Logger::Logger(const char_t* logFilePath, bool8_t outputConsole, LogLevelFlags logLevelFlags) : outputConsole(outputConsole), logLevelFlags(logLevelFlags), messages() {
		// Check if a log file path is given
		if(logFilePath) {
			// Try to open the file
			fileOutput.Open(logFilePath);
		}
	}

	void Logger::LogMessage(LogLevel level, const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(level, format, args);

		// End the args list
		va_end(args);
	}

	void Logger::LogMessageArgs(LogLevel level, const char_t* format, va_list args) {
		// Exit the function if the message level is not in the logger's mask
		if(!(level & logLevelFlags))
			return;

		// Format the message
		char_t message[MAX_MESSAGE_LENGTH];
		FormatStringArgs(message, MAX_MESSAGE_LENGTH, format, args);

		// Create the new message and add it to the message list
		messages.emplace_back(level, message); 
	
		// Print the message to the file output stream, if enabled
		if(fileOutput.IsOpen()) {
			// Create an output string array and print the full message
			const char_t* strings[3] { GetLogLevelString(level), message, "\n" };
			fileOutput.Write(3, strings, "");

			// Flush the log file
			fileOutput.Flush();
		}

		// Print the message to the console, if enabled
		if(outputConsole) {
			// Print the log level and the message
			fputs(GetLogLevelString(level), stdout);
			fputs(message, stdout);
			fputc('\n', stdout);
		}

		// Abort the program if the current message is a fatal error
		if(level == LOG_LEVEL_FATAL)
			abort();
	}

	void Logger::LogDebugMessage(const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(LOG_LEVEL_DEBUG, format, args);

		// End the args list
		va_end(args);
	}
	void Logger::LogInfoMessage(const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(LOG_LEVEL_INFO, format, args);

		// End the args list
		va_end(args);
	}
	void Logger::LogWarningMessage(const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(LOG_LEVEL_WARNING, format, args);

		// End the args list
		va_end(args);
	}
	void Logger::LogErrorMessage(const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(LOG_LEVEL_ERROR, format, args);

		// End the args list
		va_end(args);
	}
	void Logger::LogFatalMessage(const char_t* format, ...) {
		// Get the args list
		va_list args;
		va_start(args, format);

		// Output the message using the given args
		LogMessageArgs(LOG_LEVEL_FATAL, format, args);

		// End the args list
		va_end(args);
	}

	size_t Logger::GetMessageCount() const {
		return messages.size();
	}
	const Logger::Message* Logger::GetMessages() const {
		return messages.data();
	}
	void Logger::ClearMessages() {
		messages.clear();
	}

	Logger::~Logger() {
		// CLose the output file stream
		fileOutput.Close();
	}
}