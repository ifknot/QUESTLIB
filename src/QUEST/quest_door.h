/**
 * @file quest_door.h
 * @brief Door connector between locations with physical properties
 *
 * Specialized connector that simulates:
 * - Open/closed states
 * - Lockable mechanisms
 * - Material strength properties
 * - Weight-based movement requirements
 *
 * @dot
 * digraph door {
 *   node [shape=record];
 *   Door [label="quest_door_t|{<base>quest_connector_t|weight|strength|features}"];
 *   Connector [label="quest_connector_t|{locations[2]|direction}"];
 *   Door:base -> Connector [arrowhead=empty];
 * }
 * @enddot
 *
 * @code
 * // Example: Creating a locked iron door
 * quest_door_t* door = quest_door_create(arena, NULL, QUEST_DOOR,
 *     quest_info_create(arena, "Iron Gate", "Heavy security door"),
 *     150,  // Weight (kg)
 *     DOOR_IRON);
 * door->features |= DOOR_IS_LOCKABLE;
 * @endcode
 */
#ifndef QUEST_DOOR_H
#define QUEST_DOOR_H

#include <stdbool.h>

#include "quest_connector.h"
#include "quest_composite.h"
#include "quest_errors.h"
#include "quest_key.h"
#include "quest_types.h"

/**
 * @brief Door capability flags
 */
typedef enum {
    DOOR_IS_OPEN     = 0x0001, ///< Passage is traversable (e.g. open archway)
    DOOR_IS_LOCKABLE = 0x0002, ///< Can be secured with locks
} quest_door_features_t;

/**
 * @brief Door material types
 * @note Values represent relative strength for gameplay:
 * - Higher = More durable
 * - Affects smashing/breaking mechanics
 */
typedef enum {
    DOOR_PAPER  = 16,   ///< Temporary/breakable barriers
    DOOR_WOOD   = 32,   ///< Standard doors
    DOOR_IRON   = 64,   ///< Security doors
    DOOR_BRONZE = 128   ///< Ancient/reinforced doors
} quest_door_strengths_t;

/**
 * @brief Door physical properties and state
 */
typedef struct quest_door_t {
    quest_connector_t base;   ///< Base connector properties
    quest_size_t weight;      ///< Kilograms, affects opening difficulty
    quest_size_t strength;    ///< Material strength (quest_door_strengths_t)
} quest_door_t;

/**
 * @brief Initializes a door with default state
 * @param door Pre-allocated door structure
 * @param parent Parent component (e.g. doorway manager)
 * @param type Door type (QUEST_DOOR, QUEST_GATE, etc.)
 * @param info Descriptive metadata
 * @param weight Kilograms required to move
 * @param strength Material durability
 *
 * @note Default state:
 * - Unjoined to locations
 * - Features = DOOR_IS_OPEN
 * - Not lockable
 */
void quest_door_init(
    quest_door_t* door,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
);

//
/**
 * @brief Creates a new door in memory arena
 * @return New door or NULL on allocation failure
 * @see quest_door_init() for parameter details
 * @note doors are *not* joined to locations when created - use quest_connector_join(...)
 */
quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
);

/**
 * @brief Checks door open state
 * @return true if door permits passage
 */
bool quest_door_is_open(quest_door_t* door);

/**
 * @brief Attempts to open door
 * @return QUEST_SUCCESS or:
 *         QUEST_ERR_LOCKED if locked,
 *         QUEST_ERR_BLOCKED if obstructed
 */
quest_error_t quest_door_open(quest_door_t* door);

/**
 * @brief Attempts to close door
 * @return QUEST_SUCCESS or:
 *         QUEST_ERR_IMMOVABLE if too heavy,
 *         QUEST_ERR_BLOCKED if obstructed
 */
quest_error_t quest_door_close(quest_door_t* door);

/**
 * @brief Dumps door state to output stream
 * @param comp Door component (must be quest_door_t*)
 * @param stream Output destination
 */
void quest_door_dump(const quest_composite_t* comp, FILE* stream);

#endif
