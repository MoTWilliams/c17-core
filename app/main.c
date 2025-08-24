#include "input.h"

#include "handleErrors.h"
#include "textStylesAndColors.h"

#include <stdio.h>

void debug_testArgs(InputMode mode, char* arg);

int main(int argc, char* argv[]) {
        /* Default (no arguments) capture mode is interactive. */
        InputMode mode = CAPTURE_INTERACTIVE;

        /* If the second argument is present, set to batch mode */
        if (argv[1]) mode = CAPTURE_BATCH;

        /* Only up to two args allowed */
        if (argc > 2) {
                REPORT_ERR(
                        ERR_FATAL, ERRNO_SYNTAX, 
                        "IN main(); Too many arguments"
                );
        }

        /* Welcome message */
        printf(FG_BLU "\nWelcome to MASh!\n\n" RESET);

        #if DEBUG
                debug_testArgs(mode, argv[1]);
        #endif

        if (mode == CAPTURE_INTERACTIVE) do {} while (0);

        /* This only fires if everything else works */
        printf(FG_BLU "Goodbye!\n\n" RESET);

        return 0;
}

void debug_testArgs(InputMode mode, char* arg) {
        switch (mode) {
                case CAPTURE_INTERACTIVE:
                        printf("Interactive mode\n\n");
                        break;
                case CAPTURE_BATCH:
                        printf("Batch mode; arg = %s\n\n", arg);
                        break;
                default:
                        REPORT_ERR(
                                ERR_FATAL, ERRNO_UNKNOWN, 
                                "How'd you do that??");
                        break;
        }
}