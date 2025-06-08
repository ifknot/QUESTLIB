#include "quest_door.h"
#include <assert.h>

quest_error_t quest_door_init(
    quest_door_t* door,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
) {
    assert(door);
    assert(quest_connector_init(&door->base, loc1, loc2, type, info) == QUEST_SUCCESS);
    door->is_locked = locked;
    door->key = key;
    return QUEST_SUCCESS;
}

quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
) {
    assert(arena);
    quest_door_t* door = mem_arena_alloc(arena, sizeof(quest_door_t));
    assert(door);
    assert(quest_door_init(door, loc1, loc2, type, info, locked, key) == QUEST_SUCCESS);
    return door;
}
