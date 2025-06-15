#ifndef TEST_QUEST_RTTI_UID_H
#define TEST_QUEST_RTTI_UID_H

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_progress.h"

#include "quest_errors.h"
#include "quest_uid.h"
#include "quest_rtti.h"
#include "quest_objects.h"

#define QUEST_RTTI_TESTS &test_uid,                 \
                    &test_rtti_creation,            \
                    &test_type_checks,              \
                    &test_uid_uniqueness,           \
                    &test_fingerprint_components,   \
                    &test_null_type_handling

// =============================================
// UID and RTTI
// =============================================

TEST(test_uid) {
    const uint32_t ITERATIONS = 1000;
    tdd_progress_t prg = tdd_progress_make(ITERATIONS, 0, 0, 30); // 30-character wide bar starting at zero and calculating steps
    for (uint32_t i = 1; i <= ITERATIONS; i++) {
        tdd_progress_bar(&prg);
        ASSERT(i == quest_next_uid());
    }
}

TEST(test_rtti_creation) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_PLAYER);

    EXPECT_EQ(quest_rtti_type(rtti), QUEST_PLAYER);
    EXPECT_GT(quest_rtti_uid(rtti), 0);  // UIDs start at 1
}

TEST(test_type_checks) {
    quest_rtti_t loc = quest_rtti_create(QUEST_LOCATION);
    quest_rtti_t obj = quest_rtti_create(QUEST_PLAYER);

    EXPECT_EQ(quest_is_typeof(loc, QUEST_LOCATION), true);
    EXPECT_EQ(quest_is_typeof(obj, QUEST_PLAYER), true);
    EXPECT_EQ(quest_is_typeof(loc, QUEST_PLAYER), false);
}

TEST(test_uid_uniqueness) {
    quest_rtti_t rtti1 = quest_rtti_create(QUEST_NPC);
    quest_rtti_t rtti2 = quest_rtti_create(QUEST_NPC);

    EXPECT_EQ(quest_rtti_type(rtti1), quest_rtti_type(rtti2));
    EXPECT_NEQ(quest_rtti_uid(rtti1), quest_rtti_uid(rtti2));
    EXPECT_NEQ(quest_rtti_fingerprint(rtti1), quest_rtti_fingerprint(rtti2));
}

TEST(test_fingerprint_components) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_PLAYER);
    quest_fingerprint_t fp = quest_rtti_fingerprint(rtti);

    // Verify bit layout matches documentation
    EXPECT_EQ((fp >> 16) & 0xFFFF, quest_rtti_type(rtti));
    EXPECT_EQ(fp & 0xFFFF, quest_rtti_uid(rtti));
}

TEST(test_null_type_handling) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_NULL);

    EXPECT_EQ(quest_rtti_type(rtti), QUEST_NULL);
    EXPECT_GT(quest_rtti_uid(rtti), 0);  // UID still valid for NULL type
}

#endif
