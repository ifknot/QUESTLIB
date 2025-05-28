#include "tdd_progress.h"

/* ----------------- Percent Progress ----------------- */

tdd_progress_t tdd_progress_start(size_t total) {
    tdd_progress_t prog;
    prog.total_iterations = total;
    prog.current_iteration = 0;
    return prog;
}

void tdd_progress_bar(tdd_progress_t* p, size_t width) {
    if (p->current_iteration++ % (p->total_iterations / width) != 0) {
        return;
    }
    printf("\r[");
    int progress = (p->current_iteration * width) / p->total_iterations;
    for (int i = 0; i < width; i++) {
        putchar(i < progress ? '=' : ' ');
    }
    printf("] %3d%%", (int)((p->current_iteration * 100) / p->total_iterations));
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* p, size_t steps) {
    if (p->current_iteration++ % (p->total_iterations / steps) != 0) {
        return;
    }
    printf("\r%3d%% complete", (int)((p->current_iteration * 100) / p->total_iterations));
    fflush(stdout);
}

void tdd_progress_end(tdd_progress_t* p) {

}
