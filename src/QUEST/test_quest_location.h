#ifndef TEST_QUEST_LOCATION_H
#define TEST_QUEST_LOCATION_H

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"

#include "quest_constants.h"
#include "quest_errors.h"
#include "quest_types.h"
#include "quest_location.h"
#include "quest_connector.h"
#include "quest_door.h"

#define QUEST_LOCATION_TESTS &test_location_creation,  \
    &test_location_connections, \
    &test_invalid_connection,   \
    &test_multi_floor_connections

static mem_arena_t* arena = NULL;

/**
 * @brief Test fixture setup - runs before each test
 * @param initial_size Arena size in bytes (default 1MB if 0)
 */
void setup_test_arena() {
    arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_2K);
    assert(arena);
}

/**
 * @brief Test fixture teardown - runs after each test
 */
void teardown_test_arena() {
    if (arena) {
        mem_arena_delete(arena);
        arena = NULL;
    }
}

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_LOCATION,
    QUEST_CONNECTOR_PASSAGE,
    QUEST_CONNECTOR_STAIRS
} quest_object_t;

TEST(test_location_creation) {
    setup_test_arena();
    quest_location_t* loc = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'R');
    EXPECT(loc != NULL);
    EXPECT_EQ(loc->symbol, 'R');
    EXPECT_EQ(loc->floor_number, 0);
    EXPECT_EQ(loc->connection_directions, 0);
    teardown_test_arena();
}

TEST(test_location_connections) {
    setup_test_arena();
    quest_location_t* loc1 = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'A');
    quest_location_t* loc2 = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'B');
    quest_connector_t* conn = quest_connector_create(arena, loc1, loc2, QUEST_CONNECTOR_PASSAGE, NULL);

    EXPECT_EQ(quest_connector_join(loc1, loc2, CONN_E, conn), QUEST_SUCCESS);
    EXPECT(loc1->connections[2] == conn); // CONN_E is index 2
    EXPECT(loc2->connections[6] == conn); // CONN_W is index 6
    EXPECT(loc1->connection_directions & FLAG_E);
    EXPECT(loc2->connection_directions & FLAG_W);
    teardown_test_arena();
}

TEST(test_invalid_connection) {
    setup_test_arena();
    quest_location_t* loc = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'X');
    // instead of returning an error these now fail at the assert
    //EXPECT_EQ(quest_connector_join(loc, NULL, CONN_N, NULL), QUEST_INVALID_ARGS); //
    //EXPECT_EQ(quest_connector_join(NULL, loc, CONN_N, NULL), QUEST_INVALID_ARGS); //
    //EXPECT_EQ(quest_connector_join(loc, loc, 0xFFFF, NULL), QUEST_INVALID_ARGS); //
    teardown_test_arena();
}

TEST(test_multi_floor_connections) {
    setup_test_arena();
    quest_location_t* ground = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'G');
    quest_location_t* upper = quest_location_create(arena, NULL, QUEST_LOCATION, NULL, 'U');
    quest_connector_t* stairs = quest_connector_create(arena, ground, upper, QUEST_CONNECTOR_STAIRS, NULL);

    EXPECT_EQ(quest_connector_join(ground, upper, CONN_UP, stairs), QUEST_SUCCESS);
    EXPECT(ground->connections[8] == stairs); // CONN_UP is index 8
    EXPECT(upper->connections[9] == stairs); // CONN_DOWN is index 9
    EXPECT(ground->connection_directions & CONN_UP);
    EXPECT(upper->connection_directions & CONN_DOWN);
    teardown_test_arena();
}

#endif
