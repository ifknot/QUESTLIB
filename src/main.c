#include <stdio.h>

//#include "DEMOS/demo_strings.h"
//#include "STR/test_str.h"
#include "TDD/tdd_macros.h"

TEST(a) {
    EXPECT(1 == 0);
    ASSERT(1 == 1);
}

TEST(b) {
    EXPECT(1 == 1);
    ASSERT(1 == 0);
}

int run_tests(void) {
  const test_t* tests[] = {&a, &b};
  int failures = 0;
  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    tests[i]->fn(&failures);
    printf("%s: %s\n", failures ? "FAILED" : "PASSED", tests[i]->name);
  }
  return failures ? 1 : 0;
}

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

    return 0;

}
