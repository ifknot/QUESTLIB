/**
 * @file quest_errors.h
 * @brief Error handling system for Quest game engine
 *
 * Provides standardized error codes and human-readable translations
 * for game systems and subsystems.
 */

#ifndef QUEST_ERRORS_H
#define QUEST_ERRORS_H

#include <stdint.h>

/**
 * @brief Standard error codes for Quest engine operations
 *
 * All error codes are negative except QUEST_SUCCESS (0).
 * New error codes should follow the <SYSTEM>_<CONDITION> pattern.
 *
 * // Usage example:
 * quest_error_t result = quest_composite_transfer_all(chest, player_inventory);
 * if (result != QUEST_SUCCESS) {
 *    printf("Transfer failed: %s\n", quest_strerror(result));
 * }
 */
typedef enum {
    /* Core system errors (0-99) */
    QUEST_SUCCESS = 0,            ///< Operation completed successfully
    QUEST_FAILURE = -1,           ///< Generic failure condition

    /* Inventory system errors (100-199) */
    QUEST_ITEM_LIST_FULL = -100,  ///< Cannot add item - inventory at capacity
    QUEST_ITEM_LIST_EMPTY = -101, ///< Cannot remove item - inventory empty
    QUEST_ITEM_NOT_FOUND = -102,  ///< Specified item does not exist in inventory
    QUEST_ITEM_LIST_PARTIAL = -103, ///< Cannot move all items

    /* Location system errors (200-299) */
    QUEST_LOCATION_INVALID = -200, ///< Target location does not exist
    QUEST_LOCATION_LOCKED = -201,  ///< Target location is access-restricted

    /* Connector system errors */
    QUEST_WRONG_KEY = - 300,
    QUEST_ALREADY_UNLOCKED  = - 301,
    QUEST_ALREADY_LOCKED  = - 302,
    QUEST_DOOR_LOCKED = -303,
    QUEST_DOOR_UNLOCKED = -304,

    /* Resource errors (400-499) */
    QUEST_MEMORY_ERROR = -400,     ///< Memory allocation failure
    QUEST_FILE_ERROR = -401,        ///< File I/O operation failed



} quest_error_t;

/**
 * @brief Human-readable error messages
 *
 * Indexed by absolute value of quest_error_t codes.
 * Must be kept synchronized with the enum above.
 */
static const char* const quest_error_messages[] = {
    /* Core system (0-1) */
    [QUEST_SUCCESS] = "Success",
    [1] = "Unknown failure",  // Absolute value of QUEST_FAILURE

    /* Inventory system (100-102) */
    [100] = "Inventory full - cannot add item",
    [101] = "Inventory empty - cannot remove item",
    [102] = "Item not found in inventory",

    /* Location system (200-201) */
    [200] = "Invalid location reference",
    [201] = "Location is locked - access denied",

    /* Combat system (300) */
    [300] = "Target is immune to this attack",

    /* Resource errors (400-401) */
    [400] = "Memory allocation failed",
    [401] = "File operation failed"
};

/**
 * @brief Gets human-readable error message
 * @param err Error code (quest_error_t)
 * @return Corresponding error message string
 * @note Returns "Unknown error" for invalid codes
 */
static inline const char* quest_strerror(quest_error_t err) {
    uint16_t code;

    if (err == QUEST_FAILURE) {
        code = 1;
    } else {
        code = (uint16_t)(-err);
    }

    if (code < sizeof(quest_error_messages)/sizeof(quest_error_messages[0])) {
        return quest_error_messages[code];
    }
    return "Unknown error";
}

#endif /* QUEST_ERRORS_H */
