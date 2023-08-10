#include "Debug.hpp"
#include "Exception.hpp"
#include "Files.hpp"
#include "Memory.hpp"
#include "Thread.hpp"

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

	FileOutput logOutput{};
	Mutex logOutputMutex{};

	// Internal helper functions
	void* WriteToLogFileAsync(void* message) {
		// Lock the log output mutex
		logOutputMutex.Lock();

		// Output the given message to the log file and free the message's memory
		size_t messageLen = strlen((char_t*)message);

		logOutput.WriteBuffer(messageLen, message);
		free(message, MEMORY_USAGE_STRING);

		// Flush the message
		logOutput.Flush();

		// Unlock the log output mutex
		logOutputMutex.Unlock();

		return nullptr;
	}
	static void OutputMessage(LogLevel level, char_t* message) {
		// Output the message to the console
		printf("%s", message);

		// Dispatch a thread to output the message to the log file, if the file was opened
		if(!logOutput.IsOpen())
			return;
		
		// Move the message to the heap
		size_t messageLen = strnlen(message, LOG_LEVEL_NAME_LENGTH + MAX_MESSAGE_LENGTH + 1);
		void* messageMem = malloc(messageLen + 1, MEMORY_USAGE_STRING);
		if(!messageMem)
			throw BadAllocException("Failed to allocate string!");

		memcpy(messageMem, message, messageLen + 1);

		Thread logThread;
		logThread.Begin(WriteToLogFileAsync, message);
		logThread.Detach();
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

	void CreateLogger(const char_t* logFilePath) {
		// Open the file output stream to the given path
		logOutput.Open(logFilePath);
	}
	void DeleteLogger() {
		// Wait for all messages to be logged to the log file
		logOutputMutex.Lock();

		// Close the file output stream
		logOutput.Close();

		// Unlock the log output mutex
		logOutputMutex.Unlock();
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
		char_t fullMessage[LOG_LEVEL_NAME_LENGTH + MAX_MESSAGE_LENGTH + 2];

		memcpy(fullMessage, LOG_LEVEL_NAMES[level], LOG_LEVEL_NAME_LENGTH);
		FormatStringV(fullMessage, MAX_MESSAGE_LENGTH, format, args);

		size_t formattedLen = strnlen(fullMessage, MAX_MESSAGE_LENGTH);
		fullMessage[formattedLen] = '\n';
		fullMessage[formattedLen + 1] = 0;

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