#include "quest_connector.h"
#include <assert.h>

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
