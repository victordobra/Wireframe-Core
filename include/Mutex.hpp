#pragma once

#include "BuildInfo.hpp"
#include "Defines.hpp"

#ifdef WFE_PLATFORM_LINUX
#include <pthread.h>
#endif

namespace wfe {
	/// @brief An implementation of a mutex.
	class Mutex {
	public:
		/// @brief All possible results from mutex operations.
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

		/// @brief Converts the given mutex result to its string equivalent.
		/// @param result The mutex result to convert.
		/// @return The string equivalent of the given result.
		static const char_t* MutexResultToString(MutexResult result) {
			switch(result) {
			case SUCCESS:
				return "SUCCESS";
			case ERROR_ALREADY_LOCKED:
				return "ERROR_ALREADY_LOCKED";
			case ERROR_INVALID_MUTEX:
				return "ERROR_INVALID_MUTEX";
			case ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED:
				return "ERROR_MAX_RECURSIVE_LOCK_COUNT_EXCEEDED";
			case ERROR_MUTEX_OWNED:
				return "ERROR_MUTEX_OWNED";
			case ERROR_MUTEX_NOT_OWNED:
				return "ERROR_MUTEX_NOT_OWNED";
			default:
				return "ERROR_UNKNOWN";
			}
		}

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

	/// @brief An implementation of a mutex that uses atomic variables for synchronization.
	class AtomicMutex {
	public:
		/// @brief Creates an atomic mutex object.
		AtomicMutex() = default;
		AtomicMutex(const AtomicMutex&) = delete;
		AtomicMutex(AtomicMutex&&) noexcept = delete;

		AtomicMutex& operator=(const AtomicMutex&) = delete;
		AtomicMutex& operator=(AtomicMutex&&) noexcept = delete;

		/// @brief Locks the mutex. If the mutex is already locked, the calling thread will wait for it to unlock.
		void Lock();
		/// @brief Locks the mutex. If the mutex is already locked, the function will return immediately.
		/// @return True if the mutex was locked, otherwise false.
		bool8_t TryLock();
		/// @brief Unlocks the mutex.
		void Unlock();

		/// @brief Destroys the atomic mutex.
		~AtomicMutex() = default;
	private:
		atomic_uint8_t val = 0;
	};
}