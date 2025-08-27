#include "handleErrors.h"
#include "textStylesAndColors.h"
#include <stdio.h>
#include <string.h>

static const char* err_getErrName(int err);
static const char* err_getErrDescription(int err);

int err_reportError(
                ErrorFatality isErrFatal, int err,
                const char* file, int line, const char* func) {
        // Build the error splash
        fprintf(stderr, FG_RED "[");
        if (isErrFatal == ERR_FATAL) fprintf(stderr, BOLD "FATAL_");
        fprintf(stderr, "%s" UNBOLD "]" RESET ": ", err_getErrName(err));

        // Print diagnostic information
        fprintf(stderr, "FILE: %s; LINE: %d; IN %s, %s\n", 
                file, line, func, err_getErrDescription(err));
        
        // Exit on fatal error. Return errno for sys errors and 1 for custom
        int exitStatus = err < 256 ? err : 1;
        if (isErrFatal) inChild ? _exit(exitStatus) : exit(exitStatus);

        return 0;
}

static const char* err_getErrName(int err) {
        #define X(name, val) case name: return (#name + ERRNO_PREFIX_SLICE);
        switch (err) {
                SYS_ERRORS
                CUSTOM_ERRORS
                default: return "ERR_UNKNOWN";
        }
        #undef X
}

static const char* err_getErrDescription(int err) {
        if (err == ERRNO_NONE) return "Not an error";
        if (err < 1000) return strerror(err); // System errors

        // Custom errors
        static const char* tbl[] = {
                [ERRNO_SYNTAX - 1000] = "There is a problem with your input",
                [ERRNO_OVERFLOW - 1000] = "Request too large"
        };

        // Don't fall off the table and avoid NULL slots
        int tblLen = sizeof(tbl) / sizeof(tbl[0]);
        int index = err - 1000;
        if (index >= 0 && index < tblLen && tbl[index]) return tbl[index];
        return "Unknown error";
}