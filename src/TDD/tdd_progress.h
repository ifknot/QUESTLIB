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
    size_t total_iterations;
    size_t current_iteration;
} tdd_progress_t;

/**
 * @brief Initializes progress tracking
 * @param total Total expected iterations
 * @return Initialized progress tracker
 */
tdd_progress_t tdd_progress_start(size_t total);

/**
 * @brief Updates and displays progress bar
 * @param p Progress tracker pointer
 * @param width Bar width in characters
 *
 * @example
 * tdd_progress_t p = tdd_progress_start(1000);
 * for (int i=0; i<1000; i++) {
 *     tdd_progress_bar(&p, 30);
 *     // Test logic...
 * }
 */
void tdd_progress_bar(tdd_progress_t* p, size_t width);

/**
 * @brief Updates and displays percentage progress
 * @param p Progress tracker pointer
 * @param steps Number of percentage steps (0 for default 20)
 */
void tdd_progress_percent(tdd_progress_t* p, size_t steps);

/**
 * @brief Finishes progress display
 * @param p Progress tracker pointer
 */
void tdd_progress_end(tdd_progress_t* p);

#endif

/** @} */ // end of test_utils group
