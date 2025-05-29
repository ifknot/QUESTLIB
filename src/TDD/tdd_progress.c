#include "tdd_progress.h"

#include <stdio.h>
#include <assert.h>

tdd_progress_t tdd_progress_make(uint32_t total, uint32_t current, uint32_t step, uint32_t width) {
    tdd_progress_t progress;
    progress.total = total;
    progress.current = current;
    progress.step = (step) ? total / step : total / width;
    progress.width = width;
    return progress;
}

void tdd_progress_bar(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if((progress->current++) % progress->step > 0 && progress->current < progress->total) {
        return;    // current mod step != 0
    }
    printf("\r[");
    int limit = (progress->current * progress->width) / progress->total;
    for (int i = 0; i < progress->width; i++) {
        putchar(i < limit ? '=' : ' ');
    }
    printf("] %3i%%", (int)((progress->current * 100) / progress->total));
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if((progress->current++) % progress->step > 0 && progress->current < progress->total) {
        return;    // current mod step != 0
    }
    printf("\r %3i%%", (int)((progress->current * 100) / progress->total));
    fflush(stdout);
}
