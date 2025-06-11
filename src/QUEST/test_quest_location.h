#ifndef TEST_QUEST_LOCATION_H
#define TEST_QUEST_LOCATION_H

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"
#include "quest_composite.h"
#include "quest_info.h"
#include "quest_location.h"
#include "quest_connector.h"
#include "quest_door.h"
#include "quest_object_types.h"
#include "quest_types.h"

#define QUEST_LOCATION_TESTS &test_location_creation,  \
                            &test_location_connections, \
                            //&test_invalid_connection,   \
                            &test_multi_floor_connections

// =============================================
// Test Configuration
// =============================================
static mem_arena_t* test_arena = NULL;

// =============================================
// Test Utilities
// =============================================
static void setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_2K);
    V(printf("Test setup: Arena initialized\n"););
}

static void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
    }
    V(printf("Test teardown: Arena cleaned\n\n"););
}

static quest_info_t* create_test_info(const char* brief) {
    return quest_info_copy(test_arena, brief, "Test description");
}

// =============================================
// Test Cases
// =============================================
TEST(test_location_creation) {
    setup();
    quest_info_t* info = create_test_info("Ruined Temple");
    quest_location_t* loc = quest_location_create(test_arena, root, QUEST_LOCATION, info, 'R');

    EXPECT(loc != NULL);
    EXPECT_EQ(loc->symbol, 'R');
    EXPECT_EQ(loc->floor_number, 0);
    EXPECT_EQ(loc->connection_directions, 0);

    // Verify composite base
    EXPECT_EQ(loc->base.base.rtti.parts.type, QUEST_LOCATION);
    EXPECT(loc->base.base.info != NULL);
    EXPECT_STREQ(loc->base.base.info->brief, "Ruined Temple");

    teardown();
}

TEST(test_location_connections) {
    setup();

    quest_location_t* loc1 = quest_location_create(test_arena, NULL, QUEST_LOCATION, create_test_info("Hall"), 'A');
    quest_location_t* loc2 = quest_location_create(test_arena, NULL, QUEST_LOCATION, create_test_info("Chamber"), 'B');
    quest_connector_t* conn = quest_connector_create(test_arena, loc1, loc2, QUEST_CONNECTOR_PASSAGE, NULL);

    // Test east-west connection
    quest_connector_join(conn, loc1, loc2, CONN_E);
    EXPECT_EQ(loc1->connections[CONN_E], conn);
    EXPECT_EQ(loc2->connections[CONN_W], conn);
    EXPECT(loc1->connection_directions & FLAG_E);
    EXPECT(loc2->connection_directions & FLAG_W);

    V(printf("Created passage between Hall (E) and Chamber (W)\n"););

    teardown();
}

TEST(test_invalid_connection) {
    setup();

    quest_location_t* valid_loc = quest_location_create(test_arena, NULL, QUEST_LOCATION, create_test_info("Valid"), 'V');
    quest_connector_t* conn = quest_connector_create(test_arena, valid_loc, valid_loc, QUEST_CONNECTOR_PASSAGE, NULL);
    // Should trigger assertions in debug mode
    #ifndef NDEBUG
    printf("Expecting assertion failures for invalid connections:\n");
    quest_connector_join(NULL, valid_loc, NULL, CONN_N);
    quest_connector_join(conn, NULL, valid_loc, CONN_S);
    quest_connector_join(conn, valid_loc, valid_loc, 0xFFFF);
    #endif

    teardown();
}

TEST(test_multi_floor_connections) {
    setup();

    quest_location_t* ground = quest_location_create(test_arena, NULL, QUEST_LOCATION,
                                                   create_test_info("Cellar"), 'C');
    quest_location_t* upper = quest_location_create(test_arena, NULL, QUEST_LOCATION,
                                                  create_test_info("Attic"), 'A');
    ground->floor_number = 0;
    upper->floor_number = 1;

    quest_connector_t* stairs = quest_connector_create(test_arena, ground, upper,
                                                     QUEST_CONNECTOR_STAIRS,
                                                     create_test_info("Wooden Stairs"));

    // Test vertical connection
    quest_connector_join(stairs, ground, upper, CONN_UP);
    EXPECT_EQ(ground->connections[CONN_UP], stairs);
    EXPECT_EQ(upper->connections[CONN_DOWN], stairs);
    EXPECT(ground->connection_directions & FLAG_UP);
    EXPECT(upper->connection_directions & FLAG_DOWN);

    V(printf("Connected Cellar (Up) to Attic (Down)\n"););

    teardown();
}

#endif
