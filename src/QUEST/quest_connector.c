#include "quest_connector.h"

#include <assert.h>

#include "quest_constants.h"
#include "quest_location.h"

quest_error_t quest_connector_init(
    quest_connector_t* conn,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
) {
    assert(conn && loc1 && loc2 && loc1 != loc2);
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
    assert(loc1 != NULL && loc2 != NULL);
    assert(connector != NULL);
    assert(direction != 0 && "Invalid direction bitmask");

    // Verify direction is single flag
    if ((direction & (direction - 1)) != 0) {
        return QUEST_INVALID_ARGS; // Multiple bits set
    }

    // Set up bidirectional connection
    switch (direction) {
        case CONN_N:  loc1->connections[0] = connector; break;
        case CONN_NE: loc1->connections[1] = connector; break;
        case CONN_E:  loc1->connections[2] = connector; break;
        case CONN_SE: loc1->connections[3] = connector; break;
        case CONN_S:  loc1->connections[4] = connector; break;
        case CONN_SW: loc1->connections[5] = connector; break;
        case CONN_W:  loc1->connections[6] = connector; break;
        case CONN_NW: loc1->connections[7] = connector; break;
        case CONN_UP: loc1->connections[8] = connector; break;
        case CONN_DOWN: loc1->connections[9] = connector; break;
        default: return QUEST_INVALID_ARGS;
    }

    // Set opposite direction for loc2
    quest_connection_bitmask_t opposite_dir = 0;
    switch (direction) {
        case CONN_N:  opposite_dir = CONN_S; break;
        case CONN_NE: opposite_dir = CONN_SW; break;
        case CONN_E:  opposite_dir = CONN_W; break;
        case CONN_SE: opposite_dir = CONN_NW; break;
        case CONN_S:  opposite_dir = CONN_N; break;
        case CONN_SW: opposite_dir = CONN_NE; break;
        case CONN_W:  opposite_dir = CONN_E; break;
        case CONN_NW: opposite_dir = CONN_SE; break;
        case CONN_UP: opposite_dir = CONN_DOWN; break;
        case CONN_DOWN: opposite_dir = CONN_UP; break;
    }

    loc1->connection_directions |= direction;
    loc2->connection_directions |= opposite_dir;

    return QUEST_SUCCESS;
}
