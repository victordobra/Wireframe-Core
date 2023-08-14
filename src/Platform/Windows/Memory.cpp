#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Platform.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>

namespace wfe {
	void GetPerformanceInformation(PerformanceInfo& performanceInfo) {
		// Get all structs required for constructing the performance info
		SYSTEM_INFO systemInfo;
		PERFORMANCE_INFORMATION perfInfo;
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

		GetSystemInfo(&systemInfo);
		::GetPerformanceInfo(&perfInfo, sizeof(PERFORMANCE_INFORMATION));
		GlobalMemoryStatusEx(&memoryStatus);

		// Set the performance info based on the retrieved structs
		performanceInfo.pageSize = (size_t)perfInfo.PageSize;
		performanceInfo.totalPhysicalMemorySize = (uint64_t)memoryStatus.ullTotalPhys;
		performanceInfo.availablePhysicalMemorySize = (uint64_t)memoryStatus.ullAvailPhys;
		performanceInfo.totalVirtualMemorySize = (uint64_t)memoryStatus.ullTotalVirtual;
		performanceInfo.availableVirtualMemorySize = (uint64_t)memoryStatus.ullAvailVirtual;
		performanceInfo.minAppAddress = systemInfo.lpMinimumApplicationAddress;
		performanceInfo.maxAppAddress = systemInfo.lpMaximumApplicationAddress;
		
		performanceInfo.processCount = (size_t)perfInfo.ProcessCount;
		performanceInfo.threadCount = (size_t)perfInfo.ThreadCount;
		performanceInfo.processorCount = (size_t)systemInfo.dwNumberOfProcessors;
	}

	void* ReserveVirtualAddressSpace(void* address, size_t size) {
		// Reserve the wanted region using VirtualAlloc with the MEM_RESERVE flag set
		return VirtualAlloc(address, size, MEM_RESERVE, PAGE_READWRITE);
	}
	void FreeVirtualAddressSpace(void* address) {
		// Free the given region using VirtualFree with the MEM_RELEASE flag set
		VirtualFree(address, 0, MEM_RELEASE);
	}
	bool8_t CommitVirtualAddressSpace(void* address, size_t size) {
		// Commit the given region using VirtualAlloc with the MEM_COMMIT flag set
		return VirtualAlloc(address, size, MEM_COMMIT, PAGE_READWRITE);
	}
	void DecommitVirtualAddressSpace(void* address, size_t size) {
		// Uncommit the given region using VirtualFree with the MEM_DECOMMIT flag set
		VirtualFree(address, size, MEM_DECOMMIT);
	}
}

#endif