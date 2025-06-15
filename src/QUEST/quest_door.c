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
    return door->base.base.base.features & DOOR_IS_OPEN;
}

quest_error_t quest_door_open(quest_door_t* door) {
    return QUEST_SUCCESS;
}

quest_error_t quest_door_close(quest_door_t* door) {
    return 0;
}

void quest_door_dump(const quest_door_t* door, FILE* stream) {
    assert(door && "NULL door");
    assert(stream && "NULL stream");

    // Door header with features
    fprintf(stream, "\n[DOOR] \"%s\" |",
            quest_info_brief((quest_component_t*)door));

    // Feature bits (same format as other dumps)
    for (int i = 7; i >= 0; i--) {
        fprintf(stream, "%d", (door->base.base.base.features >> i) & 1);
    }
    fprintf(stream, ":");
    for (int i = 15; i >= 8; i--) {
        fprintf(stream, "%d", (door->base.base.base.features >> i) & 1);
    }
    fprintf(stream, "|\n");

    // Door-specific properties
    fprintf(stream, "  State:   %s\n", quest_door_is_open(door) ? "OPEN" : "CLOSED");
    fprintf(stream, "  Weight:  %zu kg\n", door->weight);

    // Material with enum safety check
    const char* material = "Unknown";
    switch(door->strength) {
        case DOOR_PAPER: material = "Paper"; break;
        case DOOR_WOOD: material = "Wood"; break;
        case DOOR_IRON: material = "Iron"; break;
        case DOOR_BRONZE: material = "Bronze"; break;
    }
    fprintf(stream, "  Material: %s\n", material);

    // Lock system info
    fprintf(stream, "  Lockable: %s\n",
            (door->base.base.base.features & DOOR_IS_LOCKABLE) ? "Yes" : "No");

    // Standard component info (indented)
    fprintf(stream, "  ");
    quest_component_dump((quest_component_t*)door, stream);

    // Children (locks, mechanisms etc.) if any exist
    if (door->base.base.child_count > 0) {
        fprintf(stream, "  Attachments:\n");
        for (quest_size_t i = 0; i < door->base.base.child_count; i++) {
            fprintf(stream, "    [%02zu] ", i);
            quest_component_dump(door->base.base.children[i], stream);
        }
    }
}
