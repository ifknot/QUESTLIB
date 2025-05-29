#include "tdd_progress.h"

/* ----------------- Percent Progress ----------------- */

tdd_progress_t tdd_progress_start(size_t total, size_t current, size_t step, size_t width) {
    tdd_progress_t prg;
    prg.total = total;
    prg.current = current;
    prg.step = step;
    prg.width = width;
    return prg;
}

void tdd_progress_bar(tdd_progress_t* progress) {
    if (progress->current >= progress->total) {
        return;
    }
    printf("\r[");
    int n = (progress->current * progress->width) / progress->total;
    for (int i = 0; i < width; i++) {
        putchar(i < n ? '=' : ' ');
    }
    printf("]%3d%%", (int)((progress->current_iteration * 100) / progress->total_iterations))
    progress->current++;
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* progress) {
    if (progress->current >= progress->total) {
        return;
    }
    printf("\r%3d%% complete", (int)((progress->current_iteration * 100) / progress->total_iterations));
    fflush(stdout);
}

void tdd_progress_end(tdd_progress_t* p) {
    // future proofing and consistent interface
}
