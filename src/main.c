#include <stdio.h>

//#include "DEMOS/demo_strings.h"
//#include "STR/test_str.h"
#include "TDD/tdd_macros.h"

TEST(test_scrubbing) {
    EXPECT(1 == 0);
    ASSERT(1 == 1);
}

TEST(test_processing) {
    EXPECT(1 == 1);
    ASSERT(1 == 0);
}
/*
RUN_TESTS(
    test_scrubbing,
    test_processing
)
*/
int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

int f;

test_processing_fn(&f);

    return 0;

}
