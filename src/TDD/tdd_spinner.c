#include "tdd_spinner.h"

#include <stdio.h>

tdd_spinner_t tdd_spinner_start(const char* frames) {
    return (tdd_spinner_t){
        .counter = 0,
        .spinner_frames = frames ? frames : "|/-\\"
    };
}

void tdd_spinner_step(tdd_spinner_t* s, int delay_ms) {
    printf("\r%c", s->spinner_frames[s->counter++ % strlen(s->spinner_frames)]);
    fflush(stdout);
    if (delay_ms > 0) usleep(delay_ms * 1000);
}

void tdd_spinner_end(tdd_spinner_t* s) {
    printf("\r \r"); // Clear spinner
    fflush(stdout);
}

#endif
/** @} */ // end of test_utils group
