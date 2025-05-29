/**
 * @file tdd_progress.h
 * @brief Test progress reporting utilities
 * @defgroup test_utils Test Utilities
 * @{
 */
#ifndef TDD_PROGRESS_H
#define TDD_PROGRESS_H

#include <stdint.h>

typedef struct {
    uint32_t total;
    uint32_t current;
    uint32_t width;       // bar width - 0 if not a width widget
    uint32_t step;        // step size for percent or rotation - if 0 step is calculated
} tdd_progress_t;

tdd_progress_t tdd_progress_make(uint32_t total, uint32_t current, uint32_t step, uint32_t width);

void tdd_progress_bar(tdd_progress_t* p);

void tdd_progress_percent(tdd_progress_t* p);

#endif

/** @} */ // end of test_utils group
