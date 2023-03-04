#include "UnitTests.hpp"

namespace wfe {
	void UnorderedSetUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Unordered set";
		
		/* Test find */ {
			// Test 1
			unordered_set<int32_t> test1{ 1, 3, 5, 7, 9 };

			auto test1Result1 = test1.find(5);
			auto test1Result2 = test1.find(9);
			int32_t test1Result3 = (test1.find(2) == test1.end()) ? 1 : 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find 1";
			unitTest1.FormatResult("%i %i %i", *test1Result1, *test1Result2, test1Result3);
			unitTest1.wantedResult = "5 9 1";
		}
		/* Test count */ {
			// Test 1
			unordered_set<int32_t> test1{ 1, 2, 3, 4, 5 };

			size_t test1Result1 = test1.count(4);
			size_t test1Result2 = test1.count(0);
			size_t test1Result3 = test1.count(2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "count 1";
			unitTest1.FormatResult("%llu %llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2, (unsigned long long)test1Result3);
			unitTest1.wantedResult = "1 0 1";
		}
		/* Test equal_range */ {
			// Test 1
			unordered_set<int32_t> test1{ 2, 4, 6, 8, 10 };

			auto test1Result1 = test1.equal_range(4);
			int32_t test1Result2 = (test1.equal_range(1).first == test1.end()) ? 1 : 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "equal_range 1";
			unitTest1.FormatResult("%i %i", *test1Result1.first, test1Result2);
			unitTest1.wantedResult = "4 1";
		}
		/* Test insert */ {
			// Test 1
			unordered_set<int32_t> test1{ 1, 2, 3 };

			test1.insert(5);
			test1.insert({ 4, 5, 6 });

			int32_t test1Result1 = (test1.find(4) != test1.end()) ? 1 : 0;
			int32_t test1Result2 = (test1.find(5) != test1.end()) ? 1 : 0;
			int32_t test1Result3 = (test1.find(6) != test1.end()) ? 1 : 0;
			size_t test1Result4 = test1.size();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%i %i %i %llu", test1Result1, test1Result2, test1Result3, (unsigned long long)test1Result4);
			unitTest1.wantedResult = "1 1 1 6";
		}
		/* Test erase */ {
			// Test 1
			unordered_set<int32_t> test1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			test1.erase(test1.find(4));
			
			auto test1Iter1 = test1.find(5);
			auto test1Iter2 = test1Iter1++;

			test1.erase(test1Iter2, test1Iter1);

			test1.erase(2);

			int32_t test1Result1 = (test1.find(4) == test1.end()) ? 1 : 0;
			int32_t test1Result2 = (test1.find(5) == test1.end()) ? 1 : 0;
			int32_t test1Result3 = (test1.find(2) == test1.end()) ? 1 : 0;
			size_t test1Result4 = test1.size();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%i %i %i %llu", test1Result1, test1Result2, test1Result3, (unsigned long long)test1Result4);
			unitTest1.wantedResult = "1 1 1 7";
		}
		/* Test rehash */ {
			// Test 1
			unordered_set<int32_t> test1{ 1, 2, 3, 4, 5 };

			test1.rehash(3);

			int32_t test1Result1 = (test1.find(1) != test1.end()) ? 1 : 0;
			int32_t test1Result2 = (test1.find(2) != test1.end()) ? 1 : 0;
			int32_t test1Result3 = (test1.find(3) != test1.end()) ? 1 : 0;
			int32_t test1Result4 = (test1.find(4) != test1.end()) ? 1 : 0;
			int32_t test1Result5 = (test1.find(5) != test1.end()) ? 1 : 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "rehash 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1Result1, test1Result2, test1Result3, test1Result4, test1Result5);
			unitTest1.wantedResult = "1 1 1 1 1";
		}
	}

	WFE_ADD_UNIT_TEST_CALLBACK(UnorderedSetUnitTestCallback)
}