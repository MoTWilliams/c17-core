#include "test_mem.h"

#include "test_assert.h"

int test_mem(int* totTests, int* grpTests) {
        int grpPasses = 0;
        (*grpTests) = 0;

        grpPasses += test_safeMalloc(totTests, grpTests);
        grpPasses += test_safeCalloc(totTests, grpTests);
        grpPasses += test_safeRealloc(totTests, grpTests);

        return grpPasses;
}