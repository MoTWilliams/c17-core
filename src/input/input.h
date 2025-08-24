#ifndef INPUT_H
#define INPUT_H

/* Input capture mode */
typedef enum {
        CAPTURE_INTERACTIVE,    /* From stdin (eventually raw terminal) */
        CAPTURE_BATCH           /* From a file */
} InputMode;

#endif