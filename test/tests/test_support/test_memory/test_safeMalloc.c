#include "mem.h"
#include "test_mem.h"

#include "test_assert.h"
#include "test_util.h"
#include <sys/resource.h>
#include <string.h>
#include <stdio.h>

/* ----- HELPERS ----- */

/* ERR_NONFATAL mode */

/* Trigger OOM error and return NULL */
static TestResult test_safeMalloc_OOM_NONFATAL(void);

/* Trigger impossible size error and return NULL */
static TestResult test_safeMalloc_sizeTooLarge_NONFATAL(void);

/* Simply return NULL on zero allocation */
static TestResult test_safeMalloc_zeroAlloc_NONFATAL(void);

/* Return non-NULL on non-zero allocation that does not overflow or OOM */
static TestResult test_safeMalloc_normalAlloc_NONFATAL(void);

/* Return non-NULL on very large allocation that does not overflow or OOM */
static TestResult test_safeMalloc_largeAlloc_NONFATAL(void);

/* Check that every element is initialized to NULL/zero */
static TestResult test_safeMalloc_memUsable_NONFATAL(void);

/* ERR_FATAL mode */

/* Trigger OOM error and crash */
static TestResult test_safeMalloc_OOM_FATAL(void);

/* Trigger impossible size error and crash */
static TestResult test_safeMalloc_sizeTooLarge_FATAL(void);

/* Simply return NULL on zero allocation */
static TestResult test_safeMalloc_zeroAlloc_FATAL(void);

/* Return non-NULL on non-zero allocation that does not overflow or OOM */
static TestResult test_safeMalloc_normalAlloc_FATAL(void);

/* Return non-NULL on very large allocation that does not overflow or OOM */
static TestResult test_safeMalloc_largeAlloc_FATAL(void);

/* Check that every element is initialized to NULL/zero */
static TestResult test_safeMalloc_memUsable_FATAL(void);

/* ----- safeMalloc() UNIT TESTS ----- */
int test_safeMalloc(int* totTests, int* grpTests) {
        int grpPasses = 0;
        
        /* Nonfatal failure mode */
        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_OOM_NONFATAL(),
                "test_safeMalloc_OOM_NONFATAL(); safeMalloc() == NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_sizeTooLarge_NONFATAL(),
                "test_safeMalloc_sizeTooLarge_NONFATAL(); safeMalloc() == NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_zeroAlloc_NONFATAL(),
                "test_safeMalloc_zeroAlloc_NONFATAL(); safeMalloc() == NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_normalAlloc_NONFATAL(),
                "test_safeMalloc_normalAlloc_NONFATAL(); safeMalloc() != NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_largeAlloc_NONFATAL(),
                "test_safeMalloc_largeAlloc_NONFATAL(); safeMalloc() != NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_memUsable_NONFATAL(),
                "test_safeMalloc_memUsable_NONFATAL(); all ptr[i] write/read"
        );

        /* Fatal failure mode */
        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_OOM_FATAL(), 
                "test_safeMalloc_OOM_FATAL(); safeMalloc() abort"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_sizeTooLarge_FATAL(), "test_safeMalloc_sizeTooLarge_FATAL(); safeMalloc() abort"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_zeroAlloc_FATAL(),
                "test_safeMalloc_zeroAlloc_FATAL(); safeMalloc() == NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_normalAlloc_FATAL(),
                "test_safeMalloc_normalAlloc_FATAL(); safeMalloc() != NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_largeAlloc_FATAL(),
                "test_safeMalloc_largeAlloc_NONFATAL(); safeMalloc() != NULL"
        );

        grpPasses += TEST_ASSERT(
                (Bool)test_safeMalloc_memUsable_FATAL(),
                "test_safeMalloc_memUsable_FATAL(); safeMalloc() abort"
        );

        printf("\n");
        fflush(stdout);
        return grpPasses;
}

/* ----- OOM ALLOCATION FAILURE ----- */
static OperationResult safeMalloc_OOM_OPERATION(ErrorFatality isFailureFatal);

static TestResult test_safeMalloc_OOM_NONFATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_OOM_NONFATAL()", safeMalloc_OOM_OPERATION,
                EXPECT_OP_FAILURE, ERR_NONFATAL
        );
}

static TestResult test_safeMalloc_OOM_FATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_OOM_FATAL()", safeMalloc_OOM_OPERATION,
                EXPECT_OP_FAILURE, ERR_FATAL
        );
}

static OperationResult safeMalloc_OOM_OPERATION(ErrorFatality isFailureFatal) {
        void* ptr = NULL;

        /* Set a smaller limit (8 MB) so this test doesn't actually eat all the
         * RAM */
        struct rlimit lim;
        lim.rlim_cur = SIZE_FAKE_OOM;
        lim.rlim_max = SIZE_FAKE_OOM;
        if (setrlimit(RLIMIT_AS, &lim) != 0) return OP_FAILURE;

        /* Attempt to over-allocate. This will crash on fatal failure */
        ptr = safeMalloc(2 * SIZE_FAKE_OOM, 1, isFailureFatal);

        /* In nonfatal mode, the operation should fail by reporting an error
         * and returning NULL */
        if (isFailureFatal == ERR_NONFATAL) {
                if (!ptr) return OP_FAILURE;
        }

        /* If we get this far, the operation succeeded */
        safeFree(ptr);
        return OP_SUCCESS;
}

/* ----- TOO LARGE ALLOCATION ----- */
static OperationResult safeMalloc_sizeTooLarge_OPERATION(
        ErrorFatality isFailureFatal
);

static TestResult test_safeMalloc_sizeTooLarge_NONFATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_sizeTooLarge_NONFATAL()",
                safeMalloc_sizeTooLarge_OPERATION,
                EXPECT_OP_FAILURE, ERR_NONFATAL
        );
}

static TestResult test_safeMalloc_sizeTooLarge_FATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_sizeTooLarge_FATAL()",
                safeMalloc_sizeTooLarge_OPERATION,
                EXPECT_OP_FAILURE, ERR_FATAL
        );
}

static OperationResult safeMalloc_sizeTooLarge_OPERATION(
                ErrorFatality isFailureFatal) {
        /* Attempt to over-allocate. This will crash on fatal failure */
        void* ptr = safeMalloc(MEM_SIZE_MAX, MEM_SIZE_MAX, isFailureFatal);

        /* In nonfatal mode, the operation should fail by reporting an error
         * and returning NULL */
        if (isFailureFatal == ERR_NONFATAL) {
                if(!ptr) return OP_FAILURE;
        }

        /* If we get this far, the operation failed to fail */
        safeFree(ptr);
        return OP_SUCCESS;
}

/* ----- ZERO ALLOCATION ----- */
static OperationResult safeMalloc_zeroAlloc_OPERATION(
        ErrorFatality isFailureFatal
);

static TestResult test_safeMalloc_zeroAlloc_NONFATAL(void) {
        return (safeMalloc_zeroAlloc_OPERATION(ERR_NONFATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static TestResult test_safeMalloc_zeroAlloc_FATAL(void) {
        return (safeMalloc_zeroAlloc_OPERATION(ERR_FATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static OperationResult safeMalloc_zeroAlloc_OPERATION(
                ErrorFatality isFailureFatal) {
        char* ptr = NULL;

        /* ct == 0 */
        ptr = safeMalloc(SIZE_ZERO, SIZE_NORMAL, isFailureFatal);
        if (ptr) {
                safeFree(ptr);
                return OP_FAILURE;
        }

        /* sz == 0 */
        ptr = safeMalloc(SIZE_NORMAL, SIZE_ZERO, isFailureFatal);
        if (ptr) {
                safeFree(ptr);
                return OP_FAILURE;
        }

        /* If we get this far, both operations returned NULL, as expected */
        return OP_SUCCESS;
}

/* ----- NORMAL ALLOCATION ----- */
static OperationResult safeMalloc_normalAlloc_OPERATION(
        ErrorFatality isFailureFatal
);

static TestResult test_safeMalloc_normalAlloc_NONFATAL(void) {
        return (safeMalloc_normalAlloc_OPERATION(ERR_NONFATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static TestResult test_safeMalloc_normalAlloc_FATAL(void) {
        return (safeMalloc_normalAlloc_OPERATION(ERR_FATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static OperationResult safeMalloc_normalAlloc_OPERATION(
                ErrorFatality isFailureFatal) {
        char* ptr = safeMalloc(1, SIZE_NORMAL, isFailureFatal);
        /* Allocate one element of reasonable size */
        if (ptr) {
                safeFree(ptr);
                return OP_SUCCESS;
        }

        /* If we get this far, allocation failed */
        return OP_FAILURE;
}

/* ----- VERY LARGE NORMAL ALLOCATION ----- */
static OperationResult safeMalloc_largeAlloc_OPERATION(
        ErrorFatality isFailureFatal
);

static TestResult test_safeMalloc_largeAlloc_NONFATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_largeAlloc_NONFATAL()",
                safeMalloc_largeAlloc_OPERATION,
                EXPECT_OP_SUCCESS, ERR_NONFATAL
        );
}

static TestResult test_safeMalloc_largeAlloc_FATAL(void) {
        return runTestInChildProcess(
                "test_safeMalloc_largeAlloc_FATAL()",
                safeMalloc_largeAlloc_OPERATION,
                EXPECT_OP_SUCCESS, ERR_FATAL
        );
}

static OperationResult safeMalloc_largeAlloc_OPERATION(
                ErrorFatality isFailureFatal) {
        char* ptr = NULL;
        struct rlimit lim;

        /* Portion of the available memory */
        int part;
        #ifdef RUNNING_ON_VALGRIND
                part = 4;
        #else
                part = 2;
        #endif

        /* Set a smaller limit (8 MB) so this test doesn't stress the system */
        lim.rlim_cur = SIZE_FAKE_OOM;
        lim.rlim_max = SIZE_FAKE_OOM;
        if (setrlimit(RLIMIT_AS, &lim) != 0) return OP_FAILURE;

        /* ALlocate one element of relatively very large but valid size */
        ptr = safeMalloc(1, SIZE_FAKE_OOM/part, isFailureFatal);
        if (!ptr) {
                return OP_FAILURE;
        }

        /* If we get this far, allocation returned non-NULL--success */
        safeFree(ptr);
        return OP_SUCCESS;
}

/* ----- MEMORY IS USABLE ----- */
static OperationResult safeMalloc_memUsable_OPERATION(
        ErrorFatality isFailureFatal
);

static TestResult test_safeMalloc_memUsable_NONFATAL(void) {
        return (safeMalloc_memUsable_OPERATION(ERR_NONFATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static TestResult test_safeMalloc_memUsable_FATAL(void) {
        return (safeMalloc_memUsable_OPERATION(ERR_FATAL) == OP_SUCCESS) ?
                TEST_PASS : TEST_FAIL;
}

static OperationResult safeMalloc_memUsable_OPERATION(
                ErrorFatality isFailureFatal) {
        int i;
        char* ptr = safeMalloc(SIZE_NORMAL, sizeof(*ptr), isFailureFatal);

        /* Fill every byte with 0xAA */
        memset(ptr, '~', SIZE_NORMAL);

        /* Check that data is readable */
        for (i = 0; i < SIZE_NORMAL; i++) {
                if (ptr[i] != '~') { 
                        safeFree(ptr);
                        return OP_FAILURE;
                }
        }

        /* All elements successfully written and read back */
        safeFree(ptr);
        return OP_SUCCESS;
}