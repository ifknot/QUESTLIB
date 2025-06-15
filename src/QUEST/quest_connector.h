/**
 * @file quest_connector.h
 * @brief Connection between locations
 *
 * Base class for doors, passages, and other location connectors. Features:
 * - Bidirectional connection system
 * - Directional bitmask management
 * - RTTI support for derived types
 * - Arena-based memory management
 */
#ifndef QUEST_CONNECTOR_H
#define QUEST_CONNECTOR_H

#include "../MEM/mem_arena.h"
#include "quest_composite.h"

typedef enum { CONN_N = 0, CONN_NE, CONN_E, CONN_SE, CONN_S, CONN_SW, CONN_W, CONN_NW, CONN_UP, CONN_DOWN, CONN_STAIR
} quest_connection_direction_t;

// Connection direction bitflags
typedef enum {
    FLAG_N      = 0x0001,
    FLAG_NE     = 0x0002,
    FLAG_E      = 0x0004,
    FLAG_SE     = 0x0008,
    FLAG_S      = 0x0010,
    FLAG_SW     = 0x0020,
    FLAG_W      = 0x0040,
    FLAG_NW     = 0x0080,
    FLAG_UP     = 0x0100,
    FLAG_DOWN   = 0x0200,
    FLAG_STAIR  = 0x0400
} quest_connection_flags_t;

// Lookup table for opposite directions (using quest_connection_direction_t)
static const quest_connection_direction_t opposite_directions[] = {
    CONN_S, CONN_SW, CONN_W, CONN_NW, CONN_N, CONN_NE, CONN_E, CONN_SE, CONN_DOWN, CONN_UP, CONN_STAIR
};

// Lookup table to convert direction enum -> bitflag
static const quest_bitmask_t direction_to_flag[] = {
    FLAG_N, FLAG_NE, FLAG_E, FLAG_SE, FLAG_S, FLAG_SW, FLAG_W, FLAG_NW, FLAG_UP, FLAG_DOWN, FLAG_STAIR
};

typedef struct quest_location_t quest_location_t;

/**
 * @brief Base connector structure
 *
 * Connects two locations with:
 * - Type information for polymorphism
 * - Directional relationship tracking
 * - Descriptive text
 */
typedef struct quest_connector_t {
    quest_composite_t base;
    quest_location_t* locations[2]; ///< Connected locations [0]=origin, [1]=destination
} quest_connector_t;

/**
 * @brief Initializes a connector to be joined later
 * @param comp Pointer to pre-allocated component structure
 * @param parent Parent component or NULL for root objects
 * @param type Component type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 *
 * @code
 *
 * @endcode
 */
void quest_connector_init(
    quest_connector_t* comp,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Creates a new connector
 * @param arena Memory arena for allocation (must not be NULL)
 * @param parent Parent component or NULL for root objects
 * @param type Component type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 * @return New connector or NULL on allocation failure
 *
 * @code
 *
 * @endcode
 */
quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Establishes bidirectional connection between locations
 * @param connector Connector to use (must not be NULL)
 * @param loc1 Origin location (must not be NULL)
 * @param loc2 Destination location (must not be NULL)
 * @param direction Bitmask direction (CONN_N, CONN_NE, etc.)
 *
 * @code
 * // Example 1: Connecting rooms with a door
 * quest_door_t* door = quest_door_create(arena, hall, chamber, ...);
 * quest_connector_join((quest_connector_t*)door, hall, chamber, CONN_E);
 *
 * // Example 2: Creating vertical connection
 * quest_connector_join(stairs, ground_floor, second_floor, CONN_UP);
 *
 * // Example 3: Diagonal dungeon passage
 * quest_connector_join(passage, room1, room2, CONN_SE);
 * @endcode
 */
void quest_connector_join(
    quest_connector_t* connector,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_bitmask_t direction
);

/**
 * @brief Converts direction bitmask to human-readable string
 * @param dir Direction bitflag (single flag!)
 * @return Static string representation
 */
const char* quest_direction_to_string(quest_bitmask_t dir);

/**
 * @brief Dumps connector information to a stream
 * @param conn Connector to dump (must not be NULL)
 * @param stream Output stream (must not be NULL)
 *
 * @output_format
 * [CONNECTOR_TYPE] "Name" |LLLLLLLL:HHHHHHHH|
 *   Details...
 *   RTTI: [type:NAME serial:0xFFFF time:0xFFFF]
 *   Parent: [TYPE] "Parent Name" or NULL
 *   Connections: [0xADDR1] "Location1" <-> [0xADDR2] "Location2"
 *   Directions: [N/NE/E/etc.]
 */
void quest_connector_dump(const quest_connector_t* conn, FILE* stream);

#endif
