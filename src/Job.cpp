#include "Job.hpp"
#include "Allocator.hpp"
#include "Exception.hpp"
#include "New.hpp"

namespace wfe {
	// Constants
	const size_t START_THREAD_CAPACITY = 256;

	// Internal functions
	void* JobManager::JobThreadManager(void* args) {
		// Get the job manager's pointer
		JobManager* manager = (JobManager*)args;

		while(true) {
			// Wait for at least one job to ba available
			auto waitResult = manager->jobQueueSemaphore.Wait();
			if(waitResult != Semaphore::SUCCESS)
				throw Exception("Failed to wait for semaphore! Error code: %s", Semaphore::SemaphoreResultToString(waitResult));

			// Lock the queue mutex and retrieve the first job's info from the queue
			manager->queueMutex.Lock();

			size_t jobInd = manager->queueFront;

			// Exit the loop if no job is available, as this means that the manager is begin destroyed
			if(jobInd == SIZE_T_MAX) {
				manager->queueMutex.Unlock();
				break;
			}

			JobFunction jobFunc = manager->jobList[jobInd].func;
			void* jobArgs = manager->jobList[jobInd].args;

			// Remove the current job from the queue and unlock the queue mutex
			manager->queueFront = manager->jobList[jobInd].next;
			if(manager->queueFront == SIZE_T_MAX)
				manager->queueBack = SIZE_T_MAX;

			manager->queueMutex.Unlock();

			// Run the job's function
			void* result = jobFunc(jobArgs);

			// Lock the queue mutex and check if the result still exists
			manager->queueMutex.Lock();

			if(manager->jobList[jobInd].result) {
				// Set the result variable and signal the job's semaphore
				manager->jobList[jobInd].result->result = result;

				auto signalResult = manager->jobList[jobInd].result->semaphore.Signal();
				if(signalResult != Semaphore::SUCCESS)
					throw Exception("Failed to signal semaphore! Error code: %s", Semaphore::SemaphoreResultToString(signalResult));
				
				// Remove the manager's pointer from the result
				manager->jobList[jobInd].result->manager = nullptr;
			}

			// Add the current job to the free list and unlock the queue mutex
			manager->jobList[jobInd].next = manager->freeList;
			manager->freeList = jobInd;

			manager->queueMutex.Unlock();
		}

		return nullptr;
	}

	// Public functions
	bool8_t JobManager::Result::GetResult(void** result) {
		// Check if the job has finished by trying to wait for the semaphore
		auto waitRes = semaphore.TryWait();

		// Handle the wait's result
		if(waitRes == Semaphore::ERROR_ALREADY_LOCKED)
			return false;
		if(waitRes != Semaphore::SUCCESS)
			throw Exception("Failed to try waiting for semaphore! Error code: %s", Semaphore::SemaphoreResultToString(waitRes));
		
		// Set the result at the given address, if requested
		if(result)
			*result = this->result;

		return true;
	}
	void JobManager::Result::WaitForResult(void** result) {
		// Wait for the job's semaphore
		auto waitRes = semaphore.Wait();
		if(waitRes != Semaphore::SUCCESS)
			throw Exception("Failed to wait for semaphore! Error code: %s", Semaphore::SemaphoreResultToString(waitRes));
		
		// Set the result at the given address, if requested
		if(result)
			*result = this->result;
	}

	JobManager::Result::~Result() {
		// Remove the result's pointer from its corresponding job, if it still exists
		if(manager) {
			manager->queueMutex.Lock();
			manager->jobList[jobInd].result = nullptr;
			manager->queueMutex.Unlock();
		}
	}

	JobManager::JobManager(size_t threadCount) : threadCount(threadCount), jobCapacity(START_THREAD_CAPACITY), jobList((Job*)AllocMemory(jobCapacity * sizeof(Job))), queueFront(SIZE_T_MAX), queueBack(SIZE_T_MAX), freeList(0) {
		// Check if the job list was allocated successfully
		if(!jobList)
			throw BadAllocException("Failed to allocate job list!");

		// Create the thread array
		threads = NewArray<Thread>(threadCount);

		// Start the job manager on every created thread
		for(size_t i = 0; i != threadCount; ++i) {
			auto result = threads[i].Begin(JobThreadManager, this);
			if(result != Thread::SUCCESS)
				throw Exception("Failed to begin running on thread! Error code: %s", Thread::ThreadResultToString(result));
		}

		// Set the next indices of jobs in the free list
		for(size_t i = 0; i != jobCapacity - 1; ++i)
			jobList[i].next = i + 1;
		jobList[jobCapacity - 1].next = SIZE_T_MAX;
	}

	void JobManager::SubmitJob(JobFunction func, void* args, Result& result) {
		// Lock the queue mutex and check if the job list is full
		queueMutex.Lock();

		if(freeList == SIZE_T_MAX) {
			// Reallocate the job list
			size_t oldCapacity = jobCapacity;
			jobCapacity >>= 1;

			jobList = (Job*)ReallocMemory(jobList, jobCapacity * sizeof(Job));
			if(!jobList)
				throw BadAllocException("Failed to reallocate job list!");
			
			// Set the new free list's info
			freeList = oldCapacity;
			for(size_t i = oldCapacity; i != jobCapacity - 1; ++i)
				jobList[i].next = i + 1;
			jobList[jobCapacity - 1].next = SIZE_T_MAX;
		}

		// Get an empty job from the free list
		size_t jobInd = freeList;
		freeList = jobList[jobInd].next;

		// Set the job's new info
		jobList[jobInd].func = func;
		jobList[jobInd].args = args;
		jobList[jobInd].result = &result;

		// Set the result's info
		result.manager = this;
		result.jobInd = jobInd;
		
		// Add the new job at the end of the queue
		if(queueBack == SIZE_T_MAX) {
			queueFront = jobInd;
			queueBack = jobInd;
		} else {
			jobList[queueBack].next = jobInd;
			queueBack = jobInd;
		}
		jobList[jobInd].next = SIZE_T_MAX;

		// Unlock the queue mutex and signal the queue semaphore
		queueMutex.Unlock();
		
		auto signalResult = jobQueueSemaphore.Signal();
		if(signalResult != Semaphore::SUCCESS)
			throw Exception("Failed to signal semaphore! Error code: %s", Semaphore::SemaphoreResultToString(signalResult));
	}

	JobManager::~JobManager() {
		// Signal the semaphore once for every thread, for them to reach their exit conditions
		jobQueueSemaphore.Signal((uint32_t)threadCount);
		
		// Join every thread
		for(size_t i = 0; i != threadCount; ++i)
			threads[i].Join();

		// Free the thread and job arrays
		DestroyArray(threads, threadCount);
		FreeMemory(jobList);
	}
}