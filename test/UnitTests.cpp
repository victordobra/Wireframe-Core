#include "UnitTests.hpp"

namespace wfe {
	/// @brief The maximum possible number of unit test callbacks.
	static const size_t MAX_UNIT_TEST_CALLBACK_COUNT = 256;

	/// @brief The number of unit test callbacks.
	static size_t unitTestCallbackCount = 0;
	/// @brief An array of every unit test callback.
	static UnitTestCallback unitTestCallbacks[MAX_UNIT_TEST_CALLBACK_COUNT];

	void UnitTest::FormatResult(const char_t* format, ...) {
		// Get the va list
		va_list args;
		va_start(args, format);
		
		// Format the string using the va list
		FormatStringArgs(result, MAX_UNIT_TEST_RESULT_LENGTH, format, args);

		// End the va list
		va_end(args);
	}
	bool8_t UnitTest::IsCorrect() {
		// Compare the result with the wanted result
		return !strncmp(result, wantedResult, MAX_UNIT_TEST_RESULT_LENGTH);
	}

	void AddUnitTestCallback(UnitTestCallback callback) {
		// Only add the callback to the list if the size doesn't surpass the max size
		if(unitTestCallbackCount < MAX_UNIT_TEST_CALLBACK_COUNT) {
			unitTestCallbacks[unitTestCallbackCount++] = callback;
		} else {
			++unitTestCallbackCount; // For debug purposes
		}
	}
	void RunUnitTests() {
		// Check if there are any unit test callbacks that weren't included
		if(unitTestCallbackCount > MAX_UNIT_TEST_CALLBACK_COUNT) {
			// Output a warning and set the count to the max value
			printf("WARNING: %llu unit test callbacks couldn't be included in the list! Increase the MAX_UNIT_TEST_CALLBACK_COUNT in test/UnitTests.cpp.", (unsigned long long)(unitTestCallbackCount - MAX_UNIT_TEST_CALLBACK_COUNT));
			unitTestCallbackCount = MAX_UNIT_TEST_CALLBACK_COUNT;
		}

		UnitTestList unitTestList;
		unitTestList.unitTestCount = 0;

		size_t totalTestCount = 0, totalCorrectCount = 0;

		// Run every unit test callback
		for(size_t i = 0; i != unitTestCallbackCount; ++i) {
			// Run the callback
			unitTestCallbacks[i](unitTestList);

			printf("%s tests: ", unitTestList.name);

			// Check every unit test
			size_t correctCount = 0;
			uint8_t isCorrect[MAX_UNIT_TEST_COUNT_LIST >> 3];
			memset(isCorrect, 0, MAX_UNIT_TEST_COUNT_LIST >> 3);

			size_t outCount = 0, nextOutIndex = unitTestList.unitTestCount / 10;

			if(nextOutIndex)
				--nextOutIndex;

			for(size_t j = 0; j != unitTestList.unitTestCount; ++j) {
				// Check if the unit test result is correct
				bool8_t correct = unitTestList.unitTests[j].IsCorrect();

				// Increment the number of correct tests if so
				if(correct)
					++correctCount;
				
				// Add the test to the correct array if so
				if(correct)
					isCorrect[j >> 3] |= 1 << (j & 7);
				
				// Test if another dot should be outputted
				while(j == nextOutIndex) {
					// Output the dot and increment the out count
					printf(".");
					++outCount;

					// Set the next out index
					nextOutIndex = unitTestList.unitTestCount * (outCount + 1) / 10;

					if(nextOutIndex)
						--nextOutIndex;
				}
			}

			// Calculate the precentage or correct results
			size_t precentage;
			if(unitTestList.unitTestCount) {
				precentage = correctCount * 100 / unitTestList.unitTestCount;
			} else {
				// Output 10 dots for consistency
				printf("..........");

				precentage = 100;
			}
			
			printf(" %llu%% (%llu/%llu) correct.\n", (unsigned long long)precentage, (unsigned long long)correctCount, (unsigned long long)unitTestList.unitTestCount);

			if(correctCount != unitTestList.unitTestCount) {
				// Output all the incorect unit tests
				printf("\nIncorrect unit tests:\n");

				for(size_t j = 0; j != unitTestList.unitTestCount; ++j) {
					// Check if the current unit test is incorrect
					if(!(isCorrect[j >> 3] & (1 << (j & 7)))) {
						// Output the result and the wanted result
						printf("%s: %s (wanted: %s)\n", unitTestList.unitTests[j].name, unitTestList.unitTests[j].result, unitTestList.unitTests[j].wantedResult);
					}
				}

				printf("\n");
			}

			// Increase the total test and correct count
			totalTestCount += unitTestList.unitTestCount;
			totalCorrectCount += correctCount;

			// Reset the unit test list
			unitTestList.unitTestCount = 0;
		}

		// Output the total precentage
		size_t totalPrecentage = totalCorrectCount * 100 / totalTestCount;

		// Output the total stats
		printf("\nTotal: %llu%% (%llu/%llu) correct.\n", (unsigned long long)totalPrecentage, (unsigned long long)totalCorrectCount, (unsigned long long)totalTestCount);
	}
}