/**
 * @file tdd_macros.h
 * @brief Minimalist Test-Driven Development (TDD) framework
 * @author (modified version of) Axel Lindeberg aka u/SuperSmurfen
 * @see Original implementation: https://github.com/AxlLind/libwebb/blob/main/tests/libtest.h
 * @see Example usage: https://github.com/AxlLind/libwebb/blob/main/tests/test_http.c
 *
 * @defgroup tdd_framework Test Framework
 * @{
 */

#ifndef TDD_MACROS_H
#define TDD_MACROS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tdd_progress.h"

/**
 * @brief Conditional verbose output macro
 * @param expr Expression to execute only in debug mode
 */
#ifndef NDEBUG
#define V(expr) do { { expr } } while(0)
#else
#define V(expr)
#endif

// =============================================
// Internal Implementations
// =============================================

/**
 * @brief Extracts filename from full path
 */
#define _FILENAME (strrchr("/" __FILE__, '/') + 1)

#define _EXPECT_COMPARE(a, b, op, tag)                                  \
    do {                                                                \
        if (!((a) op (b))) {                                            \
            printf("\n%s:%d - FAILED: %s %s %s\n"                       \
                   "  Actual:   %d\n"                                   \
                   "  Expected: %d\n",                                  \
                   _FILENAME, __LINE__, #a, #op, #b, (int)(a), (int)(b));\
            *pass = false;                                              \
        }                                                               \
    } while (0)

#define _EXPECT_STRCOMPARE(a, b, op, tag)                              \
    do {                                                                \
        int cmp = strcmp((a), (b));                                     \
        if (!(cmp op 0)) {                                              \
            printf("\n%s:%d - FAILED: %s %s %s\n"                       \
                   "  Actual:   \"%s\"\n"                               \
                   "  Expected: \"%s\"\n"                               \
                   "  strcmp() = %d\n",                                \
                   _FILENAME, __LINE__, #a, #op, #b, (a), (b), cmp);    \
            *pass = false;                                              \
        }                                                               \
    } while (0)

/**
 * @brief Internal assertion handler
 * @param expr Expression to evaluate
 * @param halt Whether to stop test execution on failure
 * @private
 */
#define _ASSERT(expr, halt)                                         \
    do {                                                                \
        if (!(expr)) {                                                  \
            printf("\n%s:%d - expected: %s\n", _FILENAME, __LINE__, #expr);\
            *pass = false;                                              \
            if (halt)                                                   \
                return;                                                 \
        }                                                               \
    } while (0)

/**
 * @brief Non-fatal test expectation
 * @param expr Expression to test (continues execution if false)
 */
#define EXPECT(expr) _ASSERT(expr, false)

/**
 * @brief Fatal test assertion
 * @param expr Expression to test (stops test if false)
 */
#define ASSERT(expr, ...) _ASSERT(expr, true)

// =============================================
// Numeric Comparisons
// =============================================

/**
 * @brief Expects two values to be equal (non-fatal)
 * @param a Actual value
 * @param b Expected value
 */
#define EXPECT_EQ(a, b) _EXPECT_COMPARE(a, b, ==, "EQ")

/**
 * @brief Expects two values to be unequal (non-fatal)
 * @param a First value
 * @param b Second value
 */
#define EXPECT_NEQ(a, b) _EXPECT_COMPARE(a, b, !=, "NEQ")

/**
 * @brief Expects a > b (non-fatal)
 * @param a Tested value
 * @param b Comparison threshold
 */
#define EXPECT_GT(a, b) _EXPECT_COMPARE(a, b, >, "GT")

/**
 * @brief Expects a < b (non-fatal)
 * @param a Tested value
 * @param b Comparison threshold
 */
#define EXPECT_LT(a, b) _EXPECT_COMPARE(a, b, <, "LT")

// =============================================
// String Comparisons
// =============================================

/**
 * @brief Expects two strings to be equal (strcmp == 0)
 * @param a Actual string
 * @param b Expected string
 */
#define EXPECT_STREQ(a, b) _EXPECT_STRCOMPARE(a, b, == 0, "STREQ")

/**
 * @brief Expects two strings to be unequal (strcmp != 0)
 * @param a First string
 * @param b Second string
 */
#define EXPECT_STRNEQ(a, b) _EXPECT_STRCOMPARE(a, b, != 0, "STRNEQ")



/**
 * @brief Test case structure
 */
typedef struct {
    void (*fn)(bool*);   /**< Test function pointer */
    char *name;          /**< Test name */
} test_t;

/**
 * @brief Declares a test case
 * @param name Test case name
 *
 * @details Creates:
 *          1. Test function prototype
 *          2. Test case structure
 *          3. Test function implementation
 */
#define TEST(name)                                  \
    static void name##_fn(bool*);                   \
    static const test_t name = {name##_fn, #name};  \
    static void name##_fn(bool* pass)

/**
 * @brief Executes a test suite
 * @param ... Variable list of test cases
 * @return Number of failed tests
 *
 * @details Handles:
 *          - Test execution
 *          - Result reporting
 *          - Failure counting
 *          - Verbosity control
 */
#ifndef NDEBUG
#define RUN_TESTS(...)                                                          \
    int run_tests(void) {                                                       \
        const test_t* tests[] = {__VA_ARGS__};                                  \
        int failures = 0;                                                       \
        for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {         \
            bool passed = true;                                                 \
            tests[i]->fn(&passed);                                              \
            printf("\n%s: %s\n", passed ? "PASS" : "FAIL", tests[i]->name); \
            if (!passed)                                                        \
                failures++;                                                     \
        }                                                                       \
        V(printf("Failures = %i", failures););                                  \
        return failures;                                                        \
    }
#else
#define RUN_TESTS(...)                                                          \
    int run_tests(void) {                                                       \
        const test_t* tests[] = {__VA_ARGS__};                                  \
        int iterations = sizeof(tests) / sizeof(tests[0]);                      \
        int failures = 0;                                                       \
        for (size_t i = 0; i < iterations; i++) {                               \
            bool passed = true;                                                 \
            tests[i]->fn(&passed);                                              \
            printf("%c",'.');                                                   \
            fflush(stdout);                                                     \
            if (!passed)                                                        \
                failures++;                                                     \
        }                                                                       \
        return failures;                                                        \
    }
#endif
#endif

/** @} */ // end of tdd_framework group
