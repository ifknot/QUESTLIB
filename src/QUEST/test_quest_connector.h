/**
* Important Notes
* Make sure Graphviz/dot is installed on your system and Doxygen is configured to use it:
* In Doxyfile, set HAVE_DOT = YES
* Set DOT_PATH if Graphviz isn't in system PATH
*/
#ifndef TEST_QUEST_CONNECTOR_H
#define TEST_QUEST_CONNECTOR_H

#include "tdd_macros.h"

#include "quest_constants
#include "quest_location.h"
#include "quest_connector.h"
#include "quest_door.h"
#include "quest_errors.h"
#include "../MEM/mem_arena.h"

#define QUEST_CONNECTOR_TESTS  &test_circular_building,      \
                               &test_multi_floor_tower,      \
                               &test_mixed_connection_types

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_CONNECTOR_PASSAGE,
    QUEST_CONNECTOR_STAIRS,
    QUEST_LOCATION
} quest_object_t;

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

/**
 * @brief Test Scenario: Circular Building Layout
 * @dot
 * digraph room_navigation {
 *   rankdir=LR;  // Left to right layout like your Mermaid example
 *   node [shape=box, style=rounded];
 *   
 *   A [label="N Room"];
 *   B [label="E Room"];
 *   C [label="S Room"];
 *   D [label="W Room"];
 *   
 *   A -> B [label="North"];
 *   B -> C [label="East"];
 *   C -> D [label="South"];
 *   D -> A [label="West"];
 * }
 * @enddot
*/
TEST(test_circular_building) {
    setup_test_arena();
    /* Creates a circular building with 4 rooms connected N->E->S->W->N */
    quest_location_t* rooms[4];
    for (int i = 0; i < 4; i++) {
        rooms[i] = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'A' + i);
    }

    // Connect in a circle
    quest_connector_t* conn_n = quest_connector_create(&arena, rooms[0], rooms[1], QUEST_CONNECTOR_PASSAGE, NULL);
    quest_connector_t* conn_e = quest_connector_create(&arena, rooms[1], rooms[2], QUEST_CONNECTOR_PASSAGE, NULL);
    quest_connector_t* conn_s = quest_connector_create(&arena, rooms[2], rooms[3], QUEST_CONNECTOR_PASSAGE, NULL);
    quest_connector_t* conn_w = quest_connector_create(&arena, rooms[3], rooms[0], QUEST_CONNECTOR_PASSAGE, NULL);

    EXPECT_EQ(quest_connector_join(rooms[0], rooms[1], CONN_N, conn_n), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(rooms[1], rooms[2], CONN_E, conn_e), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(rooms[2], rooms[3], CONN_S, conn_s), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(rooms[3], rooms[0], CONN_W, conn_w), QUEST_SUCCESS);

    // Verify full circle
    EXPECT(rooms[0]->connections[0] == conn_n); // N
    EXPECT(rooms[1]->connections[2] == conn_e); // E
    EXPECT(rooms[2]->connections[4] == conn_s); // S
    EXPECT(rooms[3]->connections[6] == conn_w); // W
    teardown_test_arena();
}

/**
 * @brief Test Scenario: Multi-Floor Tower
 * @dot
 * digraph G {
 *   graph [rankdir=TD];
 *   
 *   G1 [label="Ground 1"];
 *   G2 [label="G2"];
 *   G3 [label="G3"];
 *   G4 [label="G4"];
 *   G5 [label="G5"];
 *   F1 [label="Floor 1"];
 *   T1 [label="Top Floor"];
 *   
 *   G1 -> G2 [label="North"];
 *   G1 -> G3 [label="East"];
 *   G1 -> G4 [label="South"];
 *   G1 -> G5 [label="West"];
 *   G1 -> F1 [label="Up"];
 *   F1 -> T1 [label="Up"];
 * }
 * @enddot
 */
TEST(test_multi_floor_tower) {
    setup_test_arena();
    /* Creates a 3-floor tower with:
     * - Ground floor: 4 rooms
     * - First floor: 2 rooms
     * - Top floor: 1 room
     * All connected via central stairwell */
    quest_location_t* tower[3][4] = {0};
    
    // Create floors
    for (int floor = 0; floor < 3; floor++) {
        int rooms = (floor == 0) ? 4 : (floor == 1) ? 2 : 1;
        for (int i = 0; i < rooms; i++) {
            tower[floor][i] = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, '0' + floor);
        }
    }

    // Connect stairwell (center)
    quest_connector_t* stairs_up = quest_connector_create(&arena, tower[0][0], tower[1][0], QUEST_CONNECTOR_STAIRS, NULL);
    quest_connector_t* stairs_top = quest_connector_create(&arena, tower[1][0], tower[2][0], QUEST_CONNECTOR_STAIRS, NULL);

    EXPECT_EQ(quest_connector_join(tower[0][0], tower[1][0], CONN_UP, stairs_up), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(tower[1][0], tower[2][0], CONN_UP, stairs_top), QUEST_SUCCESS);

    // Connect ground floor rooms in a cross pattern
    quest_connector_t* ground_conn[4];
    for (int i = 0; i < 4; i++) {
        ground_conn[i] = quest_connector_create(&arena, tower[0][0], tower[0][i+1], QUEST_CONNECTOR_PASSAGE, NULL);
        EXPECT_EQ(quest_connector_join(tower[0][0], tower[0][i+1], 1 << (i*2), ground_conn[i]), QUEST_SUCCESS);
    }

    // Verify all connections
    EXPECT(tower[0][0]->connections[8] == stairs_up); // Up
    EXPECT(tower[1][0]->connections[9] == stairs_up); // Down
    EXPECT(tower[1][0]->connections[8] == stairs_top); // Up
    EXPECT(tower[2][0]->connections[9] == stairs_top); // Down
    teardown_test_arena();
}

/**
 * @brief Test Scenario: Mixed Connection Types
 * @dot
 * digraph G {
 *   graph [rankdir=TD];
 *   
 *   M [label="Main"];
 *   N [label="North"];
 *   E [label="East"]; 
 *   U [label="Up"];
 *   
 *   M -> N [label="Locked Door"];
 *   M -> E [label="Open Passage"];
 *   M -> U [label="Stairs Up"];
 * }
 * @enddot
 */
TEST(test_mixed_connection_types) {
    setup_test_arena();
    /* Creates a location with:
     * - Locked door to north
     * - Open passage to east
     * - Staircase up */
    quest_location_t* main_room = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'M');
    quest_location_t* north_room = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'N');
    quest_location_t* east_room = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'E');
    quest_location_t* upper_room = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'U');

    quest_rtti_t key = {.parts = {.type = QUEST_ITEM_KEY, .id = 1}};
    
    // Create different connection types
    quest_door_t* north_door = quest_door_create(&arena, main_room, north_room, QUEST_DOOR_WOODEN, NULL, true, key);
    quest_connector_t* east_passage = quest_connector_create(&arena, main_room, east_room, QUEST_CONNECTOR_PASSAGE, NULL);
    quest_connector_t* up_stairs = quest_connector_create(&arena, main_room, upper_room, QUEST_CONNECTOR_STAIRS, NULL);

    EXPECT_EQ(quest_connector_join(main_room, north_room, CONN_N, (quest_connector_t*)north_door), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(main_room, east_room, CONN_E, east_passage), QUEST_SUCCESS);
    EXPECT_EQ(quest_connector_join(main_room, upper_room, CONN_UP, up_stairs), QUEST_SUCCESS);

    // Verify connection states
    EXPECT(((quest_door_t*)main_room->connections[0])->is_locked); // North door locked
    EXPECT(main_room->connections[2] == east_passage); // East passage
    EXPECT(main_room->connections[8] == (quest_connector_t*)up_stairs); // Up stairs
    teardown_test_arena();
}

#endif
