/**
 * @file tdd_progress.h
 * @brief Test progress reporting utilities
 * @defgroup test_utils Test Utilities
 * @{
 */
#ifndef TDD_PROGRESS_H
#define TDD_PROGRESS_H

#include <stdio.h>
#include <time.h>

/* ----------------- Progress Tracking ----------------- */

typedef struct {
    size_t total;
    size_t current;
    size_t width;       // bar width - 0 if not a width widget
    size_t step;        // step size for percent or rotation - if 0 step is calculated
} tdd_progress_t;


tdd_progress_t tdd_progress_start(size_t total, size_t current, size_t step, size_t width);

void tdd_progress_bar(tdd_progress_t* p);

void tdd_progress_percent(tdd_progress_t* p);

void tdd_progress_end(tdd_progress_t* p);

#endif

/** @} */ // end of test_utils group
