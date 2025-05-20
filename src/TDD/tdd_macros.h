/**
* Thank you Axel Lindeberg u/SuperSmurfen
* https://github.com/AxlLind/libwebb/blob/main/tests/libtest.h
* See usage here for example:
* https://github.com/AxlLind/libwebb/blob/main/tests/test_http.c
*/
#ifndef TDD_MACROS_H
#define TDD_MACROS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME (strrchr("/" __FILE__, '/') + 1)

#define INTERNAL_ASSERT(expr, assert)                                   \
    do {                                                                \
        if (!(expr)) {                                                  \
            printf("%s:%d - expected: %s\n", FILENAME, __LINE__, #expr);\
            (*pfailures)++;                                             \
            if (assert)                                                 \
                return;                                                 \
        }                                                               \
    } while (0)

#define EXPECT(expr) INTERNAL_ASSERT(expr, 0)
#define ASSERT(expr) INTERNAL_ASSERT(expr, 1)

typedef struct {
  void (*fn)(int *);
  char *name;
} test_t;

#define TEST(name)                              \
  static void name##_fn(int*);                  \
  static const test_t name = {name##_fn, #name};\
  static void name##_fn(int* pfailures)

#define RUN_TESTS(...)                                                      \
  int run_tests(void) {                                                     \
    const test_t* tests[] = {__VA_ARGS__};                                  \
    int failures = 0;                                                       \
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {         \
        tests[i]->fn(&failures);                                            \
        printf("%s: %s\n", failures ? "FAILED" : "PASSED", tests[i]->name);  \
    }                                                                       \
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;                      \
  }

#endif
