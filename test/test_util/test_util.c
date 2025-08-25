#include "test_util.h"

#include <stdio.h>
#include <unistd.h>     /* For pid_t and fork() */
#include <sys/wait.h>   /* For waitpid() */
#include <errno.h>


TestResult runTestInChildProcess(
                char* testName, 
                OperationResult(*operation)(const ErrorFatality), 
                OperationExpect expect, ErrorFatality isFailureFatal) {
        pid_t pid;
        OperationResult opResult;
        int status = -99;
        int rc;
        
        /* Flush all buffers before forking, just in case */
        fflush(stdout);
        fflush(stderr);
        pid = fork();

        /* Handle fork error */
        if ((int)pid < 0) {
                /* Build the fork error message */
                char errMsg[512 + 1];
                sprintf(errMsg, "In %s; fork failed", testName);

                /* Report the fork error and prematurely fail the test */
                REPORT_ERR(ERR_NONFATAL, ERRNO_FORK, errMsg);
                return TEST_FAIL;
        } else if (pid == 0) {
                /* Run the test operation and store the result */
                opResult = operation(isFailureFatal);

                /* If operation fails when it is expected to succeed, or if it 
                 * succeeds when it is expected to fail, exit the process in
                 * error. */
                if ((expect == EXPECT_OP_SUCCESS && opResult == OP_FAILURE) ||
                (expect == EXPECT_OP_FAILURE && opResult == OP_SUCCESS)) {
                        _exit(101);
                
                /* Otherwise, exit the process normally */
                } else {
                        _exit(0);
                }
        }

        /* Wait for the child process to finish. Handle interrupted system 
         * call */
        do {
                rc = waitpid(pid, &status, 0);
        } while (rc == -1 && errno == EINTR);

        if (rc == -1) {
                REPORT_ERR(ERR_NONFATAL, ERRNO_FORK, "waitpid failed");
                return TEST_FAIL;
        }

        /* If child crashes (killed by a signal), pass if failure expected */
        if (WIFSIGNALED(status)) {
                return (expect == EXPECT_OP_FAILURE) ? TEST_PASS : TEST_FAIL;
        }

        /* If child exits normally (returns), pass iff exit status == 0 */
        if (WIFEXITED(status)) {
                int code = WEXITSTATUS(status);

                /* Result matches expectation */
                if (code == 0) return TEST_PASS;

                /* Result does not match expectation */
                if (code == 101) return TEST_FAIL;

                /* Exit on fatal REPORT_ERR */
                return (expect == EXPECT_OP_FAILURE) ? TEST_PASS : TEST_FAIL;
        }

        /* If we get this far, something went wrong */
        REPORT_ERR(
                ERR_FATAL, ERRNO_UNKNOWN, 
                "IN runTestInChildProcess(); Something went wrong"
        );
        return TEST_FAIL;
}