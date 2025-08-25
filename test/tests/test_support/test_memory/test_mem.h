#ifndef TEST_MEM_H
#define TEST_MEM_H

#define SIZE_ZERO       0               /* For consistency and semantics */
#define SIZE_NORMAL     32              /* 32 bytes (tiny memory block) */
#define SIZE_LARGE      (1024 * 1024)   /* 1 MB */
#define SIZE_FAKE_OOM   8*1024*1024     /* 8 MB--not actually OOM big */

int test_mem(int* totTests, int* grpTests);

int test_safeMalloc(int* totTests, int* grpTests);
int test_safeCalloc(int* totTests, int* grpTests);
int test_safeRealloc(int* totTests, int* grpTests);
int test_safeFree(int* totTests, int* grpTests);

#endif