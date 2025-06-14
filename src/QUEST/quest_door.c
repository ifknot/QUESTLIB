#include "quest_door.h"
#include "quest_composite.h"
#include "quest_connector.h"
#include "quest_errors.h"
//#include <assert.h>

void quest_door_init(    // defaults to an *unjoined*, open door with no lock and no hinges e.g an archway
    quest_door_t* door,
    quest_component_t* parent, // a suitable parent might be a game doorways parent to help with fast travel or visualization
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
) {
    quest_connector_init(&door->base, parent, type, info);
    door->weight = weight;
    door->strength = strength;
}

quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
) {
    assert(arena && "NULL memory arena!");
    quest_door_t* door = mem_arena_calloc(arena, sizeof(quest_door_t));
    quest_door_init(door, parent, type, info, weight, strength);
    return door;
}

bool quest_door_is_open(quest_door_t* door) {
    return door->features & DOOR_IS_OPEN;
}

quest_error_t quest_door_open(quest_door_t* door) {
    if(!quest_door_is_open(door)) {
        door->features |= DOOR_IS_OPEN;
        return QUEST_SUCCESS;
    }
    if(door->features & DOOR_HAS_LOCK) {
        return
    }
}

quest_error_t quest_door_close(quest_door_t* door) {
    return 0;
}
