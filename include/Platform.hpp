#pragma once

#include "Defines.hpp"

namespace wfe {
	struct PerformanceInfo {
		/// @brief The size of a memory page, in bytes.
		size_t pageSize;
		/// @brief The total ammount of physical memory available, in bytes.
		uint64_t totalPhysicalMemorySize;
		/// @brief The available ammount of physical memory available, in bytes.
		uint64_t availablePhysicalMemorySize;
		/// @brief The total user virtual memory space size, in bytes.
		size_t totalVirtualMemorySize;
		/// @brief The available user virtual memory space size, in bytes.
		size_t availableVirtualMemorySize;
		/// @brief The lowest memory address accessible to applications.
		void* minAppAddress;
		/// @brief The highest memory address accessible to applications.
		void* maxAppAddress;

		/// @brief The current number of processes.
		size_t processCount;
		/// @brief The current number of threads.
		size_t threadCount;
		/// @brief The number of available logical processors.
		size_t processorCount;
	};

	/// @brief Gets the system's performance info.
	/// @param performanceInfo A reference to the variable in which the performance info will be written.
	void GetPerformanceInformation(PerformanceInfo& performanceInfo);

	/// @brief Reserves a region of pages in the virtual address space of the current process.
	/// @param address The starting address to reserve in the address space. Can be set to nullptr for the system to choose the starting address.
	/// @param size The size of the region to be reserved, in bytes.
	/// @return A pointer to the base address of the allocated region, or nullptr if the operation failed.
	void* ReserveVirtualAddressSpace(void* address, size_t size);
	/// @brief Frees a region of pages from the virtual address space of the current process.
	/// @param address The starting address of the region to free.
	void FreeVirtualAddressSpace(void* address);
	/// @brief Commits a region of pages from the virtual address space of the current process by allocating required memory charges.
	/// @param address The starting address of the region to commit.
	/// @param size The size of the region to commit, in bytes.
	/// @return True if the given region was commited successfully, otherwise false.
	bool8_t CommitVirtualAddressSpace(void* address, size_t size);
	/// @brief Uncommits a region of pages from the virtual address space of the current process by freeing all allocated memory charges.
	/// @param address The starting address of the region to uncommit.
	/// @param size The size of the reguin to uncommit, in bytes.
	void DecommitVirtualAddressSpace(void* address, size_t size);
}