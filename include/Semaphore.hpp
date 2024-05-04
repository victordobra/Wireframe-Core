#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief An implementation of a semaphore.
	struct Semaphore {
	public:
		/// @brief The maximum possible value the semaphore can reach.
		const uint32_t MAX_VAL = 1048576;

		/// @brief All possible results from semaphore operations.
		typedef enum {
			/// @brief The operation completed successfully.
			SUCCESS,
			/// @brief The semaphore the operation was attempted on is already locked.
			ERROR_ALREADY_LOCKED,
			/// @brief The current semaphore is invalid.
			ERROR_INVALID_SEMAPHORE,
			/// @brief A deadlock condition was detected.
			ERROR_DEADLOCK,
			/// @brief An unknown or unhandled error prevented the requested semaphore operation.
			ERROR_UNKNOWN
		} SemaphoreResult;

		/// @brief Converts the given semaphore result to its string equivalent.
		/// @param result The semaphore result to convert.
		/// @return The string equivalent of the given result.
		static const char_t* SemaphoreResultToString(SemaphoreResult result) {
			switch(result) {
			case SUCCESS:
				return "SUCCESS";
			case ERROR_ALREADY_LOCKED:
				return "ERROR_ALREADY_LOCKED";
			case ERROR_INVALID_SEMAPHORE:
				return "ERROR_INVALID_SEMAPHORE";
			case ERROR_DEADLOCK:
				return "ERROR_DEADLOCK";
			default:
				return "ERROR_UNKNOWN";
			}
		}

		/// @brief Creates a counting semaphore.
		/// @param start The starting value of the semaphore's counter. Defaulted to 0.
		Semaphore(uint32_t start = 0);
		Semaphore(const Semaphore&) = delete;
		Semaphore(Semaphore&&) noexcept = delete;

		Semaphore& operator=(const Semaphore&) = delete;
		Semaphore& operator=(Semaphore&&) = delete;

		/// @brief Waits for the semaphore's counter to be above 0. Decrements the semaphore's counter, if successful.
		/// @return Semaphore::SUCCESS if the semaphore was waited for successfully, otherwise a corresponding error code.
		SemaphoreResult Wait();
		/// @brief Checks if the semaphore's counter is above 0. If so, it decrements the semaphore's counter, otherwise it returns Semaphore::ERROR_ALREADY_LOCKED.
		/// @return Semaphore::SUCCESS if the semaphore's counter was decremented successfully, otherwise a corresponding error code.
		SemaphoreResult TryWait();
		/// @brief Signals the semaphore.
		/// @return Semaphore::SUCCESS if the semaphore was signaled successfully, otherwise a corresponding error code.
		SemaphoreResult Signal();

		/// @brief Destroys the semaphore.
		~Semaphore();
	private:
		void* internalData;
	};
}