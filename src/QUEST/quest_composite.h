/**
 * @file quest_composite.h
 * @brief Composite pattern implementation for game object hierarchies
 *
 * @details Provides a parent-child relationship management system for game entities with:
 * - Memory safety through arena allocation
 * - Runtime type identification (RTTI)
 * - Fixed maximum children count for predictable behavior
 * - Type-safe operations with compile-time checks
 *
 * The composite pattern allows treating individual objects and compositions uniformly.
 * All game objects inherit from quest_component_t, while containers use quest_composite_t.
 */
#ifndef QUEST_COMPOSITE_H
#define QUEST_COMPOSITE_H

#include <stdbool.h>

#include "../MEM/mem_arena.h"

#include "quest_rtti.h"
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_info.h"

typedef struct quest_component_t quest_component_t;

/**
 * @brief everything in a quest world is-a component
 */
typedef struct quest_component_t {
    quest_rtti_t rtti;          ///< Unique type+ID combination
    quest_component_t* parent;  ///< Parent container (NULL if root)
    quest_info_t* info;         //< mutable string information
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
 * @brief Finds a specific child by its unique fingerprint
 * @param parent Container to search
 * @param fingerprint Unique identifier to find
 * @return Pointer to child component or NULL
 *
 * @code
 *
 * @endcode
 */
quest_component_t* quest_composite_find_fingerprint(
    quest_composite_t* parent,
    quest_fingerprint_t fingerprint
);

/**
 * @brief Counts children of a specific type in a composite
 * @param parent Container to search (must not be NULL)
 * @param target_type Type identifier to count
 * @return Number of children matching the target type
 *
 * @note This is an O(n) operation that scans all children.
 *       For frequent type counting, consider maintaining separate counters.
 *
 * @code
 * // Example: Counting treasure in a room
 * quest_size_t treasure_count = quest_composite_count_type(room, QUEST_TREASURE);
 * printf("Room contains %zu treasure items\n", treasure_count);
 * @endcode
 */
quest_size_t quest_composite_count_type(
    quest_composite_t* parent,
    quest_type_t target_type
);

/**
 * @brief Finds a child component by its unique fingerprint identifier
 * @param parent Composite container to search (must not be NULL)
 * @param fingerprint Unique identifier to locate (see quest_rtti.h)
 * @return Pointer to found child component, or NULL if not found
 *
 * @details Fingerprints provide O(1) comparison but require O(n) search time.
 *          For better performance with frequent lookups, consider maintaining
 *          a separate hash table or dictionary structure.
 *
 * @note Fingerprints are guaranteed unique across all game objects and remain
 *       constant for an object's lifetime. They combine:
 *       - Type information
 *       - Unique serial number
 *       - Creation timestamp
 *
 */
quest_component_t* quest_composite_find_type(
    quest_composite_t* parent,
    quest_type_t target_type
);

/**
 * @brief Collects all children of a specific type into an array
 * @param parent Composite container to search (must not be NULL)
 * @param target_type Component type to enumerate (from quest_types.h)
 * @param enumeration [out] Pre-allocated array to store results (must have capacity for all matching children)
 * @return Number of matching components found and stored in enumeration array
 *
 * @details This function provides efficient bulk processing of components by:
 * - Performing a single O(n) scan of children
 * - Filtering by type in the same pass
 * - Storing results in caller-provided memory
 *
 * @note For empty composites or no matches, returns 0 and leaves enumeration unchanged
 *
 * @warning Caller must ensure enumeration array has sufficient capacity (can check via quest_composite_count_type first)
 *
 * @code
 * // --- EXAMPLE 1: Basic Type Enumeration ---
 * quest_composite_t* dungeon_room = ;
 * quest_component_t* weapons[10]; // Enough for max expected
 *
 * quest_size_t weapon_count = quest_composite_enumerate_type(
 *     dungeon_room,
 *     QUEST_WEAPON,
 *     weapons
 * );
 *
 * for (quest_size_t i = 0; i < weapon_count; i++) {
 *     printf("Weapon %zu: %s\n", i+1, weapons[i]->info->name);
 * }
 *
 * // --- EXAMPLE 2: Safe Enumeration with Capacity Checking ---
 * #define MAX_TREASURE 20
 * quest_composite_t* dragon_hoard = ;
 * quest_component_t* treasure[MAX_TREASURE];
 *
 * // Check capacity first
 * quest_size_t treasure_types = quest_composite_count_type(dragon_hoard, QUEST_TREASURE);
 * if (treasure_types > MAX_TREASURE) {
 *     printf("Warning: Too much treasure (%zu), only taking %d\n",
 *            treasure_types, MAX_TREASURE);
 * }
 *
 * // Safe enumeration with array bounds
 * quest_size_t collected = quest_composite_enumerate_type(
 *     dragon_hoard,
 *     QUEST_TREASURE,
 *     treasure
 * );
 *
 * // --- EXAMPLE 3: Processing Specific Entity Types ---
 * // Define important NPC types
 * const quest_type_t NPC_TYPES[] = {
 *     QUEST_MERCHANT,
 *     QUEST_GUARD,
 *     QUEST_QUESTGIVER
 * };
 *
 * quest_composite_t* town = ;
 * quest_component_t* npcs[50];
 * quest_size_t total_npcs = 0;
 *
 * // Collect all types of NPCs
 * for (int i = 0; i < sizeof(NPC_TYPES)/sizeof(NPC_TYPES[0]); i++) {
 *     total_npcs += quest_composite_enumerate_type(
 *         town,
 *         NPC_TYPES[i],
 *         npcs + total_npcs // Append to array
 *     );
 * }
 *
 * printf("Town contains %zu NPCs\n", total_npcs);
 * @endcode
 */
quest_size_t quest_composite_enumerate_type(
    quest_composite_t* parent,
    quest_type_t target_type,
    quest_component_t** enumeration
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
