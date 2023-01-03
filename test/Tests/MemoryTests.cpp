#include "UnitTests.hpp"

namespace wfe {
	void MemoryUnitTestCallback(UnitTestList& unitTestList) {
		unitTestList.name = "Memory";

		/* Test memcpy */ {
			// Test 1
			char_t test1Src[7] = "memcpy";
			char_t test1Dst[7];

			memcpy(test1Dst, test1Src, 7);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memcpy 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "memcpy";
		}
		/* Test memmove */ {
			// Test 1
			char_t test1[] = "memmove";

			memmove(test1 + 1, test1, 3);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memmove 1";
			unitTest1.FormatResult("%s", test1);
			unitTest1.wantedResult = "mmemove";
		}
		/* Test memccpy */ {
			// Test 1
			char_t test1Src[8] = "memccpy";
			char_t test1Dst[8] = "   test";

			memccpy(test1Dst, test1Src, 'c', 8);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memccpy 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "memcest";
		}
		/* Test memset */ {
			// Test 1
			char_t test1[] = "memset";

			memset(test1, 'e', 3);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memset 1";
			unitTest1.FormatResult("%s", test1);
			unitTest1.wantedResult = "eeeset";
		}
		/* Test memcmp */ {
			// Test 1
			char_t test1Str1[] = "aaaa";
			char_t test1Str2[] = "aaab";

			int32_t test1Result = memcmp(test1Str1, test1Str2, 4);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memcmp 1";
			unitTest1.FormatResult("%i", test1Result);
			unitTest1.wantedResult = "-1";
		}
		/* Test memchr */ {
			// Test 1
			char_t test1[] = "memchr";

			char_t* test1Result = (char_t*)memchr(test1, 'c', 6);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memchr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "chr";
		}
		/* Test rawmemchr */ {
			// Test 1
			char_t test1[] = "rawmemchr";

			char_t* test1Result = (char_t*)rawmemchr(test1, 'c');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "rawmemchr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "chr";
		}
		/* Test memrchr */ {
			// Test 1
			char_t test1[] = "memrchr";

			char_t* test1Result = (char_t*)memrchr(test1, 'm', 7);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "memrchr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "mrchr";
		}
		/* Test strcpy */ {
			// Test 1
			char_t test1Src[7] = "strcpy";
			char_t test1Dst[7];

			strcpy(test1Dst, test1Src);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strcpy 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "strcpy";
		}
		/* Test strncpy */ {
			// Test 1
			char_t test1Src[13] = "strncpy test";
			char_t test1Dst[13] = "aaaaaaaaaaaa";

			strncpy(test1Dst, test1Src, 7);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strncpy 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "strncpyaaaaa";
		}
		/* Test strcat */ {
			// Test 1
			char_t test1Src[7] = "cat";
			char_t test1Dst[7] = "str";

			strcat(test1Dst, test1Src);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strcat 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "strcat";
		}
		/* Test strncat */ {
			// Test 1
			char_t test1Src[13] = "ncat test";
			char_t test1Dst[13] = "str";

			strncat(test1Dst, test1Src, 4);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strncat 1";
			unitTest1.FormatResult("%s", test1Dst);
			unitTest1.wantedResult = "strncat";
		}
		/* Test strcmp */ {
			// Test 1
			char_t test1Str1[] = "aaaa";
			char_t test1Str2[] = "aaaaa";

			int32_t test1Result = strcmp(test1Str1, test1Str2);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strcmp 1";
			unitTest1.FormatResult("%i", test1Result);
			unitTest1.wantedResult = "-1";
		}
		/* Test strncmp */ {
			// Test 1
			char_t test1Str1[] = "aaaa";
			char_t test1Str2[] = "aaaaa";

			int32_t test1Result = strncmp(test1Str1, test1Str2, 4);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strncmp 1";
			unitTest1.FormatResult("%i", test1Result);
			unitTest1.wantedResult = "0";
		}
		/* Test strchr */ {
			// Test 1
			char_t test1[] = "strchr";

			char_t* test1Result = strchr(test1, 'h');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strchr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "hr";
		}
		/* Test strrchr */ {
			// Test 1
			char_t test1[] = "strrchr";

			char_t* test1Result = strrchr(test1, 'r');

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strrchr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "r";
		}
		/* Test strcspn */ {
			// Test 1
			char_t test1[] = "strcspn";

			size_t test1Result = strcspn(test1, "cr");

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strcspn 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1Result);
			unitTest1.wantedResult = "2";
		}
		/* Test strspn */ {
			// Test 1
			char_t test1[] = "strspn";

			size_t test1Result = strspn(test1, "str");

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strspn 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1Result);
			unitTest1.wantedResult = "4";
		}
		/* Test strpbrk */ {
			// Test 1
			char_t test1[] = "strpbrk";

			char_t* test1Result = strpbrk(test1, "bkr");

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strpbrk 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "rpbrk";
		}
		/* Test strstr */ {
			// Test 1
			char_t test1[] = "strstr";

			char_t* test1Result = strstr(test1, "trst");

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strstr 1";
			unitTest1.FormatResult("%s", test1Result);
			unitTest1.wantedResult = "trstr";
		}
		/* Test strtok */ {
			// Test 1
			char_t test1[] = "- strtok; test";

			char_t* test1Result1 = strtok(test1, "- ;");
			char_t* test1Result2 = strtok(nullptr, "- ;");

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strtok 1";
			unitTest1.FormatResult("%s %s", test1Result1, test1Result2);
			unitTest1.wantedResult = "strtok test";
		}
		/* Test strlen */ {
			// Test 1
			char_t test1[] = "strlen";

			size_t test1Result = strlen(test1);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strlen 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1Result);
			unitTest1.wantedResult = "6";
		}
		/* Test strnlen */ {
			// Test 1
			char_t test1[] = "strnlen";

			size_t test1Result = strnlen(test1, 6);

			UnitTest& unitTest1 = unitTestList.unitTests[unitTestList.unitTestCount++];

			unitTest1.name = "strnlen 1";
			unitTest1.FormatResult("%llu", (unsigned long long)test1Result);
			unitTest1.wantedResult = "6";
		}
	}
	
	WFE_ADD_UNIT_TEST_CALLBACK(MemoryUnitTestCallback)
}