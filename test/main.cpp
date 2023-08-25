#include "UnitTests.hpp"
#include <sys/time.h>
#include <stdio.h>

#define TEST_COUNT 1000000
int main() {
	wfe::CreateAllocator();
	timeval start, end;
	uint64_t duration;

	gettimeofday(&start, nullptr);

	for(size_t i = 0; i != TEST_COUNT; ++i) {
		void* p1 = wfe::malloc(12, wfe::MEMORY_USAGE_OTHER);
		void* p2 = wfe::malloc(191, wfe::MEMORY_USAGE_OTHER);
		void* p3 = wfe::malloc(31, wfe::MEMORY_USAGE_OTHER);
		void* p4 = wfe::malloc(7, wfe::MEMORY_USAGE_OTHER);
		void* p5 = wfe::malloc(102, wfe::MEMORY_USAGE_OTHER);

		wfe::free(p4, wfe::MEMORY_USAGE_OTHER);
		wfe::free(p3, wfe::MEMORY_USAGE_OTHER);
		wfe::free(p5, wfe::MEMORY_USAGE_OTHER);
		wfe::free(p2, wfe::MEMORY_USAGE_OTHER);
		wfe::free(p1, wfe::MEMORY_USAGE_OTHER);
	}

	gettimeofday(&end, nullptr);
	duration = (uint64_t)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) * 1000 / TEST_COUNT;

	printf("%lluns", (unsigned long long)duration);
	
	//wfe::RunUnitTests();

	return 0;
}