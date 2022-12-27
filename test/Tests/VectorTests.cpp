#include "Tests.hpp"

namespace wfe {
    const char_t* VectorTests() {
        // Test the copy functions
        vector<int32_t> copySrc{ 1, 2, 3, 4, 5 };
        vector<int32_t> copyDst{ 6, 7, 8 };

        copyDst = copySrc;

        // Test assign
        vector<int32_t> assignTest1{ 1, 3, 5, 7, 9 };
        vector<int32_t> assignTest2{ 0, 2, 4, 6, 8 };

        assignTest1.assign({ 2, 3, 5, 7 }); // 2 3 5 7
        assignTest2.assign(7, 1); // 1 1 1 1 1 1 1

        // Test push_back
        vector<int32_t> pushBackTest{ 1, 2, 3, 4 };

        pushBackTest.push_back(5); // 1 2 3 4 5

        // Test pop_back
        vector<int32_t> popBackTest{ 1, 2, 3, 4, 10 };

        popBackTest.pop_back(); // 1 2 3 4

        // Test insert
        vector<int32_t> insertTest{ 1, 3, 5, 9 };

        insertTest.insert(insertTest.begin() + 1, 2); // 1 2 3 5 9
        insertTest.insert(insertTest.begin() + 3, 3, 4); // 1 2 3 4 4 4 5 9
        insertTest.insert(insertTest.begin() + 7, { 6, 7, 8 }); // 1 2 3 4 4 4 5 6 7 8 9

        // Test erase
        vector<int32_t> eraseTest{ 1, 0, 2, 7, 8, 9, 3, 4, 5 };

        eraseTest.erase(eraseTest.begin() + 1); // 1 2 7 8 9 3 4 5
        eraseTest.erase(eraseTest.begin() + 2, eraseTest.begin() + 5); // 1 2 3 4 5

        // Test swap
        vector<int32_t> swapTest1{ 1, 3, 5, 7, 9 };
        vector<int32_t> swapTest2{ 0, 2, 4, 6, 8 };

        swapTest1.swap(swapTest2);

        // Test clear
        vector<int32_t> clearTest{ 1, 2, 3, 4, 5 };

        clearTest.clear();

        // Test resize
        vector<int32_t> resizeTest{ 1, 2, 3, 4, 5 };

        resizeTest.resize(3); // 1 2 3
        resizeTest.resize(5, 4); // 1 2 3 4 4

        // Test comparison
        vector<int32_t> compareTest1{ 1, 2, 3, 4, 5 };
        vector<int32_t> compareTest2{ 1, 2, 3, 4, 6 };

        int32_t resultEqual = (int32_t)(compareTest1 == compareTest2); // 0
        int32_t resultSmaller = (int32_t)(compareTest1 < compareTest2); // 1
        int32_t resultBigger = (int32_t)(compareTest1 > compareTest2); // 0
        int32_t resultSmallerEqual = (int32_t)(compareTest1 <= compareTest2); // 1
        int32_t resultBiggerEqual = (int32_t)(compareTest1 >= compareTest2); // 0

        // Format the final string
        char_t result[8000];
        FormatString(result, 8000, "copy: %i %i %i\nassign: %i %i\npush_back: %llu\npop_back: %llu\ninsert: %i %i %i %i %i %i %i %i %i %i %i\nerase: %i %i %i %i %i\nswap: %i %i\nclear: %llu\nresize: %i\ncompare: %i %i %i %i %i\n", copyDst[0], copyDst[1], copyDst[2], assignTest1[0], assignTest2[0], (unsigned long long)pushBackTest.size(), (unsigned long long)popBackTest.size(), insertTest[0], insertTest[1], insertTest[2], insertTest[3], insertTest[4], insertTest[5], insertTest[6], insertTest[7], insertTest[8], insertTest[9], insertTest[10], eraseTest[0], eraseTest[1], eraseTest[2], eraseTest[3], eraseTest[4], swapTest1[0], swapTest2[0], (unsigned long long)clearTest.size(), resizeTest[4], resultEqual, resultSmaller, resultBigger, resultSmallerEqual, resultBiggerEqual);

        // Copy the final string to the heap
        size_t resultLength = strnlen(result, 8000) + 1;

        char_t* resultHeap = (char_t*)malloc(resultLength, MEMORY_USAGE_STRING);
        if(!resultHeap)
            WFE_LOG_FATAL("Failed to allocate string memory!");
        
        memcpy(resultHeap, result, resultLength);

        // Return the heap string
        return resultHeap;
    }
    const char_t* VectorTestsResult() {
        return "copy: 1 2 3\nassign: 2 1\npush_back: 5\npop_back: 4\ninsert: 1 2 3 4 4 4 5 6 7 8 9\nerase: 1 2 3 4 5\nswap: 0 1\nclear: 0\nresize: 4\ncompare: 0 1 0 1 0\n";
    }
}