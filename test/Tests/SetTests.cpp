#include "UnitTests.hpp"

namespace wfe {
	void SetUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Set";

		/* Test insert */ {
			// Test 1
			set<int32_t> test1 = { 3, 1, 7, 9, 5 };
			
			test1.insert(2);
			test1.insert(test1.begin() + 4, 4);
			test1.insert({ 6, 8, 10 });

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%i %i %i %i %i %i %i %i %i %i", *test1.begin(), *(test1.begin() + 1), *(test1.begin() + 2), *(test1.begin() + 3), *(test1.begin() + 4), *(test1.begin() + 5), *(test1.begin() + 6), *(test1.begin() + 7), *(test1.begin() + 8), *(test1.begin() + 9));
			unitTest1.wantedResult = "1 2 3 4 5 6 7 8 9 10";
		}
		/* Test erase */ {
			// Test 1
			set<int32_t> test1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			test1.erase(test1.begin());
			test1.erase(4);
			test1.erase(6);
			test1.erase(test1.begin() + 4, test1.begin() + 7);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%i %i %i %i", *test1.begin(), *(test1.begin() + 1), *(test1.begin() + 2), *(test1.begin() + 3));
			unitTest1.wantedResult = "2 3 5 7";
		}
		/* Test swap */ {
			// Test 1
			set<int32_t> test1Set1 = { 1, 3, 5 };
			set<int32_t> test1Set2 = { 2, 4, 6 };

			test1Set1.swap(test1Set2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "swap 1";
			unitTest1.FormatResult("%i %i %i %i %i %i", *test1Set1.begin(), *(test1Set1.begin() + 1), *(test1Set1.begin() + 2), *test1Set2.begin(), *(test1Set2.begin() + 1), *(test1Set2.begin() + 2));
			unitTest1.wantedResult = "2 4 6 1 3 5";
		}
		/* Test clear */ {
			// Test 1
			set<int32_t> test1 = { 1, 2, 3, 4, 5 };

			test1.clear();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "clear 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1.size());
			unitTest1.wantedResult = "0";
		}
		/* Test find */ {
			// Test 1
			set<int32_t> test1 = { 1, 3, 5, 7, 9 };

			size_t test1Result1 = test1.find(7) - test1.begin();
			size_t test1Result2 = test1.find(4) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "3 5";
		}
		/* Test lower_bound */ {
			// Test 1
			set<int32_t> test1 = { 1, 3, 5, 7, 9 };

			size_t test1Result1 = test1.lower_bound(4) - test1.begin();
			size_t test1Result2 = test1.lower_bound(5) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "lower_bound 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 2";
		}
		/* Test upper_bound */ {
			// Test 1
			set<int32_t> test1 = { 1, 3, 5, 7, 9 };

			size_t test1Result1 = test1.upper_bound(4) - test1.begin();
			size_t test1Result2 = test1.upper_bound(5) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "upper_bound 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 3";
		}
		/* Test equal_range */ {
			// Test 1
			set<int32_t> test1 = { 1, 3, 5, 7, 9 };

			auto test1Result = test1.equal_range(5);

			size_t test1Result1 = test1Result.first - test1.begin();
			size_t test1Result2 = test1Result.second - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "equal_range 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 3";
		}
	}

	WFE_ADD_UNIT_TEST_CALLBACK(SetUnitTestCallback)
}