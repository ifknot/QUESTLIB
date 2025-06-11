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
#include "quest_errors.h"
#include "quest_types.h"
#include "quest_rtti.h"
#include "quest_info.h"

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
    quest_rtti_t rtti;            ///< Runtime type information
    quest_location_t* locations[2]; ///< Connected locations [0]=origin, [1]=destination
    quest_info_t* info;           ///< Connection description (ownership transferred)
} quest_connector_t;

/**
 * @brief Initializes a connector between two locations
 * @param conn Pre-allocated connector to initialize
 * @param loc1 First connected location (must not be NULL)
 * @param loc2 Second connected location (must not be NULL)
 * @param type Connector type from quest_types.h
 * @param info Descriptive information (ownership transferred)
 *
 * @code
 * // Example: Manual initialization
 * quest_connector_t passage;
 * quest_info_t* info = quest_info_create(arena, "Stone Passage", "Carved from living rock");
 * quest_connector_init(&passage, dungeon, crypt, QUEST_CONNECTOR_PASSAGE, info);
 * @endcode
 */
void quest_connector_init(
    quest_connector_t* conn,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Creates a new connector
 * @param arena Memory arena for allocation (must not be NULL)
 * @param loc1 First connected location (must not be NULL)
 * @param loc2 Second connected location (must not be NULL)
 * @param type Connector type from quest_types.h
 * @param info Descriptive information (ownership transferred)
 * @return New connector or NULL on allocation failure
 *
 * @code
 * // Example 1: Creating a basic passage
 * quest_connector_t* tunnel = quest_connector_create(arena, cave1, cave2,
 *     QUEST_CONNECTOR_PASSAGE,
 *     quest_info_create(arena, "Tunnel", "Damp and narrow"));
 *
 * // Example 2: Creating a stairwell
 * quest_connector_t* stairs = quest_connector_create(arena, foyer, balcony,
 *     QUEST_CONNECTOR_STAIRS,
 *     quest_info_create(arena, "Marble Stairs", "Worn smooth by centuries of use"));
 * @endcode
 */
quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
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
    quest_connection_bitmask_t direction
);

#endif
