#include "handleErrors.h"

#include "textStylesAndColors.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REGISTER_ERRNO(errNo) sprintf(splash + len, "%s", #errNo + 6)

static char* err_registerErrNo(ErrNo errNo);

ErrNo err_reportError(
                ErrorFatality isFatalErr, ErrNo errNo, 
                char* msg, const char* file, int line) {
        /* The error number as a string--max 1028 characters */
        char splash[1028 + 1] = {'\0'};

        /* Build the [ERROR_SPLASH] */
        size_t len = 0;
        sprintf(splash, "%s", isFatalErr ? "FATAL_" : "");
        len = strlen(splash);

        /* Add the error code from the enum to the [ERROR_SPLASH] */
        sprintf(splash + len, "%s", err_registerErrNo(errNo));

        /* Display the full error message */
        fprintf(stderr,(FG_RED "[" BOLD "%s" UNBOLD "] " RESET),splash);
        fprintf(stderr, "FILE: \"%s\"; LINE: %d; %s\n", file, line, msg);
        
        /* Exit immediately on fatal error */
        if (isFatalErr) exit(1);

        /* Otherwise, just return the error number */
        return errNo;
}

static char* err_registerErrNo(ErrNo errNo) {
        switch (errNo) {
                case ERRNO_SYNTAX: return "SYNTAX";
                case ERRNO_IMPOSSIBLE_VALUE: return "IMPOSSIBLE_VALUE";
                case ERRNO_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
                default: return "OTHER";
        }
}