/**
 * @file quest_location.h
 * @brief Location management for game world
 *
 * Handles multi-floor environments with bitmask-based connections.
 * Uses composite pattern for child objects within locations.
 *
 * Features:
 * - Grid-based positioning (X/Y/Z)
 * - Directional connections (8 compass + up/down)
 * - Symbol-based map rendering
 * - Parent-child hierarchy for contained objects
 */
#ifndef QUEST_LOCATION_H
#define QUEST_LOCATION_H

#include <stdint.h>
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_composite.h"

#define MAX_FLOORS 10

typedef struct quest_connector_t quest_connector_t;

typedef struct quest_location_t quest_location_t;

/**
 * @brief Game location structure
 *
 * Represents a single location in the game world with:
 * - Composite base for child objects
 * - Grid positioning
 * - Multi-floor support
 * - Connection system
 */
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
 * @param loc Pre-allocated location to initialize
 * @param parent Parent component (NULL for root locations)
 * @param type Location type from quest_types.h
 * @param info Descriptive information (ownership transferred)
 * @param symbol Map display character
 *
 * @code
 * // Example: Initializing a dungeon cell
 * quest_location_t cell;
 * quest_info_t* info = quest_info_create(arena, "Dungeon Cell", "Dark and damp");
 * quest_location_init(&cell, NULL, QUEST_DUNGEON, info, '#');
 * @endcode
 */
void quest_location_init(
    quest_location_t* loc,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
);

/**
 * @brief Creates a new location
 * @param arena Memory arena for allocation
 * @param parent Parent component (NULL for root locations)
 * @param type Location type from quest_types.h
 * @param info Descriptive information (ownership transferred)
 * @param symbol Map display character
 * @return New location or NULL on failure
 *
 * @code
 * // Example 1: Creating a town square
 * quest_info_t* square_info = quest_info_create(arena, "Town Square", "Busy marketplace");
 * quest_location_t* square = quest_location_create(arena, NULL, QUEST_TOWN, square_info, 'T');
 *
 * // Example 2: Creating connected locations
 * quest_location_t* tavern = quest_location_create(arena, NULL, QUEST_BUILDING, ...);
 * quest_location_t* cellar = quest_location_create(arena, tavern, QUEST_DUNGEON, ...);
 * cellar->floor_number = -1; // Below ground
 * @endcode
 */
quest_location_t* quest_location_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    char symbol
);

#endif
