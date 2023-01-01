#include "Tests.hpp"

namespace wfe {
	const char_t* MemoryTests() {
		// Test memcpy
		char_t memcpySource[7] = "memcpy";
		char_t memcpyTarget[7];

		memcpy(memcpyTarget, memcpySource, 7);

		// Test memmove
		char_t memmoveTest[] = "memmove";

		memmove(memmoveTest + 1, memmoveTest, 3);

		// Test memccpy
		char_t memccpySource[8] = "memccpy";
		char_t memccpyTarget[8] = "   test";

		memccpy(memccpyTarget, memccpySource, 'c', 8);

		// Test memset
		char_t memsetTest[] = "memset";

		memset(memsetTest, 'e', 3);

		// Test memcmp
		char_t memcmpTest1[] = "aaaa";
		char_t memcmpTest2[] = "aaab";

		int32_t memcmpResult = memcmp(memcmpTest1, memcmpTest2, 4);

		// Test memchr
		char_t memchrTest[] = "memchr";

		char_t* memchrResult = (char_t*)memchr(memchrTest, 'c', 6);

		// Test rawmemchr
		char_t rawmemchrTest[] = "rawmemchr";

		char_t* rawmemchrResult = (char_t*)rawmemchr(rawmemchrTest, 'c');

		// Test memrchr
		char_t memrchrTest[] = "memrchr";

		char_t* memrchrResult = (char_t*)memrchr(memrchrTest, 'm', 7);

		// Test strcpy
		char_t strcpySource[16] = "strcpy\0unwanted";
		char_t strcpyTarget[16];

		strcpy(strcpyTarget, strcpySource);

		// Test strncpy
		char_t strncpySource[13] = "strncpy test";
		char_t strncpyTarget[13] = "aaaaaaaaaaaa";

		strncpy(strncpyTarget, strncpySource, 7);

		// Test strcat
		char_t strcatSource[7] = "cat";
		char_t strcatTarget[7] = "str";

		strcat(strcatTarget, strcatSource);

		// Test strncat
		char_t strncatSource[13] = "ncat test";
		char_t strncatTarget[13] = "str";

		strncat(strncatTarget, strncatSource, 4);

		// Test strcmp
		char_t strcmpTest1[] = "aaaa";
		char_t strcmpTest2[] = "aaaaa";

		int32_t strcmpResult = strcmp(strcmpTest1, strcmpTest2);

		// Test strncmp
		char_t strncmpTest1[] = "aaaa";
		char_t strncmpTest2[] = "aaaaa";

		int32_t strncmpResult = strncmp(strncmpTest1, strncmpTest2, 4);

		// Test strchr
		char_t strchrTest[] = "strchr";

		char_t* strchrResult = strchr(strchrTest, 'h');

		// Test strrchr
		char_t strrchrTest[] = "strrchr";

		char_t* strrchrResult = strrchr(strrchrTest, 'r');

		// Test strcspn
		char_t strcspnTest[] = "strcspn";

		size_t strcspnResult = strcspn(strcspnTest, "cr");

		// Test strspn
		char_t strspnTest[] = "strspn";

		size_t strspnResult = strspn(strspnTest, "str");

		// Test strpbrk
		char_t strpbrkTest[] = "strpbrk";

		char_t* strpbrkResult = strpbrk(strpbrkTest, "bkr");

		// Test strstr
		char_t strstrTest[] = "strstr";

		char_t* strstrResult = strstr(strstrTest, "trst");

		// Test strtok
		char_t strtokTest[] = "- strtok; test";

		char_t* strtokResult = strtok(strtokTest, "- ;");

		// Test strlen
		char_t strlenTest[] = "strlen";

		size_t strlenResult = strlen(strlenTest);

		// Test strnlen
		char_t strnlenTest[] = "strnlen";

		size_t strnlenResult = strnlen(strnlenTest, 6);

		// Format the final string
		char_t result[8000];
		FormatString(result, 8000, "memcpy: %s\nmemmove: %s\nmemccpy: %s\nmemset: %s\nmemcmp: %i\nmemchr: %s\nrawmemchr: %s\nmemrchr: %s\nstrcpy: %s\nstrncpy: %s\nstrcat: %s\nstrncat: %s\nstrcmp: %i\nstrncmp: %i\nstrchr: %s\nstrrchr: %s\nstrcspn: %llu\nstrspn: %llu\nstrpbrk: %s\nstrstr: %s\nstrtok: %s\nstrlen: %llu\nstrnlen: %llu\n", memcpyTarget, memmoveTest, memccpyTarget, memsetTest, memcmpResult, memchrResult, rawmemchrResult, memrchrResult, strcpyTarget, strncpyTarget, strcatTarget, strncatTarget, strcmpResult, strncmpResult, strchrResult, strrchrResult, (unsigned long long)strcspnResult, (unsigned long long)strspnResult, strpbrkResult, strstrResult, strtokResult, (unsigned long long)strlenResult, (unsigned long long)strnlenResult);

		// Copy the final string to the heap
		size_t resultLength = strnlen(result, 8000) + 1;

		char_t* resultHeap = (char_t*)malloc(resultLength, MEMORY_USAGE_STRING);
		if(!resultHeap)
			WFE_LOG_FATAL("Failed to allocate string memory!");
		
		memcpy(resultHeap, result, resultLength);

		// Return the heap string
		return resultHeap;
	}
	const char_t* MemoryTestsResult() {
		return "memcpy: memcpy\nmemmove: mmemove\nmemccpy: memcest\nmemset: eeeset\nmemcmp: -1\nmemchr: chr\nrawmemchr: chr\nmemrchr: mrchr\nstrcpy: strcpy\nstrncpy: strncpyaaaaa\nstrcat: strcat\nstrncat: strncat\nstrcmp: -1\nstrncmp: 0\nstrchr: hr\nstrrchr: r\nstrcspn: 2\nstrspn: 4\nstrpbrk: rpbrk\nstrstr: trstr\nstrtok: strtok\nstrlen: 6\nstrnlen: 6\n";
	}
}