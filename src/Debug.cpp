#include "Debug.hpp"
#include "Memory.hpp"

#include <stdlib.h>
#include <stdio.h>

namespace wfe {
	// Constants
	const size_t MAX_MESSAGE_LENGTH = 16384;

	const char_t* LOG_LEVEL_NAMES[] = {
		"[FATAL]:   ",
		"[ERROR]:   ",
		"[WARNING]: ",
		"[INFO]:    ",
		"[DEBUG]:   ",
		"[TRACE]:   "
	};
	const size_t LOG_LEVEL_NAME_LENGTH = 11;

	// Internal helper functions
	void OutputMessage(LogLevel level, const char_t* message) {
		// Only print the message for now
		printf("%s\n", message);
	}

	// External functions
	void FormatString(char_t* dest, size_t maxSize, const char_t* format, ...) {
		// Get the va list
		va_list args;
		va_start(args, format);
		
		// Format the string using the va list
		FormatStringV(dest, maxSize, format, args);

		// End the va list
		va_end(args);
	}
	void FormatStringV(char_t* dest, size_t maxSize, const char_t* format, va_list args) {
		// Call the standard library string format function
		vsnprintf(dest, maxSize, format, args);
	}

	void CreateLogger() {
		// TODO: Add output log file and separate thread to output on.
	}
	void DeleteLogger() {
		// TODO: Remove file and file output thread once those are done.
	}

	void LogMessage(LogLevel level, const char_t* format, ...) {
		// Get the va list
		va_list args;
		va_start(args, format);
		
		// Output the message using the va list
		LogMessageV(level, format, args);

		// End the va list
		va_end(args);
	}
	void LogMessageV(LogLevel level, const char_t* format, va_list args) {
		// Format the message string
		char_t formattedMessage[MAX_MESSAGE_LENGTH + 1];
		FormatStringV(formattedMessage, MAX_MESSAGE_LENGTH, format, args);

		// Generate the full message string
		char_t fullMessage[MAX_MESSAGE_LENGTH + LOG_LEVEL_NAME_LENGTH + 1];
		strncpy(fullMessage, LOG_LEVEL_NAMES[level], LOG_LEVEL_NAME_LENGTH + 1);
		strncpy(fullMessage + LOG_LEVEL_NAME_LENGTH, formattedMessage, MAX_MESSAGE_LENGTH);

		// Output the message
		OutputMessage(level, fullMessage);

		// Abort the program if the error was fatal
		if(level == LOG_LEVEL_FATAL) {
			DeleteLogger();
			abort();
		}
	}

	void ReportAssertionFailure(const char_t* expression, const char_t* message, const char_t* file, size_t line) {
		// Output a formatted message
		LogMessage(LOG_LEVEL_FATAL, "Assertion of \"%s\" failed with message \"%s\" in file %s at line %llu!", expression, message, file, (unsigned long long)line);
	}
}