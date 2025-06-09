#include "quest_connector.h"

#include <assert.h>

#include "quest_errors.h"
#include "quest_location.h"

// Lookup table for opposite directions (using quest_connection_direction_t)
static const quest_connection_direction_t opposite_directions[] = {
    CONN_S, CONN_SW, CONN_W, CONN_NW, CONN_N, CONN_NE, CONN_E, CONN_SE, CONN_DOWN, CONN_UP, CONN_STAIR
};

// Lookup table to convert direction enum -> bitflag
static const quest_connection_bitmask_t direction_to_flag[] = {
    FLAG_N, FLAG_NE, FLAG_E, FLAG_SE, FLAG_S, FLAG_SW, FLAG_W, FLAG_NW, FLAG_UP, FLAG_DOWN, FLAG_STAIR
};

quest_error_t quest_connector_init(
    quest_connector_t* conn,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
) {
    if(!(conn && loc1 && loc2 && loc1 != loc2)) {
        return QUEST_INVALID_ARGS;
    }
    conn->rtti = quest_rtti_create(type);
    conn->locations[0] = loc1;
    conn->locations[1] = loc2;
    conn->info = info;
    return QUEST_SUCCESS;
}

quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
) {
    assert(arena);
    quest_connector_t* conn = mem_arena_alloc(arena, sizeof(quest_connector_t));
    assert(conn);
    assert(quest_connector_init(conn, loc1, loc2, type, info) == QUEST_SUCCESS);
    return conn;
}

quest_error_t quest_connector_join(
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_connection_bitmask_t direction,
    quest_connector_t* connector
) {
    if(!(loc1 && loc2 && connector && direction)) {
        return QUEST_INVALID_ARGS;
    }
    // Set up bidirectional connection
    loc1->connections[direction] = connector;
    loc2->connections[opposite_directions[direction]] = connector;
    // Set bit mask
    loc1->connection_directions |= direction_to_flag[direction];
    loc2->connection_directions |= direction_to_flag[opposite_directions[direction]];
    return QUEST_SUCCESS;
}
