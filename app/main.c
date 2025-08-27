#include "handleErrors.h"
#include "textStylesAndColors.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
        /* Capture parent PID for error handling */
        err_init();
        
        /* Welcome message */
        printf(FG_BLU "c17 project starter kit" RESET "\n\n");

        printf("%d args: [", argc);
        for (int i = 0; argv[i]; i++) {
                printf("%s", argv[i]);
                if (argv[i+1]) printf(", ");
        }
        printf("]\n\n");

        #if DEBUG
                printf(FG_RED "DEBUG mode" RESET "\n\n");
        #else
                printf(FG_GRN "RELEASE mode" RESET "\n\n");
        #endif

        /* This only fires if everything else works */
        printf(FG_BLU "Goodbye!\n\n" RESET);

        return 0;
}
