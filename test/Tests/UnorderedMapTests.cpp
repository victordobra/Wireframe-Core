#include "UnitTests.hpp"

namespace wfe {
	void UnorderedMapUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Unordered map";

		/* Test operator[] */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "test" }, { 3, "three" } };

			test1[2] = "two";
			test1[4] = "four";

			const char_t* test1Result1 = test1[1];
			const char_t* test1Result2 = test1[2];
			const char_t* test1Result3 = test1[3];
			const char_t* test1Result4 = test1[4];

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "operator[] 1";
			unitTest1.FormatResult("%s %s %s %s", test1Result1, test1Result2, test1Result3, test1Result4);
			unitTest1.wantedResult = "one two three four";
		}
		/* Test at */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };

			const char_t* test1Result1 = test1.at(2);
			const char_t* test1Result2 = test1.at(5);
			const char_t* test1Result3 = test1.at(1);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "at 1";
			unitTest1.FormatResult("%s %s %s", test1Result1, test1Result2, test1Result3);
			unitTest1.wantedResult = "two five one";
		}
		/* Test find */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };

			const char_t* test1Result1 = test1.find(2)->second;
			const char_t* test1Result2 = test1.find(5)->second;
			int32_t test1Result3 = (test1.find(8) == test1.end()) ? 1 : 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find 1";
			unitTest1.FormatResult("%s %s %i", test1Result1, test1Result2, test1Result3);
			unitTest1.wantedResult = "two five 1";
		}
		/* Test count */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };

			size_t test1Result1 = test1.count(3);
			size_t test1Result2 = test1.count(7);
			size_t test1Result3 = test1.count(2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "count 1";
			unitTest1.FormatResult("%llu %llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2, (unsigned long long)test1Result3);
			unitTest1.wantedResult = "1 0 1";
		}
		/* Test equal_range */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };
		
			const char_t* test1Result1 = test1.equal_range(4).first->second;
			int32_t test1Result2 = (test1.equal_range(6).first == test1.end()) ? 1 : 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "equal_range 1";
			unitTest1.FormatResult("%s %i", test1Result1, test1Result2);
			unitTest1.wantedResult = "four 1";
		}
		/* Test insert */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 4, "four" } };

			test1.insert({ 3, "three" });
			test1.insert({ { 2, "two" }, { 5, "five" } });

			const char_t* test1Result1 = test1.at(1);
			const char_t* test1Result2 = test1.at(2);
			const char_t* test1Result3 = test1.at(3);
			const char_t* test1Result4 = test1.at(4);
			const char_t* test1Result5 = test1.at(5);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%s %s %s %s %s", test1Result1, test1Result2, test1Result3, test1Result4, test1Result5);
			unitTest1.wantedResult = "one two three four five";
		}
		/* Test erase */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };

			test1.erase(test1.find(2));
			test1.erase(4);

			size_t test1Result1 = test1.count(2);
			size_t test1Result2 = test1.count(4);
			size_t test1Result3 = test1.size();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%llu %llu %llu", (unsigned long long)test1Result1, (unsigned long long)test1Result2, (unsigned long long)test1Result3);
			unitTest1.wantedResult = "0 0 3";
		}
		/* Test rehash */ {
			// Test 1
			unordered_map<uint32_t, const char_t*> test1{ { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } };

			test1.rehash(3);

			const char_t* test1Result1 = test1.at(1);
			const char_t* test1Result2 = test1.at(2);
			const char_t* test1Result3 = test1.at(3);
			const char_t* test1Result4 = test1.at(4);
			const char_t* test1Result5 = test1.at(5);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "rehash 1";
			unitTest1.FormatResult("%s %s %s %s %s", test1Result1, test1Result2, test1Result3, test1Result4, test1Result5);
			unitTest1.wantedResult = "one two three four five";
		}
	}

	WFE_ADD_UNIT_TEST_CALLBACK(UnorderedMapUnitTestCallback)
}