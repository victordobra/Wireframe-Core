#include "UnitTests.hpp"

namespace wfe {
	void VectorUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Vector";
		
		/* Test copying */ {
			// Test 1
			vector<int32_t> test1Src{ 1, 2, 3, 4, 5 };
			vector<int32_t> test1Dst{ 6, 7, 8 };

			test1Dst = test1Src;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "copying 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1Dst[0], test1Dst[1], test1Dst[2], test1Dst[3], test1Dst[4]);
			unitTest1.wantedResult = "1 2 3 4 5";
		}
		/* Test assign */ {
			// Test 1
			vector<int32_t> test1{ 1, 3, 5, 7, 9 };

			test1.assign({ 2, 3, 5, 7, 11 });

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "assign 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1[0], test1[1], test1[2], test1[3], test1[4]);
			unitTest1.wantedResult = "2 3 5 7 11";
			
			// Test 2
			vector<int32_t> test2{ 0, 2, 4, 6, 8 };

			test2.assign(5, 1);

			UnitTest& unitTest2 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest2.name = "assign 2";
			unitTest2.FormatResult("%i %i %i %i %i", test2[0], test2[1], test2[2], test2[3], test2[4]);
			unitTest2.wantedResult = "1 1 1 1 1";
		}
		/* Test push_back */ {
			// Test 1
			vector<int32_t> test1{ 1, 2, 3, 4 };

			test1.push_back(5);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "push_back 1";
			unitTest1.FormatResult("%i", test1[4]);
			unitTest1.wantedResult = "5";
		}
		/* Test pop_back */ {
			// Test 1
			vector<int32_t> test1{ 1, 2, 3, 4, 10 };

			test1.pop_back();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "pop_back 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1.size());
			unitTest1.wantedResult = "4";
		}
		/* Test insert */ {
			// Test 1
			vector<int32_t> test1{ 1, 3, 5, 9 };

			test1.insert(test1.begin() + 1, 2); // 1 2 3 5 9
			test1.insert(test1.begin() + 3, 3, 4); // 1 2 3 4 4 4 5 9
			test1.insert(test1.begin() + 7, { 6, 7, 8 }); // 1 2 3 4 4 4 5 6 7 8 9

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%i %i %i %i %i %i %i %i %i %i %i", test1[0], test1[1], test1[2], test1[3], test1[4], test1[5], test1[6], test1[7], test1[8], test1[9], test1[10]);
			unitTest1.wantedResult = "1 2 3 4 4 4 5 6 7 8 9";
		}
		/* Test erase */ {
			// Test 1
			vector<int32_t> test1{ 1, 0, 2, 7, 8, 9, 3, 4, 5 };

			test1.erase(test1.begin() + 1);
			test1.erase(test1.begin() + 2, test1.begin() + 5);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1[0], test1[1], test1[2], test1[3], test1[4]);
			unitTest1.wantedResult = "1 2 3 4 5";
		}
		/* Test swap */ {
			// Test 1
			vector<int32_t> test1Vec1{ 1, 3, 5, 7, 9 };
			vector<int32_t> test1Vec2{ 0, 2, 4, 6, 8 };

			test1Vec1.swap(test1Vec2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "swap 1";
			unitTest1.FormatResult("%i %i %i %i %i %i %i %i %i %i", test1Vec1[0], test1Vec1[1], test1Vec1[2], test1Vec1[3], test1Vec1[4], test1Vec2[0], test1Vec2[1], test1Vec2[2], test1Vec2[3], test1Vec2[4]);
			unitTest1.wantedResult = "0 2 4 6 8 1 3 5 7 9";
		}
		/* Test clear */ {
			// Test 1
			vector<int32_t> test1{ 1, 2, 3, 4, 5 };

			test1.clear();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "clear 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1.size());
			unitTest1.wantedResult = "0";
		}
		/* Test resize */ {
			// Test 1
			vector<int32_t> test1{ 1, 2, 3, 4, 5 };

			test1.resize(3);
			test1.resize(5, 4);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "resize 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1[0], test1[1], test1[2], test1[3], test1[4]);
			unitTest1.wantedResult = "1 2 3 4 4";
		}
		/* Test comparison */ {
			// Test 1
			vector<int32_t> test1Vec1{ 1, 2, 3, 4, 5 };
			vector<int32_t> test1Vec2{ 1, 2, 3, 4, 6 };

			int32_t test1ResultEqual = (int32_t)(test1Vec1 == test1Vec2); // 0
			int32_t test1ResultSmaller = (int32_t)(test1Vec1 < test1Vec2); // 1
			int32_t test1ResultBigger = (int32_t)(test1Vec1 > test1Vec2); // 0
			int32_t test1ResultSmallerEqual = (int32_t)(test1Vec1 <= test1Vec2); // 1
			int32_t test1ResultBiggerEqual = (int32_t)(test1Vec1 >= test1Vec2); // 0

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "comparison 1";
			unitTest1.FormatResult("%i %i %i %i %i", test1ResultEqual, test1ResultSmaller, test1ResultBigger, test1ResultSmallerEqual, test1ResultBiggerEqual);
			unitTest1.wantedResult = "0 1 0 1 0";
		}
	}
	
	WFE_ADD_UNIT_TEST_CALLBACK(VectorUnitTestCallback)
}