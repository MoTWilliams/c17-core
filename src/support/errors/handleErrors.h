#ifndef HANDLE_ERRORS_H
#define HANDLE_ERRORS_H

typedef enum {
        ERR_NONFATAL,
        ERR_FATAL
} ErrorFatality;

typedef enum {
        ERRNO_UNKNOWN,
        ERRNO_SYNTAX,
        ERRNO_IMPOSSIBLE_VALUE,
        ERRNO_OUT_OF_MEMORY
} ErrNo;

/**
 * Reports error type and location. 
 * 
 * parameters:
 * ErrorFatality isFatalErr - will this error crash the process? 
 *      options: 
 *              ERR_NONFATAL - no
 *              ERR_FATAL - yes
 * ErrNo errNo - the type of error 
 *      options:
 *              ERRNO_SYNTAX
 *              ERRNO_IMPOSSIBLE_VALUE
 *              ERRNO_OUT_OF_MEMORY
 * char* msg - A more detailed description of the error. Consider mentioning
 *      the current function in this message.
 * const char* file and int line are the current file and line, respectively. 
 * They are forwarded in automatically by the REPORT_ERR macro
 * 
 * returns: 
 * ErrNo errNo - the error number's enum identifier
 **/
ErrNo err_reportError(
        ErrorFatality isFatalErr, ErrNo errNo, 
        char* msg, const char* file, int line
);

#define REPORT_ERR(isFatalErr, errNo, msg) \
        err_reportError(isFatalErr, errNo, msg, __FILE__, __LINE__)

#endif