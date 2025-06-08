/**
 * @file quest_location.h
 * @brief Location management for game world
 *
 * Handles multi-floor environments with bitmask-based connections
 * Uses composite pattern for child objects within locations
 */
#ifndef QUEST_LOCATION_H
#define QUEST_LOCATION_H

#include <stdint.h>
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_composite.h"
#include "quest_connector.h"

// Connection direction bitflags
#define CONN_N   0x0001
#define CONN_NE  0x0002
#define CONN_E   0x0004
#define CONN_SE  0x0008
#define CONN_S   0x0010
#define CONN_SW  0x0020
#define CONN_W   0x0040
#define CONN_NW  0x0080
#define CONN_UP  0x0100
#define CONN_DOWN 0x0200
#define CONN_STAIR 0x0400  // Special staircase connection

#define MAX_FLOORS 10

typedef uint16_t quest_connection_bitmask_t;

typedef struct quest_location_t {
    quest_composite_t base;           ///< Base composite properties
    quest_connector_t* connections[10]; ///< Connections array (8 dirs + up/down)
    quest_size_t grid_x, grid_y;      ///< Grid coordinates
    quest_size_t floor_number;        ///< Z-level (0=ground)
    char symbol;                     ///< Display character
    quest_connection_bitmask_t connection_directions; ///< Active connections
} quest_location_t;

/**
 * @brief Initializes a location structure
 * @param arena Memory arena for allocations
 * @param parent Parent component (or NULL)
 * @param type Location type (QUEST_LOCATION, etc.)
 * @param info Descriptive information
 * @param symbol Map display character
 * @return Error code
 */
quest_error_t quest_location_init(
    quest_location_t* loc,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
);

/**
 * @brief Creates a new location
 * @param arena Memory arena for allocation
 * @param parent Parent component (or NULL)
 * @param type Location type
 * @param info Descriptive information
 * @param symbol Map display character
 * @return New location or NULL on failure
 */
quest_location_t* quest_location_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
);

#endif
