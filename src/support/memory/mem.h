#ifndef MEM_H
#define MEM_H

/* --- MEMORY ALLOCATION WRAPPERS --- */

#include "handleErrors.h"

#include <stddef.h>

#ifndef MEM_SIZE_MAX
/**
 * size_t is unsigned, so casting -1 to a size_t yields max value for size_t
 */
#define MEM_SIZE_MAX ((size_t)-1)
#endif

/**
 * Allocates an unitialized memory block of size (ct*sz) with overflow
 * checking. Returns NULL if (ct==0 || sz==0) and on nonfatal Out-Of-Memory
 * (OOM). On fatal OOM, reports and exits.
 * 
 * NOTE: for zero-initialized memory, use safeCalloc 
 * */
void* safeMalloc(size_t ct, size_t sz, ErrorFatality isFailureFatal);

/**
 * Allocates ct elements of size sz, zero-initialized, with overflow checking. 
 * Returns NULL if (ct==0 || sz==0) and on nonfatal OOM. On fatal OOM, reports 
 * and exits. 
 * */
void* safeCalloc(size_t ct, size_t sz, ErrorFatality isFailureFatal);

/**
 * Resizes the allocation pointed to by ptr from (oldCt*elemSz) to
 * (newCt*elemSz). Relies on realloc() to preserve existing data up to (min
 * (oldCt,newCt)*elemSz). If (newCt==0 || elemSz==0), frees ptr and returns 
 * NULL. On nonfatal OOM, returns NULL and leaves ptr untouched. On fatal OOM, 
 * reports and exits.
 * 
 * NOTE: Newly added bytes are uninitialized.
 */
void* safeRealloc(
        void* ptr, size_t newCt, size_t elemSz, ErrorFatality isFailureFatal);

/**
 * May use later
 */
/*
void* safeRealloc_zeroGrow(
        void* ptr, size_t oldCt, size_t newCt,
        size_t elemSz, ErrorFatality isFailureFatal
);
*/

/* Frees ptr if non-NULL. It is safe to pass in NULL. */
void safeFree(void* ptr);

#endif