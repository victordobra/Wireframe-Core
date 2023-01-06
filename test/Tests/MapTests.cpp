#include "UnitTests.hpp"

namespace wfe {
	void MapUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Map";

		/* Test operator[] */ {
			// Test 1
			map<int32_t, const char_t*> test1;

			test1[2] = "second";
			test1[3] = "fourth";
			test1[1] = "first";
			test1[3] = "third";

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "operator[] 1";
			unitTest1.FormatResult("%s %s %s", test1[1], test1[2], test1[3]);
			unitTest1.wantedResult = "first second third";
		}
		/* Test at */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 3, "three" }, { 2, "two" } };

			const char_t* test1Result1 = test1.at(1);
			const char_t* test1Result2 = test1.at(2);
			const char_t* test1Result3 = test1.at(3);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "at 1";
			unitTest1.FormatResult("%s %s %s", test1Result1, test1Result2, test1Result3);
			unitTest1.wantedResult = "one two three";
		}
		/* Test insert */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 5, "five" }, { 3, "three" }, { 1, "one" } };

			test1.insert({ 4, "four" });
			test1.insert(test1.begin() + 1, { 2, "two" });
			test1.insert({ { 6, "six" }, { 7, "seven" } });

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%s %s %s %s %s %s %s", test1.begin()->second, (test1.begin() + 1)->second, (test1.begin() + 2)->second, (test1.begin() + 3)->second, (test1.begin() + 4)->second, (test1.begin() + 5)->second, (test1.begin() + 6)->second);
			unitTest1.wantedResult = "one two three four five six seven";
		}
		/* Test erase */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 3, "three" }, { 5, "five" }, { 7, "seven" }, { 2, "two" }, { 4, "out" }, { 6, "six" }, { 8, "eight" } };

			test1.erase(test1.begin() + 1);
			test1.erase(4);
			test1.erase(test1.begin() + 3, test1.end());

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%s %s %s", test1.begin()->second, (test1.begin() + 1)->second, (test1.begin() + 2)->second);
			unitTest1.wantedResult = "one three five";
		}
		/* Test swap */ {
			// Test 1
			map<int32_t, const char_t*> test1Map1 = { { 1, "one" }, { 3, "three" } };
			map<int32_t, const char_t*> test1Map2 = { { 2, "two" }, { 4, "four" } };

			test1Map1.swap(test1Map2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "swap 1";
			unitTest1.FormatResult("%s %s %s %s", test1Map1.begin()->second, (test1Map1.begin() + 1)->second, test1Map2.begin()->second, (test1Map2.begin() + 1)->second);
			unitTest1.wantedResult = "two four one three";
		}
		/* Test clear */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 4, "four" }, { 2, "two" }, { 3, "three" } };

			test1.clear();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "clear 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1.size());
			unitTest1.wantedResult = "0";
		}
		/* Test find */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 4, "four" }, { 2, "two" }, { 3, "three" } };

			size_t test1Result1 = test1.find(3) - test1.begin();
			size_t test1Result2 = test1.find(5) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 4";
		}
		/* Test lower_bound */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 4, "four" }, { 2, "two" }, { 3, "three" } };

			size_t test1Result1 = test1.lower_bound(3) - test1.begin();
			size_t test1Result2 = test1.lower_bound(5) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "lower_bound 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 4";
		}
		/* Test upper_bound */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 4, "four" }, { 2, "two" }, { 3, "three" } };

			size_t test1Result1 = test1.upper_bound(3) - test1.begin();
			size_t test1Result2 = test1.upper_bound(5) - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "upper_bound 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "3 4";
		}
		/* Test equal_range */ {
			// Test 1
			map<int32_t, const char_t*> test1 = { { 1, "one" }, { 4, "four" }, { 2, "two" }, { 3, "three" } };

			auto test1Result = test1.equal_range(3);

			size_t test1Result1 = test1Result.first - test1.begin();
			size_t test1Result2 = test1Result.second - test1.begin();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "equal_range 1";
			unitTest1.FormatResult("%llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2);
			unitTest1.wantedResult = "2 3";
		}
	}

	WFE_ADD_UNIT_TEST_CALLBACK(MapUnitTestCallback)
}