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

#include <stdbool.h>

#include "../MEM/mem_arena.h"

#include "quest_rtti.h"
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_info.h"

typedef struct quest_spell_t quest_spell_t;    // forward declaration

typedef struct quest_component_t quest_component_t;

/**
 * @brief everything in a quest world is-a component
 */
typedef struct quest_component_t {
    quest_rtti_t rtti;          ///< Unique type+ID combination
    quest_component_t* parent;  ///< Parent container (NULL if root)
    quest_info_t* info;         //< mutable string information
    quest_spell_t* spell;       //< any item in a quest world can have spell plaed upon it
} quest_component_t;

typedef struct {
    quest_component_t base;       ///< Base component properties
    quest_component_t* children[QUEST_COMPOSITE_MAX_CHILDREN]; ///< Child array
    quest_size_t child_count;     ///< Current number of children
} quest_composite_t;

/**
 * @brief Initializes a component structure with provided values
 * @param comp Pointer to pre-allocated component structure
 * @param parent Parent component or NULL for root objects
 * @param type Component type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 *
 * @code
 * // Example: Initializing a component in custom memory
 * quest_component_t custom_comp;
 * quest_info_t* info = quest_info_create(arena, "Rock", "A mossy boulder");
 * quest_component_init(&custom_comp, NULL, QUEST_OBJECT, info);
 * @endcode
 */
void quest_component_init(
    quest_component_t* comp,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);

/**
 * @brief Creates a new game component
 * @param arena Memory arena for allocation (must not be NULL)
 * @param parent Parent component or NULL for root objects
 * @param type Component type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 * @return New component or NULL on failure
 *
 * @code
 * // Example 1: Creating a root object
 * quest_info_t* rock_info = quest_info_create(arena, "Rock", "A mossy boulder");
 * quest_component_t* rock = quest_component_create(arena, NULL, QUEST_OBJECT, rock_info);
 *
 * // Example 2: Creating a child object
 * quest_info_t* gem_info = quest_info_create(arena, "Sapphire", "Glows with inner light");
 * quest_component_t* gem = quest_component_create(arena, parent, QUEST_TREASURE, gem_info);
 * @endcode
 */
quest_component_t* quest_component_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info
);

bool quest_component_is_spellbound();

/**
 * @brief any item in a quest world can have a spell cast upon it, unless:
 * 1. component already spellbound with a stronger spell
 * 2. component has a no spell cast upon it
 * If a stronger spell is cast it replaces the weaker spell which is returned
 */
quest_spell_t* quest_component_cast_spell(quest_component_t* comp, quest_spell_t* spell);

/**
 * @brief in order to undo (remove) a spell a *matching* undo spell must be cast, after which the item is open to having another spell cast upon it.
 */
quest_spell_t* quest_component_undo_spell(quest_component_t* comp, quest_undo_spell_t* undo_spell);

/**
 * @brief Initializes a composite structure with provided values
 * @param comp Pointer to pre-allocated composite structure
 * @param parent Parent component or NULL for root containers
 * @param type Composite type from quest_object_types.h
 * @param info Pre-allocated info structure (ownership transferred)
 *
 * @code
 * // Example: Initializing a composite in custom memory
 * quest_composite_t custom_comp;
 * quest_info_t* info = quest_info_create(arena, "Chest", "Wooden treasure chest");
 * quest_composite_init(&custom_comp, NULL, QUEST_CONTAINER, info);
 * @endcode
 */
void quest_composite_init(
    quest_composite_t* comp,
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
 * @code
 * // Example 1: Creating a room
 * quest_info_t* tavern_info = quest_info_create(arena, "Tavern", "Smoky and loud...");
 * quest_composite_t* tavern = quest_composite_create(arena, NULL, QUEST_LOCATION, tavern_info);
 *
 * // Example 2: Creating player inventory
 * quest_info_t* inv_info = quest_info_create(arena, "Inventory", "Carried items");
 * quest_composite_t* inventory = quest_composite_create(arena, player, QUEST_INVENTORY, inv_info);
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
 *
 * @code
 * // Example 1: Adding a sword to inventory
 * quest_component_t* sword = quest_component_create(arena, NULL, QUEST_WEAPON,
 *     quest_info_create(arena, "Sword", "Sharp blade"));
 * quest_composite_add(player->inventory, sword);
 *
 * // Example 2: Adding multiple items
 * quest_component_t* items[] = {
 *     quest_component_create(arena, NULL, QUEST_ITEM, ...),
 *     quest_component_create(arena, NULL, QUEST_ARMOR, ...)
 * };
 * for (int i = 0; i < 2; i++) {
 *     quest_composite_add(chest, items[i]);
 * }
 * @endcode
 */
void quest_composite_add(
    quest_composite_t* parent,
    quest_component_t* child
);

/**
 * @brief Removes a child by fingerprint
 * @param parent Container to modify
 * @param child to remove
 * @return Pointer to *parent-less* child component or NULL
 *
 * @code
 * // Example: Removing and transferring an item
 * quest_component_t* removed = quest_composite_remove(chest, gold_coins);
 * if (removed) {
 *     quest_composite_add(player->inventory, removed);
 * }
 * @endcode
 */
quest_component_t* quest_composite_remove(
    quest_composite_t* parent,
    quest_component_t* child
);

/**
 * @brief Finds a child by fingerprint
 * @param parent Container to search
 * @param fingerprint Unique identifier to find
 * @return Pointer to child component or NULL
 *
 * @code
 * // Example 1: Finding equipped armor
 * quest_component_t* helm = quest_composite_find(
 *     player->equipment,
 *     (quest_fingerprint_t){QUEST_ARMOR_HELMET, 1}
 * );
 * if (helm) apply_armor_bonus(helm);
 *
 * // Example 2: Finding by dynamic type
 * quest_component_t* first_gold = quest_composite_find(
 *     inventory,
 *     (quest_fingerprint_t){QUEST_ITEM_GOLD, 0} // ID 0 matches any gold
 * );
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
 * @return Number of items transferred
 *
 * @code
 * // Example: Looting a chest
 * size_t looted = quest_composite_transfer_all(player->inventory, chest);
 * printf("Looted %zu items!\n", looted);
 * @endcode
 */
quest_size_t quest_composite_transfer_all(
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
 * @code
 * // Example 1: Stealing gold
 * quest_size_t gold_stolen = quest_composite_transfer_type(
 *     player->inventory,
 *     merchant->purse,
 *     QUEST_ITEM_GOLD
 * );
 * printf("Stole %zu gold pieces!\n", gold_stolen);
 *
 * // Example 2: Collecting all weapons
 * quest_size_t weapons_collected = quest_composite_transfer_type(
 *     armory,
 *     player->inventory,
 *     QUEST_WEAPON
 * );
 * @endcode
 */
quest_size_t quest_composite_transfer_type(
    quest_composite_t* dst,
    quest_composite_t* src,
    quest_type_t target_type
);

/**
 * @brief Dump human readable a component to a stream
 * @param comp Component to dump
 * @param stream Output stream
 *
 * @code
 * // Example: Debugging a component
 * quest_component_dump(my_item, stdout);
 * @endcode
 */
void quest_component_dump(const quest_component_t* comp, FILE* stream);

/**
 * @brief Serializes composite with all children
 * @param comp Composite to dump
 * @param stream Output stream
 *
 * @code
 * // Example: Debugging a composite structure
 * quest_composite_dump(tavern, stdout);
 * @endcode
 */
void quest_composite_dump(const quest_composite_t* comp, FILE* stream);

#endif
