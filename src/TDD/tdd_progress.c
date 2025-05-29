#include "tdd_progress.h"

#include <stdio.h>

typedef struct {
    size_t total;
    size_t current;
    size_t width;       // bar width - 0 if not a width widget
    size_t step;        // step size for percent or rotation - if 0 step is calculated
} private_tdd_progress_t;

tdd_progress_t tdd_progress_start(size_t total, size_t current, size_t step, size_t width) {
    tdd_progress_t progress;
    progress.total = total;
    progress.current = current;
    progress.step = (step) ? total / step : total / width;
    progress.width = width;
    return progress;
}

void tdd_progress_bar(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if(++progress->current % progress->step) {
        return;    // current mod step != 0
    }
    printf("\r[");
    int limit = (progress->current * progress->width) / progress->total;
    for (int i = 0; i < width; i++) {
        putchar(i < limit ? '=' : ' ');
    }
    printf("] %3d%%", (int)((progress->current_iteration * 100) / progress->total_iterations))
    progress->current++;
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if(++progress->current % progress->step) {
        return;    // current mod step != 0
    }
    printf("\r%3d%% complete", (int)((progress->current_iteration * 100) / progress->total_iterations));
    fflush(stdout);
}
