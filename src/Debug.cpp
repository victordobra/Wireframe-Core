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
	const size_t MAX_MESSAGE_LENGTH = 16384;
	const size_t MAX_MESSAGE_QUEUE_LENGTH = 1024;

	const char_t* const LOG_LEVEL_NAMES[] = {
		"[FATAL]:   ",
		"[ERROR]:   ",
		"[WARNING]: ",
		"[INFO]:    ",
		"[DEBUG]:   ",
		"[TRACE]:   "
	};
	const size_t LOG_LEVEL_NAME_LENGTH = 11;

	// Variables
	char_t* messageQueue[MAX_MESSAGE_QUEUE_LENGTH];
	atomic_size_t queueMessageIndex = 0;
	atomic_size_t outputMessageIndex = 0;

	FileOutput logOutput{};
	Thread outputThread{};
	bool8_t outputThreadRunning = true;

	// Internal helper functions
	static void* WriteToLogFileAsync(void* params) {
		// Keep trying to output messages until the thread is stopped
		while(outputThreadRunning) {
			// Loop through all new messages
			while(outputMessageIndex != queueMessageIndex) {
				// Output the current message to the log file
				char_t* message = messageQueue[outputMessageIndex];
				size_t messageLen = strnlen(message, LOG_LEVEL_NAME_LENGTH + MAX_MESSAGE_LENGTH + 1);

				logOutput.WriteBuffer(messageLen, message);
				logOutput.Flush();

				// Free the message memory at the current position
				free(message, MEMORY_USAGE_STRING);

				// Increment the message index
				++outputMessageIndex;
				outputMessageIndex &= MAX_MESSAGE_QUEUE_LENGTH - 1;
			}

			// Wait for new messages to appear
			while(outputMessageIndex == queueMessageIndex && outputThreadRunning)
				sleep(0);
		}

		return nullptr;
	}
	static void OutputMessage(const char_t* message) {
		// Output the message to the console
		printf("%s", message);

		// Copy the current message to the heap
		size_t messageLen = strnlen(message, LOG_LEVEL_NAME_LENGTH + MAX_MESSAGE_LENGTH + 1);
		char_t* messageMem = (char_t*)malloc(messageLen + 1, MEMORY_USAGE_STRING);
		if(!messageMem)
			throw BadAllocException("Failed to allocate string!");
		
		memcpy(messageMem, message, messageLen + 1);

		// Try to write the message to the console
		bool8_t messageWritten = false;
		size_t oldIndex, newIndex;
		while(!messageWritten) {
			// Try to increment the index
			messageWritten = true;
			do {
				// Load the new old index and set the new index
				oldIndex = queueMessageIndex;
				newIndex = (oldIndex + 1) & (MAX_MESSAGE_QUEUE_LENGTH - 1);

				// Exit the loop if the new index is equal to the output index
				if(newIndex == outputMessageIndex) {
					messageWritten = false;
					break;
				}
			} while(!queueMessageIndex.compare_exchange_weak(oldIndex, newIndex));
		}

		// Add the current message to the queue
		messageQueue[oldIndex] = messageMem;
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

		// Run the file output thread
		outputThread.Begin(WriteToLogFileAsync, nullptr);
	}
	void DeleteLogger() {
		// Wait for the file log thread to finish outputting all messages
		outputThreadRunning = false;
		outputThread.Join();

		// Close the file output stream
		logOutput.Close();
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
		FormatStringV(fullMessage + LOG_LEVEL_NAME_LENGTH, MAX_MESSAGE_LENGTH, format, args);

		size_t formattedLen = strnlen(fullMessage, MAX_MESSAGE_LENGTH);
		fullMessage[formattedLen] = '\n';
		fullMessage[formattedLen + 1] = 0;

		// Output the message
		OutputMessage(fullMessage);

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