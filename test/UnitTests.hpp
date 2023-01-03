#pragma once

#include "Core.hpp"

namespace wfe {
	/// @brief The maximum possible length of the result of a unit test.
	const size_t MAX_UNIT_TEST_RESULT_LENGTH = 256;
	/// @brief The maximum possible number of unit tests for every list.
	const size_t MAX_UNIT_TEST_COUNT_LIST = 128;
	
	/// @brief Stores a simple unit test's info and its result
	struct UnitTest {
		/// @brief The unit test's name.
		const char_t* name;
		/// @brief The result of the unit test.
		char_t result[MAX_UNIT_TEST_RESULT_LENGTH];
		/// @brief The wanted result of the unit test.
		const char_t* wantedResult;

		/// @brief Formats the unit test's result.
		/// @param format The format to use for the result.
		void FormatResult(const char_t* format, ...);
		/// @brief Checks if the unit test's result is correct.
		/// @return True if the unit test's result is correct, otherwise false.
		bool8_t IsCorrect();
	};

	/// @brief Stores a list of unit tests which fall under a larger category.
	struct UnitTestList {
		/// @brief The unit test list's name.
		const char_t* name;
		/// @brief The number of unit tests in the list.
		size_t unitTestCount;
		/// @brief An array of every unit test in the list.
		UnitTest unitTests[MAX_UNIT_TEST_COUNT_LIST];
	};

	/// @brief Called when unit tests are run.
	typedef void(*UnitTestCallback)(UnitTestList&);

	/// @brief Adds the given unit test callback to the list of callbacks to be run when unit tests are checked.
	void AddUnitTestCallback(UnitTestCallback callback);
	/// @brief Runs all unit tests.
	void RunUnitTests();
}

/// @brief Adds the given unit test callback to the list of callbacks to be run when unit tests are checked.
#define WFE_ADD_UNIT_TEST_CALLBACK(callback) \
namespace { \
	struct UnitTestCallbackAdder { \
		UnitTestCallbackAdder() { \
			wfe::AddUnitTestCallback(callback); \
		} \
	}; \
	UnitTestCallbackAdder callbackAdder; \
}
