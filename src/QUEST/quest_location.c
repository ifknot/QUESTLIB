#include "quest_location.h"
#include "quest_composite.h"
#include <assert.h>

void quest_location_init(
    quest_location_t* loc,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
) {
    quest_composite_init(&loc->base, parent, type, info);
    assert(symbol >= 32 && symbol <= 126 && "Invalid ASCII symbol"); // Printable chars only
    loc->symbol = symbol;
}

quest_location_t* quest_location_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
) {
    assert(arena && "NULL memory arena!");
    quest_location_t* loc = mem_arena_calloc(arena, sizeof(quest_location_t));
    quest_location_init(loc, parent, type, info, symbol);
    return loc;
}
