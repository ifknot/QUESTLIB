#include "quest_connector.h"

#include <assert.h>

#include "quest_location.h"

// Lookup table for opposite directions (using quest_connection_direction_t)
static const quest_connection_direction_t opposite_directions[] = {
    CONN_S, CONN_SW, CONN_W, CONN_NW, CONN_N, CONN_NE, CONN_E, CONN_SE, CONN_DOWN, CONN_UP, CONN_STAIR
};

// Lookup table to convert direction enum -> bitflag
static const quest_connection_bitmask_t direction_to_flag[] = {
    FLAG_N, FLAG_NE, FLAG_E, FLAG_SE, FLAG_S, FLAG_SW, FLAG_W, FLAG_NW, FLAG_UP, FLAG_DOWN, FLAG_STAIR
};

void quest_connector_init(
    quest_composite_t* comp,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
)) {
    quest_composite_init(&comp->base, parent, type, info);
}

quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
) {
    assert(arena && "NULL memory arena!");
    quest_connector_t* conn = mem_arena_calloc(arena, sizeof(quest_arena_t));
    quest_connector_init(conn, parent, type, info);
    return conn;
}

void quest_connector_join(
    quest_connector_t* connector,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_connection_bitmask_t direction
) {
    assert(connector && "NULL connector!");
    assert(loc1 && "NULL 1st location!");
    assert(loc2 && "NULL 2nd location!");
    assert(direction && "INVALIDdirection (0)!");
    assert((direction & (direction - 1)) == 0 && "MULTIPLE direction flags set!");
    // Set up bidirectional connection
    loc1->connections[direction] = connector;
    loc2->connections[opposite_directions[direction]] = connector;
    // Set bit mask
    loc1->connection_directions |= direction_to_flag[direction];
    loc2->connection_directions |= direction_to_flag[opposite_directions[direction]];
}
