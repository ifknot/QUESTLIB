#include "quest_connector.h"

#include <assert.h>

#include "quest_location.h"

void quest_connector_init(
    quest_connector_t* comp,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
) {
    quest_composite_init(&comp->base, parent, type, info);
}

quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
) {
    assert(arena && "NULL memory arena!");
    quest_connector_t* conn = mem_arena_calloc(arena, sizeof(quest_connector_t));
    quest_connector_init(conn, parent, type, info);
    return conn;
}

void quest_connector_join(
    quest_connector_t* connector,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_bitmask_t direction
) {
    assert(connector && "NULL connector!");
    assert(loc1 && "NULL 1st location!");
    assert(loc2 && "NULL 2nd location!");
    assert(direction >= CONN_N && direction <= CONN_STAIR && "Invalid direction enum");
    // Set up bidirectional connection
    loc1->connections[direction] = connector;
    loc2->connections[opposite_directions[direction]] = connector;
    // Set bit mask
    loc1->active_directions |= direction_to_flag[direction];
    loc2->active_directions |= direction_to_flag[opposite_directions[direction]];
}

const char* quest_direction_to_string(quest_bitmask_t dir) {
    static const char* names[] = {
        "N", "NE", "E", "SE", "S", "SW", "W", "NW",
        "UP", "DOWN", "/" // Up/Down/Stairs
    };

    for (int i = 0; i < sizeof(direction_to_flag)/sizeof(direction_to_flag[0]); i++) {
        if (dir == direction_to_flag[i]) {
            return names[i];
        }
    }
    return "?";
}

void quest_connector_dump(const quest_connector_t* conn, FILE* stream) {
    assert(conn && "NULL connector");
    assert(stream && "NULL stream");

    // First dump the base composite information
    quest_composite_dump(&conn->base, stream);

    // Then add connector-specific information
    fprintf(stream, "  Connections: ");
    if (conn->locations[0] && conn->locations[1]) {
        fprintf(stream, "[%p] \"%s\" <-> [%p] \"%s\"\n",
                (void*)conn->locations[0],
                quest_info_brief((quest_component_t*)conn->locations[0]),
                (void*)conn->locations[1],
                quest_info_brief((quest_component_t*)conn->locations[1]));
    } else {
        fprintf(stream, "UNCONNECTED\n");
    }

    // Show active directions (bitmask analysis)
    fprintf(stream, "  Directions: [");
    bool first = true;
    for (int i = 0; i < sizeof(direction_to_flag)/sizeof(direction_to_flag[0]); i++) {
        if (conn->base.base.features & direction_to_flag[i]) {
            if (!first) fprintf(stream, "/");
            fprintf(stream, "%s", quest_direction_to_string(direction_to_flag[i]));
            first = false;
        }
    }
    fprintf(stream, "]\n");
}
