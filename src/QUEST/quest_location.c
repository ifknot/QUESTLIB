#include "quest_location.h"

#include <assert.h>

#include "quest_composite.h"
#include "quest_connector.h"

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

/**
 * @brief Dumps location information to a stream
 * @param loc Location to dump (must not be NULL)
 * @param stream Output stream (must not be NULL)
 *
 * @output_format
 * [LOCATION_TYPE] "Name" |LLLLLLLL:HHHHHHHH|
 *   Details...
 *   RTTI: [type:NAME serial:0xFFFF time:0xFFFF]
 *   Parent: [TYPE] "Parent Name" or NULL
 *   Position: (X,Y,Z)=(#, #, #)
 *   Symbol: 'X'
 *   Active Directions: [N/NE/E/etc.]
 *   Connections:
 *     [DIRECTION] ->
 *       [CONNECTOR_TYPE] "Name" |LLLLLLLL:HHHHHHHH|
 *         ... (full connector dump)
 */
void quest_location_dump(const quest_location_t* loc, FILE* stream) {
    assert(loc && "NULL location");
    assert(stream && "NULL stream");

    // First dump the base composite information
    quest_composite_dump(&loc->base, stream);

    // Add location-specific information
    fprintf(stream, "  Position: (X,Y,Z)=(%zu, %zu, %zu)\n",
            loc->grid_x, loc->grid_y, loc->floor_number);
    fprintf(stream, "  Symbol: '%c'\n", loc->symbol);

    // Show active directions
    fprintf(stream, "  Active Directions: [");
    bool first = true;
    for (int i = 0; i < sizeof(direction_to_flag)/sizeof(direction_to_flag[0]); i++) {
        if (loc->active_directions & direction_to_flag[i]) {
            if (!first) fprintf(stream, "/");
            fprintf(stream, "%s", quest_direction_to_string(direction_to_flag[i]));
            first = false;
        }
    }
    fprintf(stream, "]\n");

    // List connections with full connector dumps
    fprintf(stream, "  Connections:\n");
    for (int i = 0; i < MAX_FLOORS; i++) {
        if (loc->connections[i]) {
            fprintf(stream, "    [%s] ->\n", quest_direction_to_string(direction_to_flag[i]));
            quest_connector_dump(loc->connections[i], stream);
            fprintf(stream, "\n");
        }
    }
}
