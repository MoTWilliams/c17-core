#ifndef TEXT_STYLES_AND_COLORS_H
#define TEXT_STYLES_AND_COLORS_H

#if NO_COLORS   // Define no escape sequences 
        /* Turn off all styling */
        #define RESET ""

        /* Text styles */
        #define BOLD ""
        #define DIM ""
        #define ITALIC ""
        #define UNDER ""
        #define DBL_UNDER ""
        #define STRIKE ""
        #define BLINK ""
        #define SWAP ""

        /* Turn off text styles */
        #define UNBOLD ""
        #define UNDIM ""
        #define UNITALIC ""
        #define UNUNDER ""
        #define UNDBL_UNDER ""
        #define UNSTRIKE ""
        #define UNBLINK ""
        #define UNSWAP ""

        /* Foreground colors */
        #define NO_FGC ""
        #define FG_BLK ""
        #define FG_BLK_B ""
        #define FG_RED ""
        #define FG_RED_B ""
        #define FG_GRN ""
        #define FG_GRN_B ""
        #define FG_YEL ""
        #define FG_YEL_B ""
        #define FG_BLU ""
        #define FG_BLU_B ""
        #define FG_MAG ""
        #define FG_MAG_B ""
        #define FG_CYN ""
        #define FG_CYN_B ""
        #define FG_WHT ""
        #define FG_WHT_B ""

        /* Background colors */
        #define NO_BGC ""
        #define BG_BLK ""
        #define BG_BLK_B ""
        #define BG_RED ""
        #define BG_RED_B ""
        #define BG_GRN ""
        #define BG_GRN_B ""
        #define BG_YEL ""
        #define BG_YEL_B ""
        #define BG_BLU ""
        #define BG_BLU_B ""
        #define BG_MAG ""
        #define BG_MAG_B ""
        #define BG_CYN ""
        #define BG_CYN_B ""
        #define BG_WHT ""
        #define BG_WHT_B ""
#else
        /* Turn off all styling */
        #define RESET "\033[0m"

        /* Text styles */
        #define BOLD "\033[1m"
        #define DIM "\033[2m"
        #define ITALIC "\033[3m"
        #define UNDER "\033[4m"
        #define DBL_UNDER "\033[21m"
        #define STRIKE "\033[9m"
        #define BLINK "\033[5m"
        #define SWAP "\033[7m"

        /* Turn off text styles */
        #define UNBOLD "\033[22m"
        #define UNDIM "\033[22m"
        #define UNITALIC "\033[23m"
        #define UNUNDER "\033[24m"
        #define UNDBL_UNDER "\033[21m"
        #define UNSTRIKE "\033[29m"
        #define UNBLINK "\033[25m"
        #define UNSWAP "\033[27m"

        /* Foreground colors */
        #define NO_FGC "\033[39m" /* Reset to default foreground color */
        #define FG_BLK "\033[30m"
        #define FG_BLK_B "\033[90m"
        #define FG_RED "\033[31m"
        #define FG_RED_B "\033[91m"
        #define FG_GRN "\033[32m"
        #define FG_GRN_B "\033[92m"
        #define FG_YEL "\033[33m"
        #define FG_YEL_B "\033[93m"
        #define FG_BLU "\033[34m"
        #define FG_BLU_B "\033[94m"
        #define FG_MAG "\033[35m"
        #define FG_MAG_B "\033[95m"
        #define FG_CYN "\033[36m"
        #define FG_CYN_B "\033[96m"
        #define FG_WHT "\033[37m"
        #define FG_WHT_B "\033[97m"

        /* Background colors */
        #define NO_BGC "\033[49m" /* Reset to default background color */
        #define BG_BLK "\033[40m"
        #define BG_BLK_B "\033[100m"
        #define BG_RED "\033[41m"
        #define BG_RED_B "\033[101m"
        #define BG_GRN "\033[42m"
        #define BG_GRN_B "\033[102m"
        #define BG_YEL "\033[43m"
        #define BG_YEL_B "\033[103m"
        #define BG_BLU "\033[44m"
        #define BG_BLU_B "\033[104m"
        #define BG_MAG "\033[45m"
        #define BG_MAG_B "\033[105m"
        #define BG_CYN "\033[46m"
        #define BG_CYN_B "\033[106m"
        #define BG_WHT "\033[47m"
        #define BG_WHT_B "\033[107m"
#endif

#endif