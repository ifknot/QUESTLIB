#include "tdd_progress.h"

tdd_progress_t tdd_progress_start(size_t total) {
    return (tdd_progress_t){
        .total_iterations = total,
        .current_iteration = 0,
        .start_time = time(NULL)
    };
}

void tdd_progress_bar(tdd_progress_t* p, int width) {
    if (p->current_iteration++ % (p->total_iterations / (width ?: 50)) != 0) 
        return;
    printf("\r[");
    int progress = (p->current_iteration * (width ?: 50)) / p->total_iterations;
    for (int i = 0; i < (width ?: 50); i++)
        putchar(i < progress ? '=' : ' ');
    printf("] %3d%%", (int)((p->current_iteration * 100) / p->total_iterations));
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* p, int steps) {
    if (p->current_iteration++ % (p->total_iterations / (steps ?: 20)) != 0)
        return;
    printf("\r%3d%% complete", (int)((p->current_iteration * 100) / p->total_iterations));
    fflush(stdout);
}

void tdd_progress_end(tdd_progress_t* p) {
    time_t duration = time(NULL) - p->start_time;
    printf("\nCompleted %zu iterations in %ld seconds\n", 
           p->total_iterations, duration);
}
