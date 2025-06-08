/**
 * @file quest_connector.h
 * @brief Connection between locations
 *
 * Base class for doors, passages, and other location connectors
 */
#ifndef QUEST_CONNECTOR_H
#define QUEST_CONNECTOR_H

#include "../MEM/mem_arena.h"
#include "quest_errors.h"
#include "quest_types.h"
#include "quest_rtti.h"
#include "quest_info.h"

typedef struct quest_location_t quest_location_t;

typedef struct quest_connector_t {
    quest_rtti_t rtti;            ///< Runtime type information
    quest_location_t* locations[2]; ///< Connected locations
    quest_info_t* info;           ///< Connection description
} quest_connector_t;

/**
 * @brief Initializes a connector
 * @param conn Connector to initialize
 * @param loc1 First connected location
 * @param loc2 Second connected location
 * @param type Connector type
 * @param info Descriptive information
 * @return Error code
 */
quest_error_t quest_connector_init(
    quest_connector_t* conn,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Creates a new connector
 * @param arena Memory arena for allocation
 * @param loc1 First connected location
 * @param loc2 Second connected location
 * @param type Connector type
 * @param info Descriptive information
 * @return New connector or NULL on failure
 */
quest_connector_t* quest_connector_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info
);

#endif
