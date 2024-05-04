#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Thread.hpp"
#include "Memory.hpp"
#include "UnorderedMap.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	// Internal structs
	struct ThreadParams {
		Thread::ThreadFunction func;
		void* params;
	};

	// Internal variables
	static atomic_uint64_t threadResultBitmask = UINT64_T_MAX;
	static void* threadResults[64];

	DWORD WINAPI ThreadFunctionWrapper(LPVOID voidParams) {
		// Convert the given params
		ThreadParams* params = (ThreadParams*)voidParams;

		// Call the given function using the given params
		void* result = params->func(params->params);

		// Free the given thread params
		FreeMemory(params);

		// Try to find a free thread result array index
		size_t threadResultIndex = 0;
		bool8_t threadResultIndexFound = false;
		while(!threadResultIndexFound) {
			// Try to find a index for the current thread result
			threadResultIndexFound = true;
			uint64_t oldBitmask, newBitmask;
			do {
				// Load the old bitmask
				oldBitmask = threadResultBitmask;

				// Exit the loop if the old bitmask is full
				if(!oldBitmask) {
					threadResultIndexFound = false;
					break;
				}

				// Calculate the thread result index and new thread bitmask
				threadResultIndex = Pow2BitIndex(RightmostBit(oldBitmask));
				newBitmask = oldBitmask & ~(1 << threadResultIndex);
			} while(!threadResultBitmask.compare_exchange_weak(oldBitmask, newBitmask));
		}

		// Store the thread result in the thread result array
		threadResults[threadResultIndex] = result;

		// Return the thread result index
		return (DWORD)threadResultIndex;
	}

	bool8_t Thread::operator==(const Thread& other) const {
		// Compare the two thread's IDs
		return threadID == other.threadID;
	}
	bool8_t Thread::operator!=(const Thread& other) const {
		// Compare the two thread's IDs
		return threadID != other.threadID;
	}

	Thread::ThreadID Thread::GetID() const {
		// Get the thread's ID using GetThreadId
		return threadID;
	}
	void* Thread::GetInternalData() {
		return internalData;
	}

	Thread::ThreadResult Thread::Begin(ThreadFunction func, void* args) {
		// Check if the thread was already opened
		if(internalData)
			return ERROR_THREAD_ALREADY_BEGUN;

		// Create another variable to save the thread ID in
		DWORD threadIDWord;

		// Allocate the current thread's params
		PushMemoryUsageType(MEMORY_USAGE_TYPE_COMMAND);
		ThreadParams* params = (ThreadParams*)AllocMemory(sizeof(ThreadParams));
		PopMemoryUsageType();

		// Check if the thread params allocated correctly
		if(!params)
			return ERROR_FAILED_HEAP_ALLOCATION;

		params->func = func;
		params->params = args;

		// Create the thread using CreateThread
		internalData = (void*)CreateThread(NULL, 0, ThreadFunctionWrapper, params, 0, &threadIDWord);

		// Set the thread's new ID
		threadID = (ThreadID)threadIDWord;

		// Check if the thread was created successfully
		if(!internalData) {
			// Convert the Win32 error code to a thread error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_NOT_ENOUGH_MEMORY:
			case ERROR_OUTOFMEMORY:
			case ERROR_DS_THREAD_LIMIT_EXCEEDED:
				return ERROR_INSUFFICIENT_RESOURCES;
			case ERROR_INVALID_THREAD_ID:
				return ERROR_INVALID_THREAD;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_DETECTED_DEADLOCK;
			default:
				return ERROR_UNKNOWN;
			};
		}

		return SUCCESS;
	}
	Thread::ThreadResult Thread::Detach() {
		// Check if the thread was begun
		if(!internalData)
			return ERROR_INVALID_THREAD;

		// Close the thread's handle and ignore the thread
		WINBOOL result = CloseHandle((HANDLE)internalData);

		// If successful, reset the thread's values and exit the function
		if(result) {
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		}

		// Convert the Win32 error code to a thread error code
		DWORD error = GetLastError();

		switch(error) {
		case ERROR_SUCCESS:
			// This shouldn't happen? w/e
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case ERROR_INVALID_HANDLE:
			return ERROR_INVALID_THREAD;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Thread::ThreadResult Thread::Cancel() {
		// Check if the thread was begun
		if(!internalData)
			return ERROR_INVALID_THREAD;

		// Terminate the thread using TerminateThread
		WINBOOL result = TerminateThread((HANDLE)internalData, 0);

		// If successful, reset the thread's values and exit the function
		if(result) {
			internalData = nullptr;
			threadID = 0;
		}

		// Convert the Win32 error code to a thread error code
		DWORD error = GetLastError();

		switch(error) {
		case ERROR_SUCCESS:
			// This shouldn't happen? w/e
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case ERROR_NOT_ENOUGH_MEMORY:
		case ERROR_OUTOFMEMORY:
		case ERROR_DS_THREAD_LIMIT_EXCEEDED:
			return ERROR_INSUFFICIENT_RESOURCES;
		case ERROR_INVALID_THREAD_ID:
		case ERROR_INVALID_HANDLE:
			return ERROR_INVALID_THREAD;
		case ERROR_POSSIBLE_DEADLOCK:
			return ERROR_DETECTED_DEADLOCK;
		default:
			return ERROR_UNKNOWN;
		};
	}
	Thread::ThreadResult Thread::Join(void** returnPtr) {
		// Check if the thread was begun
		if(!internalData)
			return ERROR_INVALID_THREAD;

		// Wait for the thred to finish execution using WaitForSingleObject
		DWORD waitResult = WaitForSingleObject((HANDLE)internalData, INFINITE);

		// Check if the function failed
		if(waitResult == WAIT_FAILED) {
			// Convert the Win32 error code to a thread error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_THREAD;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_DETECTED_DEADLOCK;
			default:
				return ERROR_UNKNOWN;
			}
		}

		// Get the thread's exit code
		DWORD result;
		WINBOOL exitResult = GetExitCodeThread((HANDLE)internalData, &result);

		// Check if the function failed
		if(!exitResult) {
			// Convert the Win32 error code to a thread error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_INVALID_THREAD_ID:
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_THREAD;
			default:
				return ERROR_UNKNOWN;
			}
		}

		if(returnPtr) {
			// Save the full result to the given pointer
			*returnPtr = threadResults[result];

			// Update the result bitmask
			threadResultBitmask |= (uint64_t)1 << result;
		}
		
		// Close the thread's handle
		WINBOOL closeResult = CloseHandle((HANDLE)internalData);

		// If successful, reset the thread's values and exit the function
		if(closeResult) {
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		}

		// Convert the Win32 error code to a thread error code
		DWORD error = GetLastError();

		switch(error) {
		case ERROR_SUCCESS:
			// This shouldn't happen? w/e
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case ERROR_INVALID_HANDLE:
			return ERROR_INVALID_THREAD;
		default:
			return ERROR_UNKNOWN;
		}
	}

	Thread::~Thread() {
		// Check if the thread was begun
		if(!internalData)
			return;

		// Terminate the thread
		TerminateThread((HANDLE)internalData, 0);
	}

	Thread::ThreadID GetCurrentThreadID() {
		// Get the current thread ID using GetCurrentThreadId
		return (Thread::ThreadID)GetCurrentThreadId();
	}
	void ExitCurrentThread(void* returnValue) {
		// Get the current thread's ID
		Thread::ThreadID threadID = GetCurrentThreadID();

		// Try to find a free thread result array index
		size_t threadResultIndex = 0;
		bool8_t threadResultIndexFound = false;
		while(!threadResultIndexFound) {
			// Try to find a index for the current thread result
			threadResultIndexFound = true;
			uint64_t oldBitmask, newBitmask;
			do {
				// Load the old bitmask
				oldBitmask = threadResultBitmask;

				// Exit the loop if the old bitmask is full
				if(!oldBitmask) {
					threadResultIndexFound = false;
					break;
				}

				// Calculate the thread result index and new thread bitmask
				threadResultIndex = Pow2BitIndex(RightmostBit(oldBitmask));
				newBitmask = oldBitmask & ~(1 << threadResultIndex);
			} while(threadResultBitmask.compare_exchange_weak(oldBitmask, newBitmask));
		}

		// Store the thread result in the thread result array
		threadResults[threadResultIndex] = returnValue;

		// Exit the current thread using ExitThread and return the thread result index
		ExitThread((DWORD)threadResultIndex);
	}
	size_t GetProcessorCount() {
		// Get the SYSTEM_INFO struct
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		// Return the processor count from the system info
		return (size_t)systemInfo.dwNumberOfProcessors;
	}
}

#endif