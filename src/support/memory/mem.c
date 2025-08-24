#include "mem.h"

#include <stdlib.h>

void* safeMalloc(size_t ct, size_t sz, ErrorFatality isFailureFatal) {
        void* ptr = NULL;

        /* Zero allocation */
        if (ct == 0 || sz == 0) {
                return NULL;
        }

        /* Multiplication overflow */
        if (sz > (MEM_SIZE_MAX / ct)) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_IMPOSSIBLE_VALUE,
                        "IN safeMalloc(); Allocation size too large"
                );
                return NULL;
        }

        ptr = malloc(ct*sz);

        /* Allocation failure */
        if (!ptr) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_OUT_OF_MEMORY, 
                        "IN safeMalloc(); Allocation failure (OOM)"
                );
                return NULL;
        }

        return ptr;
}

void* safeCalloc(size_t ct, size_t sz, ErrorFatality isFailureFatal) {
        void* ptr = NULL;

        /* Zero allocation */
        if (ct == 0 || sz == 0) {
                return NULL;
        }

        /* Multiplication overflow */
        if (sz > (MEM_SIZE_MAX / ct)) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_IMPOSSIBLE_VALUE,
                        "IN safeCalloc(); Allocation size too large"
                );
                return NULL;
        }

        ptr = calloc(ct, sz);

        /* Allocation failure */
        if (!ptr) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_OUT_OF_MEMORY, 
                        "IN safeCalloc(); Allocation failure (OOM)"
                );
                return NULL;
        }

        return ptr;
}

void* safeRealloc(
        void* ptr, size_t newCt, size_t elemSz, ErrorFatality isFailureFatal) {
        /* Use a temporary pointer, because realloc() failure returns NULL and 
         * leaves the original pointer unchanged. */
        void* temp = NULL;

        /* Zero reallocation */
        if (newCt == 0 || elemSz == 0) {
                safeFree(ptr);
                return NULL;
        }

        /* Multiplication overflow */
        if (elemSz > (MEM_SIZE_MAX / newCt)) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_IMPOSSIBLE_VALUE, 
                        "IN safeRealloc(); Allocation size too large"
                );
                return NULL;
        }

        temp = realloc(ptr, (newCt * elemSz));

        /* Allocation failure */
        if (!temp) {
                REPORT_ERR(
                        isFailureFatal, ERRNO_OUT_OF_MEMORY, 
                        "IN safeRealloc(); Reallocation failure (OOM)"
                );
                return NULL;
        }

        /* pointer is now resized */
        return temp;
}

void safeFree(void* ptr) {
        if (!ptr) return;

        free(ptr);
}