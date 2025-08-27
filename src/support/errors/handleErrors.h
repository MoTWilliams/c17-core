#ifndef HANDLE_ERRORS_H
#define HANDLE_ERRORS_H

#include <errno.h>
#include <stdbool.h>

extern bool inChild;

typedef enum {
        ERR_NONFATAL,
        ERR_FATAL
} ErrorFatality;

#define ERRNO_PREFIX_SLICE 6

#define SYS_ERRORS \
        X(ERRNO_EPERM, EPERM) \
        X(ERRNO_EAGAIN, EAGAIN) \
        X(ERRNO_ENOMEM, ENOMEM)

#define CUSTOM_ERRORS \
        X(ERRNO_NONE, 0) \
        X(ERRNO_SYNTAX, 1000) \
        X(ERRNO_OVERFLOW, 1001) \

#define X(name, val) name = val,
enum Errors {
        SYS_ERRORS
        CUSTOM_ERRORS
};
#undef X

int err_reportError(
        ErrorFatality isErrFatal, int err,
        const char* file, int line, const char* func);

#endif