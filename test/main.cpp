#include "Core.hpp"
#include "Tests.hpp"
#include <stdio.h>
#include <time.h>

namespace wfe {
	typedef const char_t*(*TestsCallback)();

	void RunTest(const char_t* testName, TestsCallback testsCallback, TestsCallback testsResultCallback) {
		// Output the running tests message
		printf("Running %s tests ... ", testName);

		// Get the tests' results
		char_t* testsResult = (char_t*)testsCallback();
		char_t* testsWantedResult = (char_t*)testsResultCallback();

		// Compare the given result with the wanted result
		int32_t comparison = strcmp(testsResult, testsWantedResult);

		// Exit the function if the result is correct
		if(!comparison) {
			printf("Correct\n");

			// Free the given result
			free(testsResult, strlen(testsResult + 1), MEMORY_USAGE_STRING);

			return;
		}

		// Print out the given and wanted result
		printf("Incorrect\n\nResult:\n%s\nWanted:\n%s\n", testsResult, testsWantedResult);

		// Free the given result
		free(testsResult, strlen(testsResult + 1), MEMORY_USAGE_STRING);
	}
	void RunTests() {
		RunTest("memory", MemoryTests, MemoryTestsResult);
		RunTest("string", StringTests, StringTestsResult);
	}
}

int main() {
	wfe::RunTests();
	return 0;
}