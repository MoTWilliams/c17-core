#include "test_assert.h"
#include "test_mem.h"

#include "textStylesAndColors.h"
#include <stdio.h>

typedef enum {
        TESTS,
        PASSES,
        NUM_SCORES
} GroupScores;

typedef enum {
        GRP_MEM,
        NUM_GROUPS      /* This will scale as more groups are added */
} TestGroup;

int main(void) {
        /* Total tests and passed tests per group */
        int scores[NUM_GROUPS][NUM_SCORES] = {0};

        /* Group names */
        static const char* grpNames[NUM_GROUPS] = {""};

        int totTests = 0;
        int totPasses = 0;
        TestGroup grp;

        /* grpTests is incremented by TEST_ASSERT, which is called inside each 
         * function's unit tests. */
        int grpTests = 0;

        printf(FG_BLU "Running " BOLD "tests" UNBOLD " on MASh\n\n" RESET);
        fflush(stdout);

        /* --- RUN UNIT TESTS --- */

        /* Test memory handling module*/
        printf("Memory tests:\n");
        fflush(stdout);
        grpNames[GRP_MEM] = "memory";
        scores[GRP_MEM][PASSES] = test_mem(&totTests, &grpTests);
        scores[GRP_MEM][TESTS] = grpTests;
        printf("\n");
        fflush(stdout);

        /* --- SUMMARY --- */
        printf("-----SUMMARY-----\n");
        fflush(stdout);
        
        for (grp = GRP_MEM; grp < NUM_GROUPS; grp++) {
                totPasses += scores[grp][PASSES];
                printf(
                        "Failed %d of %d %s tests\n",
                        scores[grp][TESTS] - scores[grp][PASSES],
                        scores[grp][TESTS], grpNames[grp]
                );
        }

        printf("\nFailed %d of %d total tests\n\n", 
                totTests - totPasses, totTests);

        return 0;
}