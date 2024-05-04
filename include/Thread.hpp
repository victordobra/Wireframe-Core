#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief An implementation of a processor thread.
	struct Thread {
	public:
		/// @brief All possible results for thread operations.
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

		/// @brief Converts the given thread result to its string equivalent.
		/// @param result The thread result to convert.
		/// @return The string equivalent of the given result.
		static const char_t* ThreadResultToString(ThreadResult result) {
			switch(result) {
			case SUCCESS:
				return "SUCCESS";
			case ERROR_FAILED_HEAP_ALLOCATION:
				return "ERROR_FAILED_HEAP_ALLOCATION";
			case ERROR_THREAD_ALREADY_BEGUN:
				return "ERROR_THREAD_ALREADY_BEGUN";
			case ERROR_INSUFFICIENT_RESOURCES:
				return "ERROR_INSUFFICIENT_RESOURCES";
			case ERROR_INVALID_THREAD:
				return "ERROR_INVALID_THREAD";
			case ERROR_THREAD_NOT_JOINABLE:
				return "ERROR_THREAD_NOT_JOINABLE";
			case ERROR_DETECTED_DEADLOCK:
				return "ERROR_DETECTED_DEADLOCK";
			default:
				return "ERROR_UNKNOWN";
			}
		}

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