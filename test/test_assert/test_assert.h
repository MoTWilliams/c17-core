#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include "booleanValues.h"

typedef enum {
        TEST_FAIL,
        TEST_PASS
} TestResult;

TestResult test_assert(
        Bool condition, const char* msg, const char* filePath,
        int lineNo, int* totTests, int* grpTests
);

#define TEST_ASSERT(condition, msg) \
        (test_assert(condition, msg, __FILE__, __LINE__, totTests, grpTests))

#endif