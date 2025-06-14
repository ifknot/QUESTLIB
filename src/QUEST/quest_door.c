#include "quest_door.h"
#include <assert.h>

void quest_door_init(    // defaults to an open door with no lock
    quest_door_t* door,
     quest_type_t type,
    quest_info_t* info,
    quest_location_t* loc1,
    quest_location_t* loc2
) {}

quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
) {}

quest_error_t quest_door_open(quest_door_t* door) {
    return 0;
}

quest_error_t quest_door_close(quest_door_t* door) {
    return 0;
}
