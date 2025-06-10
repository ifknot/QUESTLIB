#include "quest_door.h"
#include <assert.h>

void quest_door_init(
    quest_door_t* door,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
) { 
    quest_connector_init(&door->base, loc1, loc2, type, info);
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
    quest_door_init(door, loc1, loc2, type, info, locked, key);
    return door;
}

quest_error_t quest_door_lock(quest_door_t* door) {
    assert(door != NULL);

    if (door->is_locked) {
        return QUEST_ALREADY_LOCKED;
    }

    door->is_locked = true;
    return QUEST_SUCCESS;
}

quest_error_t quest_door_unlock(quest_door_t* door, const quest_rtti_t key) {
    assert(door != NULL);

    if (!door->is_locked) {
        return QUEST_ALREADY_UNLOCKED;
    }

    if (door->key.fingerprint, key->fingerprint){
        return QUEST_WRONG_KEY;
    }

    door->is_locked = false;
    return QUEST_SUCCESS;
}
