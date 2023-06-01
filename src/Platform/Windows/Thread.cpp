#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Thread.hpp"
#include "Debug.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
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

		// Create the thread using CreateThread
		internalData = (void*)CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func, args, 0, &threadIDWord);

		// Set the thread's new ID
		threadID = (ThreadID)threadIDWord;

		// Convert the Win32 error code to a thread error code
		DWORD error = GetLastError();

		switch(error) {
		case ERROR_SUCCESS:
			return SUCCESS;
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

		// Save the result to the given pointer
		*returnPtr = (void*)result;        
		
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

	Mutex::Mutex() {
		// Create the mutex using CreateMutex
		internalData = (void*)CreateMutexA(nullptr, FALSE, nullptr);

		// Check if the mutex creation failed
		if(!internalData) {
			// Convert the Win32 error code to a mutex error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_NOT_ENOUGH_MEMORY:
			case ERROR_OUTOFMEMORY:
				WFE_LOG_FATAL("Failed to create mutex! Reason: Insufficient memory.");
				break;
			default:
				WFE_LOG_FATAL("Failed to creatre mutex! Reason: Unknown.");
			}
		}
	}

	Mutex::MutexResult Mutex::Lock() {
		// Wait for the mutex using WaitForSingleObject
		DWORD result = WaitForSingleObject((HANDLE)internalData, INFINITE);

		// Handle the function's result
		switch(result) {
		case WAIT_OBJECT_0:
			return SUCCESS;
		case WAIT_ABANDONED:
			return ERROR_UNKNOWN;
		case WAIT_FAILED: {
			// Convert the Win32 error code to a mutex error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_MUTEX;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_MUTEX_OWNED;
			default:
				return ERROR_UNKNOWN;
			}
		}
		}

		return ERROR_UNKNOWN;
	}
	Mutex::MutexResult Mutex::TryLock() {
		// Check if the mutex is locked by calling WaitForSingleObject with a null max time
		DWORD result = WaitForSingleObject((HANDLE)internalData, 0);

		// Handle the function's result
		switch(result) {
		case WAIT_OBJECT_0:
			return SUCCESS;
		case WAIT_TIMEOUT:
			return ERROR_ALREADY_LOCKED;
		case WAIT_ABANDONED:
			return ERROR_UNKNOWN;
		case WAIT_FAILED: {
			// Convert the Win32 error code to a mutex error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_MUTEX;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_MUTEX_OWNED;
			default:
				return ERROR_UNKNOWN;
			}
		}
		}
	}
	Mutex::MutexResult Mutex::Unlock() {
		// Unlock the mutex using ReleaseMutex
		BOOL result = ReleaseMutex((HANDLE)internalData);

		// Check if the function failed
		if(!result) {
			// Convert the Win32 error code to a mutex error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_MUTEX;
			case ERROR_NOT_OWNER:
				return ERROR_MUTEX_NOT_OWNED;
			default:
				return ERROR_UNKNOWN;
			}
		}

		return SUCCESS;
	}

	Mutex::~Mutex() {
		// Close the mutex's handle
		CloseHandle((HANDLE)internalData);
	}

	Thread::ThreadID GetCurrentThreadID() {
		// Get the current thread ID using GetCurrentThreadId
		return (Thread::ThreadID)GetCurrentThreadId();
	}
	void ExitCurrentThread(void* returnValue) {
		// Exit the current thread using ExitThread
		ExitThread((DWORD)returnValue);
	}
}

#endif