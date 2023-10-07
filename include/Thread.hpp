#pragma once

#include "Defines.hpp"

#ifdef WFE_PLATFORM_LINUX
#include <pthread.h>
#endif

namespace wfe {
	/// @brief An implementation of a processor thread.
	struct Thread {
	public:
		/// @brief All possible results for thread functions.
		typedef enum {
			/// @brief The operation completed successfully.
			SUCCESS,
			/// @brief The operation failed to allocate memory from the heap
			ERROR_FAILED_HEAP_ALLOCATION,
			/// @brief The thread to be begun was already begun.
			ERROR_THREAD_ALREADY_BEGUN,
			/// @brief The thread couldn't be created due to insufficient resources.
			ERROR_INSUFFICIENT_RESOURCES,
			/// @brief The current thread is not valid.
			ERROR_INVALID_THREAD,
			/// @brief The current thread is not joinable.
			ERROR_THREAD_NOT_JOINABLE,
			/// @brief A deadlock in the current thread was detected.
			ERROR_DETECTED_DEADLOCK,
			/// @brief An unknown or unhandled error prevented the requested thread operation.
			ERROR_UNKNOWN
		} ThreadResult;

		/// @brief The function that will be run on the thread.
		typedef void*(*ThreadFunction)(void* args);
		/// @brief Holds a thread's ID.
		typedef uint64_t ThreadID;

		/// @brief Creates a thread object.
		Thread() = default;
		Thread(const Thread&) = delete;
		Thread(Thread&&) noexcept = delete;

		Thread& operator=(const Thread&) = delete;
		Thread& operator=(Thread&&) noexcept = delete;

		/// @brief Compares the thread IDs.
		/// @param other The thread to compare with.
		/// @return True if their IDs are equal, otherwise false.
		bool8_t operator==(const Thread& other) const;
		/// @brief Compares the thread IDs.
		/// @param other The thread to compare with.
		/// @return True if their IDs are different, otherwise false.
		bool8_t operator!=(const Thread& other) const;

		/// @brief Gets the thread's ID.
		/// @return The thread's ID, or 0 if the thread hasn't begun executing.
		ThreadID GetID() const;
		/// @brief Gets the thread class's internal data, which is the platform specific thread pointer.
		/// @return A void pointer, which can be cast to the specific platform's thread object.
		void* GetInternalData();

		/// @brief Begins executing the given function on the thread.
		/// @param func The function to execute on the thread.
		/// @param args The args to be passed to the given function.
		/// @return Thread::SUCCESS if the thread began executing correctly, otherwise a corresponding error code.
		ThreadResult Begin(ThreadFunction func, void* args);
		/// @brief Detaches the thread, causing it to be reclaimed by the implementation once it terminates.
		/// @return Thread::SUCCESS if the thread was detached successfully, otherwise a corresponding error code.
		ThreadResult Detach();
		/// @brief Requests for the thread to cancel execution.
		/// @return Thread::SUCCESS if the cancel request was sent successfully, otherwise a corresponding error code.
		ThreadResult Cancel();
		/// @brief Waits for this thread to finish execution.
		/// @param returnPtr An optional pointer to a void pointer which will be set to the thread's return value.
		/// @return Thread::SUCCESS if the thread was joined successfully, otherwise a correcponding error code.
		ThreadResult Join(void** returnPtr = nullptr);

		/// @brief Destroys the thread.
		~Thread();
	private:
		void* internalData = nullptr;
		ThreadID threadID = 0;
	};

	/// @brief An implementation of a mutex.
	struct Mutex {
	public:
		/// @brief All possible results from mutex functions.
		typedef enum {
			/// @brief The operation completed successfully
			SUCCESS,
			/// @brief The mutex to be locked was already locked.
			ERROR_ALREADY_LOCKED,
			/// @brief The current mutex is not valid.
			ERROR_INVALID_MUTEX,
			/// @brief The current mutex's max reccursive lock count has been exceeded.
			ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED,
			/// @brief The current thread owns the mutex.
			ERROR_MUTEX_OWNED,
			/// @brief The current thread doesn't own the mutex.
			ERROR_MUTEX_NOT_OWNED,
			/// @brief An unknown or unhandled error prevented the requested mutex operation.
			ERROR_UNKNOWN
		} MutexResult;

		/// @brief Creates a mutex object.
		Mutex();
		Mutex(const Mutex&) = delete;
		Mutex(Mutex&&) noexcept = delete;

		Mutex& operator=(const Mutex&) = delete;
		Mutex& operator=(Mutex&&) noexcept = delete;

		/// @brief Locks the mutex. If the mutex is already locked, the calling thread will wait for it to unlock.
		/// @return Mutex::SUCCESS if the mutex was locked successfully, otherwise a corresponding error code.
		MutexResult Lock();
		/// @brief Locks the mutex. If the mutex is already locked, the function will return immediately Mutex::ERROR_ALREADY_LOCKED.
		/// @return Mutex::SUCCESS if the mutex was locked successfully, otherwise a corresponding error code.
		MutexResult TryLock();
		/// @brief Unlocks the mutex.
		/// @return Mutex::SUCCESS if the mutex was unlocked successfully, otherwise a corresponding error code.
		MutexResult Unlock();

		/// @brief Destroys the mutex.
		~Mutex();
	private:
#ifdef WFE_PLATFORM_LINUX
		pthread_mutex_t internalData;
#else
		void* internalData;
#endif
	};

	/// @brief Gets the current thread's ID.
	/// @return THe current thread's ID.
	Thread::ThreadID GetCurrentThreadID();
	/// @brief Exits the current thread, calcelling all processing on it.
	/// @param returnValue The return value to be extracted on joining the thread.
	void ExitCurrentThread(void* returnValue);
	/// @brief Returns the number of processors.
	/// @return The number of processors.
	size_t GetProcessorCount();
}