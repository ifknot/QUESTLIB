#include "quest_location.h"
#include <assert.h>

void quest_location_init(
    quest_location_t* loc,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
) {
    assert(loc != NULL && "NULL location!");
    assert(parent && "NULL parent!");
    assert(info && "NULL string information!");

    loc->grid_x = 0;
    loc->grid_y = 0;
    loc->floor_number = 0;
    loc->symbol = symbol;
    loc->connection_directions = 0;

    for (int i = 0; i < 10; i++) {
        loc->connections[i] = NULL;
    }

    return QUEST_SUCCESS;
}

quest_location_t* quest_location_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
) {
    assert(arena != NULL);
    quest_location_t* loc = mem_arena_alloc(arena, sizeof(quest_location_t));
    assert(loc);
    quest_location_init(loc, parent, type, info, symbol);
    return loc;
}
