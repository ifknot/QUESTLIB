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

#include "../MEM/mem_arena.h"

#include "quest_rtti.h"
#include "quest_errors.h"
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_info.h"

typedef struct quest_component_t quest_component_t;

typedef struct quest_component_t {
    quest_rtti_t rtti;      ///< Unique type+ID combination
    quest_component_t* parent; ///< Parent container (NULL if root)
    quest_info_t* info;     //< mutable string information
} quest_component_t;

typedef struct {
    quest_component_t base;       ///< Base component properties
    quest_component_t* children[QUEST_COMPOSITE_MAX_CHILDREN]; ///< Child array
    quest_size_t child_count;     ///< Current number of children
} quest_composite_t;

/**
 * @brief Creates a new game component
 * @param arena Memory arena for allocation (must not be NULL)
 * @param parent Parent component or NULL for root objects
 * @param type Component type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 * @return New component or NULL on failure
 *
 * @warning The info structure becomes owned by the component and will be
 *          freed when the arena is destroyed
 *
 * @code
 * // Example 1: Creating a root object
 * quest_info_t* rock_info = quest_info_copy(arena, "Rock", "A mossy boulder");
 * quest_component_t* rock = quest_component_create(arena, NULL, QUEST_OBJECT, rock_info);
 *
 * // Example 2: Creating a child object
 * quest_info_t* gem_info = quest_info_create(arena, 32, 128);
 * str_copy(gem_info->brief, "Sapphire");
 * str_copy(gem_info->details, "Glows with inner light");
 * quest_component_t* gem = quest_component_create(arena, parent, QUEST_TREASURE, gem_info);
 * @endcode
 */
quest_component_t* quest_component_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Creates a new container composite
 * @param arena Memory arena for allocation (must not be NULL)
 * @param parent Parent component or NULL for root containers
 * @param type Composite type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 * @return New composite or NULL on failure
 *
 * @note Child slots are pre-initialized to NULL
 *
 * @code
 * // Example 1: Creating a room
 * quest_info_t* tavern_info = quest_info_copy(arena, "Tavern", "Smoky and loud...");
 * quest_composite_t* tavern = quest_composite_create(arena, NULL, QUEST_LOCATION, tavern_info);
 *
 * // Example 2: Creating player inventory
 * quest_info_t* inv_info = quest_info_link(arena, "Inventory", "Carried items");
 * quest_composite_t* inventory = quest_composite_create(arena, player, QUEST_INVENTORY, inv_info);
 *
 * // Adding items
 * quest_component_t* sword = quest_component_create(arena, NULL, QUEST_WEAPON, ...);
 * quest_composite_add(inventory, sword);
 * @endcode
 */
quest_composite_t* quest_composite_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);
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
 * @param child to remove
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
    quest_component_t* child
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
    quest_composite_t* dst,
    quest_composite_t* src
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
    quest_composite_t* dst,
    quest_composite_t* src,
    quest_type_t target_type
);

/**
 * @brief Dump human readable a component to a stream
 * @param comp Component to dump
 * @param stream Output stream
 */
void quest_component_dump(const quest_component_t* comp, FILE* stream);

/**
 * @brief Serializes composite with all children
 * @param comp Composite to dump
 * @param stream Output stream
 */
void quest_composite_dump(const quest_composite_t* comp, FILE* stream);

#endif
