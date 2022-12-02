#include "Tests.hpp"

namespace wfe {
    const char_t* StringTests() {
        // Test string::at
        string atTest = "at";

        char_t atResult = atTest.at(1); // t

        // Test string::front
        string frontTest = "front";

        char_t frontResult = frontTest.front(); // f

        // Test string::back
        string backTest = "back";

        char_t backResult = backTest.back(); // k

        // Test append functions
        string appendTest = "append";

        appendTest += " tested";
        appendTest += ' ';
        appendTest.append(frontTest);
        appendTest.append(backTest, 1, 2);
        appendTest.append(" with");
        appendTest.append(" an unit tester", 8);
        appendTest.append(5, 'a');
        appendTest.push_back('.');

        // append tested frontac with an unitaaaaa.

        // Test assign functions
        string assignTest1 = "assign";
        string assignTest2 = "assign";
        string assignTest3 = "assign";

        assignTest1.assign(frontTest, 1, 3); // ron
        assignTest2.assign("testing", 4); // test
        assignTest3.assign(10, 'b'); // bbbbbbbbbb

        // Test insert functions
        string insertTest = "insert";

        insertTest.insert(2, frontTest);
        insertTest.insert(7, backTest, 1);
        insertTest.insert(0, "un");
        insertTest.insert(9, " and tested", 5);
        insertTest.insert(10, 3, 'a');
        insertTest.insert(insertTest.begin() + 2, 3, 'i');
        insertTest.insert(insertTest.end() - 2, 'e');
        
        // uniiiinfront aaaand ackseert
        
        // Test erase functions
        string eraseTest = "erasing text is important";

        eraseTest.erase(17, 6);
        eraseTest.erase(eraseTest.end() - 1);
        eraseTest.erase(eraseTest.begin() + 4, eraseTest.begin() + 9);
        eraseTest.pop_back();

        // erasext is i

        // Test replace functions
        string replaceTest = "replace";

        replaceTest.replace(0, 2, frontTest);
        replaceTest.replace(replaceTest.begin() + 2, replaceTest.begin() + 3, frontTest);
        replaceTest.replace(11, 3, backTest, 1);
        replaceTest.replace(2, 3, "test");
        replaceTest.replace(replaceTest.begin() + 8, replaceTest.begin() + 10, "test");
        replaceTest.replace(6, 2, "and tested", 3);
        replaceTest.replace(replaceTest.begin() + 2, replaceTest.begin() + 6, "best engine", 4);
        replaceTest.replace(0, 2, 1, '-');
        replaceTest.replace(replaceTest.begin() + 5, replaceTest.begin() + 8, 5, '.');

        // -best.....testplack

        // Test string::swap
        string swapTest1 = "swap1";
        string swapTest2 = "swap2";

        swapTest1.swap(swapTest2);

        // Test string::copy
        string copyTest = "copy test";
        char_t copyTarget[5];

        copyTest.copy(copyTarget, 4, 2); // py t
        copyTarget[4] = 0;

        // Test the find functions
        string findTest = "find front in this front string";

        size_t findResult1 = findTest.find(frontTest); // 5
        size_t findResult2 = findTest.find("in", 3); // 11
        size_t findResult3 = findTest.find("inside", 0, 2); // 1
        size_t findResult4 = findTest.find('d'); // 3

        size_t rfindResult1 = findTest.rfind(frontTest); // 19
        size_t rfindResult2 = findTest.rfind("in"); // 28
        size_t rfindResult3 = findTest.rfind("onto", SIZE_T_MAX, 3); // 21
        size_t rfindResult4 = findTest.rfind('t'); // 26

        size_t ffoResult1 = findTest.find_first_of(frontTest); // 0
        size_t ffoResult2 = findTest.find_first_of("ront"); // 2
        size_t ffoResult3 = findTest.find_first_of("this", 0, 2); // 9
        size_t ffoResult4 = findTest.find_first_of('d'); // 3

        size_t floResult1 = findTest.find_last_of(frontTest); // 29
        size_t floResult2 = findTest.find_last_of(" ts"); // 26
        size_t floResult3 = findTest.find_last_of("fo ", SIZE_T_MAX, 2); // 21
        size_t floResult4 = findTest.find_last_of('t'); // 26

        size_t ffnoResult1 = findTest.find_first_not_of(frontTest); // 1
        size_t ffnoResult2 = findTest.find_first_not_of("find "); // 6
        size_t ffnoResult3 = findTest.find_first_not_of("findro ", 0, 4); // 4
        size_t ffnoResult4 = findTest.find_first_not_of('f'); // 1

        size_t flnoResult1 = findTest.find_last_not_of(frontTest); // 30
        size_t flnoResult2 = findTest.find_last_not_of("string "); // 21
        size_t flnoResult3 = findTest.find_last_not_of("stingr", SIZE_T_MAX, 5); // 27
        size_t flnoResult4 = findTest.find_last_not_of('g'); // 29

        // Test string::substr
        string substrTest = "get the substring of this string";
        string substrResult = substrTest.substr(8, 9); // substring

        // Test the compare functions
        string compareTest = "the string to compare";

        int32_t compareResult1 = compareTest.compare(frontTest); // 1
        int32_t compareResult2 = compareTest.compare(14, 7, frontTest); // -1
        int32_t compareResult3 = compareTest.compare(4, 6, frontTest, 1); // 1
        int32_t compareResult4 = compareTest.compare("string to compare"); // 1
        int32_t compareResult5 = compareTest.compare(4, 6, "string to compare"); // -1
        int32_t compareResult6 = compareTest.compare(4, 6, "string to compare", 6); // 0

        // Format the final string
        char_t result[8000];
        FormatString(result, 8000, "at: %c\nfront: %c\nback: %c\nappend: %s\nassign: %s %s %s\ninsert: %s\nerase: %s\nreplace: %s\nswap: %s %s\ncopy: %s\nfind: %llu %llu %llu %llu\nrfind: %llu %llu %llu %llu\nfind_first_of: %llu %llu %llu %llu\nfind_last_of: %llu %llu %llu %llu\nfind_first_not_of: %llu %llu %llu %llu\nfind_last_not_of: %llu %llu %llu %llu\nsubstr: %s\ncompare: %i %i %i %i %i %i\n", atResult, frontResult, backResult, appendTest.c_str(), assignTest1.c_str(), assignTest2.c_str(), assignTest3.c_str(), insertTest.c_str(), eraseTest.c_str(), replaceTest.c_str(), swapTest1.c_str(), swapTest2.c_str(), copyTarget, (unsigned long long)findResult1, (unsigned long long)findResult2, (unsigned long long)findResult3, (unsigned long long)findResult4, (unsigned long long)rfindResult1, (unsigned long long)rfindResult2, (unsigned long long)rfindResult3, (unsigned long long)rfindResult4, (unsigned long long)ffoResult1, (unsigned long long)ffoResult2, (unsigned long long)ffoResult3, (unsigned long long)ffoResult4, (unsigned long long)floResult1, (unsigned long long)floResult2, (unsigned long long)floResult3, (unsigned long long)floResult4, (unsigned long long)ffnoResult1, (unsigned long long)ffnoResult2, (unsigned long long)ffnoResult3, (unsigned long long)ffnoResult4, (unsigned long long)flnoResult1, (unsigned long long)flnoResult2, (unsigned long long)flnoResult3, (unsigned long long)flnoResult4, substrResult.c_str(), compareResult1, compareResult2, compareResult3, compareResult4, compareResult5, compareResult6);

        // Copy the final string to the heap
        size_t resultLength = strnlen(result, 8000) + 1;

        char_t* resultHeap = (char_t*)malloc(resultLength, MEMORY_USAGE_STRING);
        if(!resultHeap)
            WFE_LOG_FATAL("Failed to allocate string memory!");
        
        memcpy(resultHeap, result, resultLength);

        // Return the heap string
        return resultHeap;
    }
    const char_t* StringTestsResult() {
        return "at: t\nfront: f\nback: k\nappend: append tested frontac with an unitaaaaa.\nassign: ron test bbbbbbbbbb\ninsert: uniiiinfront aaaand ackseert\nerase: erasext is i\nreplace: -best.....testplack\nswap: swap2 swap1\ncopy: py t\nfind: 5 11 1 3\nrfind: 19 28 21 26\nfind_first_of: 0 2 9 3\nfind_last_of: 29 26 21 26\nfind_first_not_of: 1 6 4 1\nfind_last_not_of: 30 21 27 29\nsubstr: substring\ncompare: 1 -1 1 1 -1 0\n";
    }
}