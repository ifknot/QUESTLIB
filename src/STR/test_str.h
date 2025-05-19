#ifndef TEST_STR_H
#define TEST_STR_H

#include "../TDD/tdd_macros.h"

TEST(test_scrubbing) {
    EXPECT(1 == 0);
    ASSERT(1 == 1);
}

TEST(test_processing) {
    EXPECT(1 == 1);
    ASSERT(1 == 0);
}

#endif
