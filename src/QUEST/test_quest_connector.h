/**
 * @file test_quest_connector.h
 * @brief Test harness for location connectors
 *
 * Tests connection systems including:
 * - Basic passageways
 * - Multi-floor structures
 * - Locked doors
 * - Circular layouts
 *
 * @dot
 * digraph overview {
 *   rankdir=LR;
 *   node [shape=box, style=rounded];
 *   
 *   A -> B [label="Passage"];
 *   B -> C [label="Door"];
 *   C -> D [label="Stairs", dir=both];
 * }
 * @enddot
 */
#ifndef TEST_QUEST_CONNECTOR_H
#define TEST_QUEST_CONNECTOR_H

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"
#include "quest_connector.h"
#include "quest_location.h"
#include "quest_door.h"

#define QUEST_CONNECTOR_TESTS &test_circular_building,      \
                             &test_multi_floor_tower,      \
                             &test_mixed_connection_types, \
                             &test_connector_edge_cases

// =============================================
// Test Configuration
// =============================================
static mem_arena_t* test_arena = NULL;
static quest_rtti_t test_key = { .parts = {QUEST_ITEM_KEY, 42} };

// =============================================
// Test Utilities
// =============================================
static void setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_2K);
    ASSERT(test_arena != NULL, "Arena creation failed");
    V(printf("Test setup: Arena initialized\n"));
}

static void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
    }
    V(printf("Test teardown: Arena cleaned\n\n"));
}

static quest_location_t* create_test_location(char symbol) {
    quest_info_t* info = quest_info_create(test_arena, "Test Loc", "Test location");
    quest_location_t* loc = quest_location_create(test_arena, NULL, QUEST_LOCATION, info, symbol);
    ASSERT(loc != NULL, "Location creation failed");
    return loc;
}

// =============================================
// Test Cases
// =============================================

/**
 * @brief Tests circular building layout
 * @dot
 * digraph {
 *   rankdir=LR;
 *   A -> B -> C -> D -> A;
 * }
 * @enddot
 */
TEST(test_circular_building) {
    setup();
    
    /* Creates 4 rooms connected N->E->S->W->N */
    quest_location_t* rooms[4];
    for (int i = 0; i < 4; i++) {
        rooms[i] = create_test_location('A' + i);
    }

    // Create and connect passages
    quest_connector_t* connections[4];
    const quest_connection_direction_t dirs[4] = {CONN_N, CONN_E, CONN_S, CONN_W};
    
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        connections[i] = quest_connector_create(test_arena, rooms[i], rooms[next], 
                                              QUEST_CONNECTOR_PASSAGE, NULL);
        quest_connector_join(connections[i], rooms[i], rooms[next], dirs[i]);
    }

    // Verify connections
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        EXPECT_EQ(rooms[i]->connections[dirs[i]], connections[i]);
        EXPECT_EQ(rooms[next]->connections[opposite_directions[dirs[i]]], connections[i]);
    }
    
    V(printf("Created circular building layout\n"));
    teardown();
}

/**
 * @brief Tests multi-floor tower structure
 * @dot
 * digraph {
 *   rankdir=BT;
 *   G -> F1 -> T1;
 *   G -> {G2 G3 G4};
 * }
 * @enddot
 */
TEST(test_multi_floor_tower) {
    setup();
    
    /* 3-floor tower with:
     * - Ground: 4 rooms
     * - First: 2 rooms  
     * - Top: 1 room */
    quest_location_t* floors[3][4] = {0};
    
    // Create floors
    for (int floor = 0; floor < 3; floor++) {
        int rooms = (floor == 0) ? 4 : (floor == 1) ? 2 : 1;
        for (int i = 0; i < rooms; i++) {
            floors[floor][i] = create_test_location('0' + floor);
        }
    }

    // Connect stairwell
    quest_connector_t* stairs[2];
    stairs[0] = quest_connector_create(test_arena, floors[0][0], floors[1][0], 
                                     QUEST_CONNECTOR_STAIRS, NULL);
    stairs[1] = quest_connector_create(test_arena, floors[1][0], floors[2][0],
                                     QUEST_CONNECTOR_STAIRS, NULL);
    
    quest_connector_join(stairs[0], floors[0][0], floors[1][0], CONN_UP);
    quest_connector_join(stairs[1], floors[1][0], floors[2][0], CONN_UP);

    // Verify vertical connections
    EXPECT_EQ(floors[0][0]->connections[CONN_UP], stairs[0]);
    EXPECT_EQ(floors[1][0]->connections[CONN_DOWN], stairs[0]);
    EXPECT_EQ(floors[1][0]->connections[CONN_UP], stairs[1]);
    EXPECT_EQ(floors[2][0]->connections[CONN_DOWN], stairs[1]);
    
    V(printf("Created multi-floor tower\n"));
    teardown();
}

/**
 * @brief Tests mixed connection types
 * @dot
 * digraph {
 *   M -> N [label="Door"];
 *   M -> E [label="Passage"]; 
 *   M -> U [label="Stairs"];
 * }
 * @enddot
 */
TEST(test_mixed_connection_types) {
    setup();
    
    quest_location_t* main = create_test_location('M');
    quest_location_t* north = create_test_location('N');
    quest_location_t* east = create_test_location('E');
    quest_location_t* upper = create_test_location('U');

    // Create different connectors
    quest_door_t* door = quest_door_create(test_arena, main, north, 
                                         QUEST_DOOR_WOODEN, NULL, true, test_key);
    quest_connector_t* passage = quest_connector_create(test_arena, main, east,
                                                      QUEST_CONNECTOR_PASSAGE, NULL);
    quest_connector_t* stairs = quest_connector_create(test_arena, main, upper,
                                                     QUEST_CONNECTOR_STAIRS, NULL);

    // Connect them
    quest_connector_join((quest_connector_t*)door, main, north, CONN_N);
    quest_connector_join(passage, main, east, CONN_E);
    quest_connector_join(stairs, main, upper, CONN_UP);

    // Verify connections
    EXPECT_EQ(main->connections[CONN_N], (quest_connector_t*)door);
    EXPECT_EQ(main->connections[CONN_E], passage);
    EXPECT_EQ(main->connections[CONN_UP], stairs);
    EXPECT(((quest_door_t*)main->connections[CONN_N])->is_locked);
    
    V(printf("Created mixed connection types\n"));
    teardown();
}

/**
 * @brief Tests edge cases and error conditions
 */
TEST(test_connector_edge_cases) {
    setup();
    
    quest_location_t* loc = create_test_location('X');
    
    // Should trigger assertions in debug mode
    #ifndef NDEBUG
    printf("Expecting assertion failures for edge cases:\n");
    quest_connector_t dummy;
    EXPECT_EQ(quest_connector_join(NULL, loc, loc, CONN_N), QUEST_INVALID_ARGS);
    EXPECT_EQ(quest_connector_join(&dummy, NULL, loc, CONN_S), QUEST_INVALID_ARGS);
    EXPECT_EQ(quest_connector_join(&dummy, loc, NULL, CONN_E), QUEST_INVALID_ARGS);
    EXPECT_EQ(quest_connector_join(&dummy, loc, loc, 0), QUEST_INVALID_ARGS);
    #endif
    
    teardown();
}

// =============================================
// Test Runner
// =============================================
RUN_TESTS(QUEST_CONNECTOR_TESTS)

#endif 
