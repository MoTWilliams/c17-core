#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "test_assert.h"

#include "handleErrors.h"

/**
 * Run the test operation in a child process to avoid terminating the test
 * suite on crash or intentional fatal failure
 */
typedef enum {
        EXPECT_OP_FAILURE,
        EXPECT_OP_SUCCESS
} OperationExpect;

typedef enum {
        OP_FAILURE,
        OP_SUCCESS
} OperationResult;

TestResult runTestInChildProcess(
        char* testName, OperationResult (*operation)(const ErrorFatality), 
        OperationExpect expect, ErrorFatality isFailureFatal
);

#endif