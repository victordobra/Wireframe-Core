#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Thread.hpp"
#include "Debug.hpp"
#include "Memory.hpp"
#include <pthread.h>
#include <errno.h>

namespace wfe {
	bool8_t Thread::operator==(const Thread& other) const {
		// Compare the two threads using pthread_equal
		return pthread_equal((pthread_t)internalData, (pthread_t)other.internalData);
	}
	bool8_t Thread::operator!=(const Thread& other) const {
		// Compare the two threads using pthread_equal
		return !pthread_equal((pthread_t)internalData, (pthread_t)other.internalData);
	}

	Thread::ThreadID Thread::GetID() const {
		return threadID;
	}
	void* Thread::GetInternalData() {
		return internalData;
	}

	Thread::ThreadResult Thread::Begin(ThreadFunction func, void* args) {
		// Use pthread_create to start the thread
		int32_t result = pthread_create((pthread_t*)&internalData, nullptr, func, args);

		// Set the thread's ID to the thread pointer
		threadID = (ThreadID)internalData;

		// Convert the given result to a thread result
		switch(result) {
		case 0:
			return SUCCESS;
		case EAGAIN:
			return ERROR_INSUFFICIENT_RESOURCES;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Thread::ThreadResult Thread::Detach() {
		// Use pthread_detach to detach the thread
		int32_t result = pthread_detach((pthread_t)internalData);

		// Convert the given result to a thread result
		switch(result) {
		case 0:
			// Set the thread pointer and ID back to default
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case EINVAL:
			return ERROR_THREAD_NOT_JOINABLE;
		case ESRCH:
			return ERROR_INVALID_THREAD;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Thread::ThreadResult Thread::Cancel() {
		// Use pthread_cancel to cancel the thread
		int32_t result = pthread_cancel((pthread_t)internalData);

		// Convert the given result to a thread result
		switch(result) {
		case 0:
			// Set the thread pointer and ID back to default
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case ESRCH:
			return ERROR_INVALID_THREAD;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Thread::ThreadResult Thread::Join(void** returnPtr) {
		// Use pthread_join to join the thread with the current thread
		int32_t result = pthread_join((pthread_t)internalData, returnPtr);

		// Convert the given result to a thread result
		switch(result) {
		case 0:
			// Set the thread pointer and ID back to default
			internalData = nullptr;
			threadID = 0;

			return SUCCESS;
		case EDEADLK:
			return ERROR_DETECTED_DEADLOCK;
		case EINVAL:
			return ERROR_THREAD_NOT_JOINABLE;
		case ESRCH:
			return ERROR_INVALID_THREAD;
		default:
			return ERROR_UNKNOWN;
		}
	}

	Thread::~Thread() {
		// Exit the function if the thread isn't running or is detached
		if(!internalData)
			return;
		
		// Cancel the thread's execution (ignore error codes since pthread_cancel has no error code relevant to this situation)
		pthread_cancel((pthread_t)internalData);
	}

	Mutex::Mutex() {
		// Allocate the required memory for the mutex
		internalData = malloc(sizeof(pthread_mutex_t), MEMORY_USAGE_HEAP_OBJECT);

		// Check if the memory was allocated corectly
		if(!internalData)
			WFE_LOG_FATAL("Failed to create mutex! Reason: Insufficient memory.");

		// Create the mutex using pthread_mutex_init
		int32_t result = pthread_mutex_init((pthread_mutex_t*)internalData, nullptr);

		// Handle the given result
		switch(result) {
		case 0:
			break;
		case EAGAIN:
			WFE_LOG_FATAL("Failed to create mutex! Reason: Insufficient resources.");
			break;
		case ENOMEM:
			WFE_LOG_FATAL("Failed to create mutex! Reason: Insufficient memory.");
			break;
		case EPERM:
			WFE_LOG_FATAL("Failed to create mutex! Reason: Caller lacks privilege.");
			break;
		case EBUSY:
			WFE_LOG_FATAL("Failed to create mutex! Reason: Mutex already initialised.");
			break;
		default:
			WFE_LOG_FATAL("Failed to create mutex! Reason: Unknown");
			break;
		}
	}

	Mutex::MutexResult Mutex::Lock() {
		// Lock the mutex using pthread_mutex_lock
		int32_t result = pthread_mutex_lock((pthread_mutex_t*)internalData);

		// Convert the given result to a mutex result
		switch(result) {
		case 0:
			return SUCCESS;
		case EINVAL:
			return ERROR_INVALID_MUTEX;
		case EAGAIN:
			return ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED;
		case EDEADLK:
			return ERROR_MUTEX_OWNED;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Mutex::MutexResult Mutex::TryLock() {
		// Try to lock the mutex using pthread_mutex_trylock
		int32_t result = pthread_mutex_trylock((pthread_mutex_t*)internalData);

		// Convert the given result to a mutex result
		switch(result) {
		case 0:
			return SUCCESS;
		case EINVAL:
			return ERROR_INVALID_MUTEX;
		case EBUSY:
			return ERROR_ALREADY_LOCKED;
		case EAGAIN:
			return ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED;
		default:
			return ERROR_UNKNOWN;
		}
	}
	Mutex::MutexResult Mutex::Unlock() {
		// Unlock the mutex using pthread_mutex_unlock
		int32_t result = pthread_mutex_unlock((pthread_mutex_t*)internalData);

		// Convert the given result to a mutex result
		switch(result) {
		case 0:
			return SUCCESS;
		case EINVAL:
			return ERROR_INVALID_MUTEX;
		case EAGAIN:
			return ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED;
		case EPERM:
			return ERROR_MUTEX_NOT_OWNED;
		default:
			return ERROR_UNKNOWN;
		}
	}

	Mutex::~Mutex() {
		// Destroy the mutex using pthread_mutex_destroy
		pthread_mutex_destroy((pthread_mutex_t*)internalData);

		// Free the allocated memory
		free(internalData, sizeof(pthread_mutex_t), MEMORY_USAGE_HEAP_OBJECT);
	}

	Thread::ThreadID GetCurrentThreadID() {
		// Get the ID using pthread_self
		return (Thread::ThreadID)pthread_self();
	}
	void ExitCurrentThread(void* returnValue) {
		// Exit the current thread using pthread_exit
		pthread_exit(returnValue);
	}
}

#endif