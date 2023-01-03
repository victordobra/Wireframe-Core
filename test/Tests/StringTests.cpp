#include "UnitTests.hpp"

namespace wfe {
	void StringUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "String";

		/* Test append */ {
			// Test 1
			string test1Str1 = "front", test1Str2 = "back";

			string test1 = "append";

			test1 += " tested";
			test1 += ' ';
			test1.append(test1Str1);
			test1.append(test1Str2, 1, 2);
			test1.append(" with");
			test1.append(" an unit tester", 8);
			test1.append(5, 'a');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "append 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "append tested frontac with an unitaaaaa";
		}
		/* Test push_back */ {
			// Test 1
			string test1 = "test";

			test1.push_back('.');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "push_back 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "test.";
		}
		/* Test assign */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "assign";

			test1.assign(test1Str1, 1, 3);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "assign 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "ron";

			// Test 2
			string test2 = "assign";

			test2.assign("testing", 4);

			UnitTest& unitTest2 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest2.name = "assign 2";
			unitTest2.FormatResult("%s", test2.c_str());
			unitTest2.wantedResult = "test";

			// Test 3
			string test3 = "assign";

			test3.assign(10, 'b');

			UnitTest& unitTest3 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest2.name = "assign 3";
			unitTest2.FormatResult("%s", test3.c_str());
			unitTest2.wantedResult = "bbbbbbbbbb";
		}
		/* Test insert */ {
			// Test 1
			string test1Str1 = "front", test1Str2 = "back";

			string test1 = "insert";

			test1.insert(2, test1Str1);
			test1.insert(7, test1Str2, 1);
			test1.insert(0, "un");
			test1.insert(9, " and tested", 5);
			test1.insert(10, 3, 'a');
			test1.insert(test1.begin() + 2, 3, 'i');
			test1.insert(test1.end() - 2, 'e');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "insert 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "uniiiinfront aaaand ackseert";
		}
		/* Test erase */ {
			// Test 1
			string test1 = "erasing text is important";

			test1.erase(17, 6);
			test1.erase(test1.end() - 1);
			test1.erase(test1.begin() + 4, test1.begin() + 9);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "erase 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "erasext is in";
		}
		/* Test pop_back */ {
			// Test 1
			string test1 = "test.";

			test1.pop_back();

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "pop_back 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "test";
		}
		/* Test replace */ {
			// Test 1
			string test1Str1 = "front", test1Str2 = "back";

			string test1 = "replace";

			test1.replace(0, 2, test1Str1);
			test1.replace(test1.begin() + 2, test1.begin() + 3, test1Str1);
			test1.replace(11, 3, test1Str2, 1);
			test1.replace(2, 3, "test");
			test1.replace(test1.begin() + 8, test1.begin() + 10, "test");
			test1.replace(6, 2, "and tested", 3);
			test1.replace(test1.begin() + 2, test1.begin() + 6, "test string", 4);
			test1.replace(0, 2, 1, '-');
			test1.replace(test1.begin() + 5, test1.begin() + 8, 5, '.');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "replace 1";
			unitTest1.FormatResult("%s", test1.c_str());
			unitTest1.wantedResult = "-test.....testplack";
		}
		/* Test swap */ {
			// Test 1
			string test1Str1 = "swap1";
			string test1Str2 = "swap2";

			test1Str1.swap(test1Str2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "swap 1";
			unitTest1.FormatResult("%s %s", test1Str1.c_str(), test1Str2.c_str());
			unitTest1.wantedResult = "swap2 swap1";
		}
		/* Test copy */ {
			// Test 1
			string test1Src = "copy test";
			char_t test1Dst[5];

			test1Src.copy(test1Dst, 4, 2);
			test1Dst[4] = 0;

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "copy 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "py t";
		}
		/* Test find */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.find(test1Str1);
			size_t test1Result2 = test1.find("in", 3);
			size_t test1Result3 = test1.find("inside", 0, 2);
			size_t test1Result4 = test1.find('d');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "5 11 1 3";
		}
		/* Test rfind */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.rfind(test1Str1);
			size_t test1Result2 = test1.rfind("in");
			size_t test1Result3 = test1.rfind("onto", SIZE_T_MAX, 3);
			size_t test1Result4 = test1.rfind('t');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "rfind 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "19 28 21 26";
		}
		/* Test find_first_of */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.find_first_of(test1Str1);
			size_t test1Result2 = test1.find_first_of("ront");
			size_t test1Result3 = test1.find_first_of("this", 0, 2);
			size_t test1Result4 = test1.find_first_of('d');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find_first_of 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "0 2 9 3";
		}
		/* Test find_last_of */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.find_last_of(test1Str1);
			size_t test1Result2 = test1.find_last_of(" ts");
			size_t test1Result3 = test1.find_last_of("fo ", SIZE_T_MAX, 2);
			size_t test1Result4 = test1.find_last_of('t');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find_last_of 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "29 26 21 26";
		}
		/* Test find_first_not_of */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.find_first_not_of(test1Str1);
			size_t test1Result2 = test1.find_first_not_of("find ");
			size_t test1Result3 = test1.find_first_not_of("findro ", 0, 4);
			size_t test1Result4 = test1.find_first_not_of('f');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find_first_not_of 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "1 6 4 1";
		}
		/* Test find_last_not_of */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "find front in this front string";

			size_t test1Result1 = test1.find_last_not_of(test1Str1);
			size_t test1Result2 = test1.find_last_not_of("string ");
			size_t test1Result3 = test1.find_last_not_of("stingr", SIZE_T_MAX, 5);
			size_t test1Result4 = test1.find_last_not_of('g');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "find_last_not_of 1";
			unitTest1.FormatResult("%llu %llu %llu %llu", (unsigned long long)test1Result1,(unsigned long long)test1Result2,(unsigned long long)test1Result3,(unsigned long long)test1Result4);
			unitTest1.wantedResult = "30 21 27 29";
		}
		/* Test substr */ {
			// Test 1
			string test1 = "get the substring of this string";

			string test1Result = test1.substr(8, 9);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "substr 1";
			unitTest1.FormatResult("%s", test1Result.c_str());
			unitTest1.wantedResult = "substring";
		}
		/* Test compare */ {
			// Test 1
			string test1Str1 = "front";

			string test1 = "the string to compare";

			int32_t test1Result1 = test1.compare(test1Str1);
			int32_t test1Result2 = test1.compare(14, 7, test1Str1);
			int32_t test1Result3 = test1.compare(4, 6, test1Str1, 1);
			int32_t test1Result4 = test1.compare("string to compare");
			int32_t test1Result5 = test1.compare(4, 6, "string to compare");
			int32_t test1Result6 = test1.compare(4, 6, "string to compare", 6);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "compare 1";
			unitTest1.FormatResult("%i %i %i %i %i %i", test1Result1, test1Result2, test1Result3, test1Result4, test1Result5, test1Result6);
			unitTest1.wantedResult = "1 -1 1 1 -1 0";
		}
	}

	WFE_ADD_UNIT_TEST_CALLBACK(StringUnitTestCallback)
}