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
#include "quest_object_types.h"

#define QUEST_CONNECTOR_TESTS \
    &test_circular_building,      \
    &test_multi_floor_tower,      \
    &test_connector_lifecycle,    \
    &test_directional_ops,        \
    &test_connector_dump

// =============================================
// Test Configuration
// =============================================
static mem_arena_t* test_arena = NULL;
static quest_rtti_t test_key = { .parts = {QUEST_KEY, 42} };

// =============================================
// Test Utilities
// =============================================
static void setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_2K);
    assert(test_arena != NULL && "NULL test_arena");
    V(printf("Test setup: Arena initialized\n"););
}

static void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
    }
    V(printf("Test teardown: Arena cleaned\n\n"););
}

static quest_location_t* create_test_location(char symbol) {
    char name[32];
    snprintf(name, sizeof(name), "Room %c", symbol);
    quest_info_t* info = quest_info_copy(test_arena, name, "Test location");
    quest_location_t* loc = quest_location_create(test_arena, NULL, QUEST_LOCATION, info, '?');
    return loc;
}

// =============================================
// Test Cases (Original + Enhanced)
// =============================================

/**
 * @brief Tests circular building layout
 * @dot
 * digraph {
 *   rankdir=LR;
 *   A -> B -> C -> D -> A;
 *   label="Circular Building Layout";
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
        connections[i] = quest_connector_create(test_arena, NULL, QUEST_CONNECTOR_PASSAGE,
            quest_info_copy(test_arena, "Passage", "Stone corridor"));
        quest_connector_join(connections[i], rooms[i], rooms[next], dirs[i]);

        // New: Verify dump output
        /*
        FILE* tmp = tmpfile();
        quest_connector_dump(connections[i], tmp);
        rewind(tmp);
        char buf[128];
        fgets(buf, sizeof(buf), tmp);
        EXPECT_STR_CONTAINS(buf, "[CONNECTOR] \"Passage\"");
        fclose(tmp);
        */
    }

    // Verify connections
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        EXPECT_EQ(rooms[i]->connections[dirs[i]], connections[i]);
        EXPECT_EQ(rooms[next]->connections[opposite_directions[dirs[i]]], connections[i]);
    }

    teardown();
}

/**
 * @brief Tests multi-floor tower structure
 * @dot
 * digraph {
 *   rankdir=BT;
 *   G -> F1 -> T1;
 *   G -> {G2 G3 G4};
 *   label="Multi-Floor Tower";
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
    stairs[0] = quest_connector_create(test_arena, NULL, QUEST_CONNECTOR_STAIRS,
        quest_info_copy(test_arena, "Staircase", "Spiral stairs"));
    stairs[1] = quest_connector_create(test_arena, NULL, QUEST_CONNECTOR_STAIRS,
        quest_info_copy(test_arena, "Ladder", "Wooden rungs"));

    quest_connector_join(stairs[0], floors[0][0], floors[1][0], CONN_UP);
    quest_connector_join(stairs[1], floors[1][0], floors[2][0], CONN_UP);

    // New: Test direction helpers
    EXPECT_STREQ(quest_direction_to_string(CONN_UP), "UP");
    EXPECT_STREQ(quest_direction_to_string(CONN_DOWN), "DOWN");

    teardown();
}

// ... (keep original test_mixed_connection_types and test_connector_edge_cases) ...

/**
 * @brief Tests connector lifecycle management
 * @dot
 * digraph {
 *   rankdir=LR;
 *   A [shape=point];
 *   B [shape=point];
 *   A -> B [label="init→create→join→dump→destroy"];
 * }
 * @enddot
 */
TEST(test_connector_lifecycle) {
    setup();

    // Stack initialization
    quest_connector_t stack_conn;
    quest_connector_init(&stack_conn, NULL, QUEST_CONNECTOR_PASSAGE,
        quest_info_copy(test_arena, "Hallway", "Long corridor"));

    // Heap creation
    quest_connector_t* heap_conn = quest_connector_create(test_arena, NULL,
        QUEST_DOOR, quest_info_copy(test_arena, "Door", "Wooden door"));

    // Verify initialization
    EXPECT_EQ(stack_conn.base.base.rtti.parts.type, QUEST_CONNECTOR_PASSAGE);
    EXPECT_EQ(heap_conn->base.base.rtti.parts.type, QUEST_DOOR);

    teardown();
}

/**
 * @brief Tests directional operations
 */
TEST(test_directional_ops) {
    // Test opposite directions
    EXPECT_EQ(opposite_directions[CONN_N], CONN_S);
    EXPECT_EQ(opposite_directions[CONN_NE], CONN_SW);
    EXPECT_EQ(opposite_directions[CONN_UP], CONN_DOWN);

    // Test flag conversion
    EXPECT_EQ(direction_to_flag[CONN_N], FLAG_N);
    EXPECT_EQ(direction_to_flag[CONN_UP], FLAG_UP);
}

/**
 * @brief Tests connector dump output
 */
TEST(test_connector_dump) {
    setup();

    quest_location_t* kitchen = create_test_location('K');
    quest_location_t* pantry = create_test_location('P');

    quest_connector_t* door = quest_connector_create(test_arena, NULL, QUEST_DOOR,
        quest_info_copy(test_arena, "Kitchen Door", "Swinging door"));
    quest_connector_join(door, kitchen, pantry, CONN_W);

    // Capture dump output
    FILE* tmp = tmpfile();
    quest_connector_dump(door, tmp);

    // Verify content
    rewind(tmp);
    char buffer[256];
    fgets(buffer, sizeof(buffer), tmp);
    EXPECT_STR_CONTAINS(buffer, "[DOOR] \"Kitchen Door\"");
    fgets(buffer, sizeof(buffer), tmp);
    EXPECT_STR_CONTAINS(buffer, "Room K ←W→ Room P");

    fclose(tmp);
    teardown();
}

#endif
