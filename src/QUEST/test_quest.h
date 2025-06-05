#ifndef TEST_QUEST_H
#define TEST_QUEST_H

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_progress.h"

#include "quest_uid.h"

#define QUEST_TESTS &test_uid

TEST(test_uid) {
    const uint32_t ITERATIONS = 1000;
    tdd_progress_t prg = tdd_progress_make(ITERATIONS, 0, 0, 30); // 30-character wide bar starting at zero and calculating steps
    for (uint32_t i = 1; i <= ITERATIONS; i++) {
        tdd_progress_bar(&prg);
        ASSERT(i == quest_next_uid());
    }
}

#endif
