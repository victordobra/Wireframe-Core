#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Semaphore.hpp"
#include "Exception.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	// Public functions
	Semaphore::Semaphore(uint32_t start) {
		// Create the semaphore
		internalData = (void*)CreateSemaphoreA(nullptr, (LONG)start, (LONG)MAX_VAL, nullptr);

		// Check if the semaphore creation failed
		if(!internalData) {
			// Throw an exception based on the Win32 error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				break;
			case ERROR_NOT_ENOUGH_MEMORY:
			case ERROR_OUTOFMEMORY:
				throw BadAllocException("Failed to create semaphore! Reason: Insufficient memory.");
				break;
			default:
				throw Exception("Failed to create semaphore! Reason: Unknown.");
			}
		}
	}

	Semaphore::SemaphoreResult Semaphore::Wait() {
		// Wait for the semaphore using WaitForSingleObject
		DWORD result = WaitForSingleObject((HANDLE)internalData, INFINITE);

		// Handle the function's result
		switch(result) {
		case WAIT_OBJECT_0:
			return SUCCESS;
		case WAIT_TIMEOUT:
			return ERROR_UNKNOWN;
		case WAIT_FAILED: {
			// Convert the Win32 error code to a semaphore error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_SEMAPHORE;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_DEADLOCK;
			default:
				return ERROR_UNKNOWN;
			}
		}
		}

		return ERROR_UNKNOWN;
	}
	Semaphore::SemaphoreResult Semaphore::TryWait() {
		// Check if the semaphore's counter is above 0 using WaitForSingleObject with a null max time
		DWORD result = WaitForSingleObject((HANDLE)internalData, 0);

		// Handle the function's result
		switch(result) {
		case WAIT_OBJECT_0:
			return SUCCESS;
		case WAIT_TIMEOUT:
			return ERROR_ALREADY_LOCKED;
		case WAIT_FAILED: {
			// Convert the Win32 error code to a semaphore error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_SEMAPHORE;
			case ERROR_POSSIBLE_DEADLOCK:
				return ERROR_DEADLOCK;
			default:
				return ERROR_UNKNOWN;
			}
		}
		}

		return ERROR_UNKNOWN;

	}
	Semaphore::SemaphoreResult Semaphore::Signal() {
		// Signal the semaphore using ReleaseSemaphore
		BOOL result = ReleaseSemaphore((HANDLE)internalData, 1, nullptr);

		// Check if the semaphore signal failed
		if(!result) {
			// Convert the Win32 error code to a semaphore error code
			DWORD error = GetLastError();

			switch(error) {
			case ERROR_SUCCESS:
				return SUCCESS;
			case ERROR_INVALID_HANDLE:
				return ERROR_INVALID_SEMAPHORE;
			default:
				return ERROR_UNKNOWN;
			}
		}

		return SUCCESS;
	}

	Semaphore::~Semaphore() {
		// Close the semaphore's handle
		CloseHandle((HANDLE)internalData);
	}
}
#endif