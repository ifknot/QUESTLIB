#include "tdd_spinner.h"

#include <stdio.h>
#include <string.h>

typedef struct {
    size_t counter;
    const char* spinner_frames;
} private_tdd_spinner_t;

tdd_spinner_t tdd_spinner_start(const char* frames) {
    tdd_spinner_t spin;
    spin.counter = 0;
    spin.spinner_frames = (frames) ? frames : "|/-\\";
    return spin;
}

void tdd_spinner_step(tdd_spinner_t* s) {
    printf("\r%c", s->spinner_frames[s->counter++ % strlen(s->spinner_frames)]);
    fflush(stdout);
}

void tdd_spinner_end(tdd_spinner_t* s) {
    printf("\r \r"); // Clear spinner
    fflush(stdout);
}
/** @} */ // end of test_utils group
