#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Mutex.hpp"
#include "Exception.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	Mutex::Mutex() {
		// Create the mutex using CreateMutex
		internalData = (void*)CreateMutexA(nullptr, FALSE, nullptr);

		// Check if the mutex creation failed
		if(!internalData) {
			// Throw an exception based on the Win32 error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_NOT_ENOUGH_MEMORY:
			case ERROR_OUTOFMEMORY:
				throw BadAllocException("Failed to create mutex! Reason: Insufficient memory.");
				break;
			default:
				throw Exception("Failed to create mutex! Reason: Unknown.");
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

			break;
		}
		}

		return ERROR_UNKNOWN;
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

	void AtomicMutex::Lock() {
		// Wait until the mutex's value is set to 0
		uint8_t target = 0;
		while(!val.compare_exchange_weak(target, 1)) {
			// Reset the target value
			target = 0;
			Sleep(0);
		}
	}
	bool8_t AtomicMutex::TryLock() {
		// Try to set the mutex's value
		uint8_t target = 0;
		return val.compare_exchange_weak(target, 1);
	}
	void AtomicMutex::Unlock() {
		// Set the mutex's value to 0
		val = 0;
	}
}

#endif