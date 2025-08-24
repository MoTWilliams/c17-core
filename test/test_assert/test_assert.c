#include "test_assert.h"

#include "textStylesAndColors.h"
#include <stdio.h>

TestRestult test_assert(
                Bool condition, const char* msg, const char* filePath,
                int lineNo, int* totTests, int* grpTests) {
        /* Add 1 to the total and group test counts */
        (*totTests)++;
        (*grpTests)++;

        /* Print FAIL message in red if the test fails */
        if (!condition) {
                fprintf(stderr, FG_RED "[" BOLD "FAIL" UNBOLD "]-----");
                fprintf(stderr,
                        "%s on LINE: %d of FILE: \"%s\"" RESET "\n",
                        msg, lineNo, filePath
                );
                return TEST_FAIL;
        }

        /* Otherwise, print PASS message in green if the test passes */
        printf(FG_GRN "[" BOLD "PASS" UNBOLD "]-----");
        printf("%s in FILE: \"%s\"" RESET "\n", msg, filePath);
        return TEST_PASS;
}