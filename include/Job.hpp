#pragma once

#include "Defines.hpp"
#include "Mutex.hpp"
#include "Semaphore.hpp"
#include "Thread.hpp"
#include "UnorderedMap.hpp"

namespace wfe {
	class JobManager {
	public:
		/// @brief The function pointer type for a job's function.
		typedef void*(*JobFunction)(void* args);

		/// @brief A class that can retrieve a job's result once finished.
		class Result {
		public:
			/// @brief Creates an empty job result.
			Result() = default;
			Result(const Result&) = delete;
			Result(Result&&) noexcept = delete;

			Result& operator=(const Result&) = delete;
			Result& operator=(Result&&) = delete;

			/// @brief Tries to get the job's result, if finished. Note that the job's result can only be successfully retrieved once.
			/// @param resultPtr An optional pointer to a void pointer which will be set to the job's result.
			/// @return True if the job has finished, otherwise false.
			bool8_t GetResult(void** result = nullptr);
			/// @brief Waits for the job to finish and gets its result. Note that the job's result can only be successfully retrieved once.
			/// @param resultPtr An optional pointer to a void pointer which will be set to the job's result.
			void WaitForResult(void** result = nullptr);

			/// @brief Destroys the current job result.
			~Result();
		private:
			friend JobManager;

			JobManager* manager;
			size_t jobInd;

			Semaphore semaphore;
			void* result;
		};

		/// @brief Creates a job manager.
		/// @param threadCount The number of threads used by the job manager.
		JobManager(size_t threadCount = 6);
		JobManager(const JobManager&) = delete;
		JobManager(JobManager&) noexcept = delete;

		JobManager& operator=(const JobManager&) = delete;
		JobManager& operator=(JobManager&) = delete;

		/// @brief Submits a job to be executed.
		/// @param func The function to call in the job.
		/// @param args The args to call the job function with.
		/// @param result A reference to the result variable which will be used to 
		void SubmitJob(JobFunction func, void* args, Result& result);

		/// @brief Destroys the job manager.
		~JobManager();
	private:
		static void* JobThreadManager(void* args);

		struct Job {
			JobFunction func;
			void* args;
			Result* result;

			size_t next;
		};

	 	size_t threadCount;
		Thread* threads;
		
		size_t jobCapacity;
		Job* jobList;
		size_t queueFront, queueBack;
		size_t freeList;
		AtomicMutex queueMutex;
		Semaphore jobQueueSemaphore;
	};
}