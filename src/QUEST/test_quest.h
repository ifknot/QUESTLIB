#ifndef TEST_QUEST_H
#define TEST_QUEST_H

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_progress.h"

#include "quest_uid.h"
#include "quest_rtti.h"

#define QUEST_TESTS &test_uid,                      \
                    &test_rtti_creation,            \
                    &test_type_checks,              \
                    &test_uid_uniqueness,           \
                    &test_fingerprint_components,    \
                    &test_null_type_handling

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_LOCATION,
    QUEST_CHARACTER,
    QUEST_PLAYER,
    QUEST_OBJECT
} quest_object_t;

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

    EXPECT_EQ(quest_get_type(rtti), QUEST_PLAYER);
    EXPECT_GT(quest_get_uid(rtti), 0);  // UIDs start at 1
}

TEST(test_type_checks) {
    quest_rtti_t loc = quest_rtti_create(QUEST_LOCATION);
    quest_rtti_t obj = quest_rtti_create(QUEST_OBJECT);

    EXPECT_EQ(quest_is_typeof(loc, QUEST_LOCATION), true);
    EXPECT_EQ(quest_is_typeof(obj, QUEST_OBJECT), true);
    EXPECT_EQ(quest_is_typeof(loc, QUEST_OBJECT), false);
}

TEST(test_uid_uniqueness) {
    quest_rtti_t rtti1 = quest_rtti_create(QUEST_CHARACTER);
    quest_rtti_t rtti2 = quest_rtti_create(QUEST_CHARACTER);

    EXPECT_EQ(quest_get_type(rtti1), quest_get_type(rtti2));
    EXPECT_NEQ(quest_get_uid(rtti1), quest_get_uid(rtti2));
    EXPECT_NEQ(quest_get_fingerprint(rtti1), quest_get_fingerprint(rtti2));
}

TEST(test_fingerprint_components) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_OBJECT);
    quest_fingerprint_t fp = quest_get_fingerprint(rtti);

    // Verify bit layout matches documentation
    EXPECT_EQ((fp >> 16) & 0xFFFF, quest_get_type(rtti));
    EXPECT_EQ(fp & 0xFFFF, quest_get_uid(rtti));
}

TEST(test_null_type_handling) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_NULL);

    EXPECT_EQ(quest_get_type(rtti), QUEST_NULL);
    EXPECT_GT(quest_get_uid(rtti), 0);  // UID still valid for NULL type
}

#endif
