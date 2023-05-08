#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Thread.hpp"
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
}

#endif