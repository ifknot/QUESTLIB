/**
 * @file quest_composite.h
 * @brief Composite pattern implementation for game object hierarchies
 *
 * Provides parent-child relationship management for game entities using:
 * - Fixed-size arrays for memory safety
 * - RTTI fingerprints for identification
 * - Error-checked operations
 */
#ifndef QUEST_COMPOSITE_H
#define QUEST_COMPOSITE_H

#include "quest_rtti.h"
#include "quest_errors.h"
#include "quest_constants.h"

typedef struct quest_component_t {
    quest_rtti_t rtti;      ///< Unique type+ID combination
    struct quest_component_t* parent; ///< Parent container (NULL if root)
} quest_component_t;

typedef struct {
    quest_component_t base;       ///< Base component properties
    quest_component_t* children[QUEST_COMPOSITE_MAX_CHILDREN]; ///< Child array
    quest_size_t child_count;     ///< Current number of children
} quest_composite_t;

/**
 * @brief Adds a child to a composite
 * @param parent Container to receive child
 * @param child Component to add
 * @return QUEST_SUCCESS, QUEST_ITEM_LIST_FULL, or QUEST_INVALID_ARGS
 *
 * @example Adding a sword to inventory:
 * @code
 * quest_component_t* sword = create_sword();
 * if (quest_composite_add(player->inventory, sword) != QUEST_SUCCESS) {
 *     printf("Failed to add sword!\n");
 * }
 * @endcode
 */
quest_error_t quest_composite_add(
    quest_composite_t* parent,
    quest_component_t* child
);

/**
 * @brief Removes a child by fingerprint
 * @param parent Container to modify
 * @param fingerprint Unique identifier to remove
 * @return QUEST_SUCCESS, QUEST_ITEM_NOT_FOUND, or QUEST_ITEM_LIST_EMPTY
 *
 * @example Removing item by ID:
 * @code
 * quest_fingerprint_t target = {QUEST_ITEM_POTION, 42};
 * if (quest_composite_remove(bag, target) == QUEST_SUCCESS) {
 *     printf("Removed potion 42\n");
 * }
 * @endcode
 */
quest_error_t quest_composite_remove(
    quest_composite_t* parent,
    quest_fingerprint_t fingerprint
);

/**
 * @brief Finds a child by fingerprint
 * @param parent Container to search
 * @param fingerprint Unique identifier to find
 * @return Pointer to component or NULL
 *
 * @example Finding equipped armor:
 * @code
 * quest_component_t* helm = quest_composite_find(
 *     player->equipment,
 *     (quest_fingerprint_t){QUEST_ARMOR_HELMET, 1}
 * );
 * if (helm) apply_armor_bonus(helm);
 * @endcode
 */
quest_component_t* quest_composite_find(
    quest_composite_t* parent,
    quest_fingerprint_t fingerprint
);

/**
 * @brief Transfers all children between composites
 * @param src Source container
 * @param dst Destination container
 * @return Transfer status code
 *
 * @example Looting a chest:
 * @code
 * switch (quest_composite_transfer_all(chest, player->inventory)) {
 *     case QUEST_SUCCESS: printf("Looted everything!\n"); break;
 *     case QUEST_ITEM_LIST_PARTIAL: printf("Inventory full!\n"); break;
 * }
 * @endcode
 */
quest_error_t quest_composite_transfer_all(
    quest_composite_t* src,
    quest_composite_t* dst
);

/**
 * @brief Transfers specific type between composites
 * @param src Source container
 * @param dst Destination container
 * @param target_type Type filter for transfer
 * @return Number of items transferred
 *
 * @example Stealing gold:
 * @code
 * quest_size_t gold_stolen = quest_composite_transfer_type(
 *     merchant->purse,
 *     player->inventory,
 *     QUEST_ITEM_GOLD
 * );
 * printf("Stole %zu gold pieces!\n", gold_stolen);
 * @endcode
 */
quest_error_t quest_composite_transfer_type(
    quest_composite_t* src,
    quest_composite_t* dst,
    quest_type_t target_type
);

#endif
