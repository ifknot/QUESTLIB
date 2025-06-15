/**
 * @file quest_composite.h
 * @brief Composite pattern implementation (v2.2)
 *
 * @version 2.2
 * @changelog
 * - v2.2: Enhanced dump functions with
 * - v2.1: Added feature bitmask to quest_component_t
 * - v2.0: Refactored to 16-bit features (lower=common, upper=type-specific)
 * - v1.4: Initial stable release
 *
 *
 * @details Provides a parent-child relationship management system for game entities with:
 *
 * - Memory safety through arena allocation
 * - Runtime type identification (RTTI)
 * - Fixed maximum children count for predictable behavior
 * - Type-safe operations with compile-time checks
 *
 * The composite pattern allows treating individual objects and compositions uniformly.
 * All game objects inherit from quest_component_t, while containers use quest_composite_t.
 *
 * Key Relationships Diagram
 * @dot
 * digraph quest_composite_v2 {
 *     graph [
 *         label=<<B>Quest Composite Object Model v2.1</B><BR/><FONT POINT-SIZE="10">features: uint16_t</FONT>>,
 *         labelloc="t",
 *         tooltip="Updated for feature bitmask",
 *         fontname="Courier New"
 *     ];
 *
 *     // Nodes with feature field
 *     Component [label=<
 *         <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *         <TR><TD COLSPAN="2" BGCOLOR="#2b2b2b"><FONT COLOR="#ffffff">quest_component_t</FONT></TD></TR>
 *         <TR><TD ALIGN="LEFT">+ rtti</TD><TD ALIGN="LEFT">quest_rtti_t</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ parent</TD><TD ALIGN="LEFT">quest_component_t*</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ info</TD><TD ALIGN="LEFT">quest_info_t*</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ features</TD><TD ALIGN="LEFT">uint16_t</TD></TR>
 *         </TABLE>
 *     >];
 *
 *     Composite [label=<
 *         <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *         <TR><TD COLSPAN="2" BGCOLOR="#2b5b84"><FONT COLOR="#ffffff">quest_composite_t</FONT></TD></TR>
 *         <TR><TD ALIGN="LEFT">+ base</TD><TD ALIGN="LEFT">quest_component_t</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ children[16]</TD><TD ALIGN="LEFT">quest_component_t*</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ child_count</TD><TD ALIGN="LEFT">quest_size_t</TD></TR>
 *         </TABLE>
 *     >, fillcolor="#e6f3ff"];
 *
 *     Info [label=<
 *         <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
 *         <TR><TD COLSPAN="2" BGCOLOR="#7a3b00"><FONT COLOR="#ffffff">quest_info_t</FONT></TD></TR>
 *         <TR><TD ALIGN="LEFT">+ brief</TD><TD ALIGN="LEFT">char*</TD></TR>
 *         <TR><TD ALIGN="LEFT">+ details</TD><TD ALIGN="LEFT">char*</TD></TR>
 *         </TABLE>
 *     >, fillcolor="#ffeee6"];
 *
 *     // ==============================================
 *     // Relationships (No C-style comments)
 *     // ==============================================
 *     // Inheritance
 *     Composite -> Component [label=<<I>inherits</I>>, arrowhead=empty];
 *
 *     // Composition
 *     Composite -> Component [
 *         label=<<B>contains 0..16</B>>,
 *         fontcolor="#0066cc",
 *         color="#0066cc"
 *     ];
 *
 *     // Ownership
 *     Component -> Info [
 *         label=<<B>owns</B>>,
 *         fontcolor="#cc3300",
 *         color="#cc3300"
 *     ];
 *
 *     // Reference
 *     Component -> Composite [
 *         label=<<I>optional parent</I>>,
 *         dir=back,
 *         fontcolor="#009933",
 *         color="#009933"
 *     ];
 *
 *     // ==============================================
 *     // Legend (Using HTML-like labels)
 *     // ==============================================
 *     subgraph cluster_legend {
 *         label=<<B>Legend</B>>;
 *         fontsize=10;
 *         color=none;
 *
 *         {
 *             node [shape=plaintext];
 *
 *             l1 [label=<<FONT COLOR="#000000">■</FONT> Inheritance>];
 *             l2 [label=<<FONT COLOR="#0066cc">■</FONT> Composition>];
 *             l3 [label=<<FONT COLOR="#cc3300">■</FONT> Ownership>];
 *             l4 [label=<<FONT COLOR="#009933">■</FONT> Reference>];
 *         }
 *     }
 *     // Add feature legend
 *     subgraph cluster_features {
 *         label=<<B>Feature Bits</B>>;
 *         fontsize=10;
 *
 *         f1 [label=<<TABLE BORDER="0" CELLBORDER="1">
 *             <TR><TD>0-7</TD><TD>Common</TD></TR>
 *             <TR><TD>8-15</TD><TD>Type-Specific</TD></TR>
 *         </TABLE>>, shape=none];
 *     }
 * }
 * @enddot
 */
#ifndef QUEST_COMPOSITE_H
#define QUEST_COMPOSITE_H

#include <stdbool.h>

#include "../MEM/mem_arena.h"

#include "quest_rtti.h"
#include "quest_constants.h"
#include "quest_types.h"
#include "quest_info.h"
#include "quest_objects.h"

/**
 * @brief Maximum number of child components any composite can contain
 *
 * @note This fixed-size design ensures:
 * - Memory safety (no dynamic allocation per composite)
 * - Predictable memory overhead (sizeof(quest_composite_t) = base + 16 pointers)
 * - Cache-friendly iteration (contiguous child array)
 *
 * @tradeoff Choosing this value:
 * - Too low: Limits complex object hierarchies
 * - Too high: Wastes memory for simple objects
 *
 * @default 16 is optimal for:
 * - RPG inventories (slots + equipped items)
 * - Room contents (NPCs + items + features)
 * - Nested containers (chests within chests)
 */
#define QUEST_COMPOSITE_MAX_CHILDREN    16

typedef struct quest_component_t quest_component_t;

/**
 * @brief Fundamental building block of all game entities
 *
 * @inheritance Every game object inherits from this base type through either:
 * - Direct composition (e.g., quest_item_t has a quest_component_t as first member)
 * - Containment (quest_composite_t's base field)
 *
 * @memory_layout
 * +-----------------------+
 * | rtti (16+ bits)       |
 * +-----------------------+
 * | parent* (machine word)|
 * +-----------------------+
 * | info* (ptr to strings)|
 * +-----------------------+
 */
typedef struct quest_component_t {
    /**
     * @brief Runtime Type Information (RTTI)
     * @see quest_rtti.h for bitfield layout
     * @invariant Must be first member to enable:
     * - Type-safe casting via (quest_type_t)component->rtti.parts.type
     * - Memory alignment guarantees
     */
    quest_rtti_t rtti;

    /**
     * @brief Hierarchical parent pointer
     * @behavior
     * - NULL for root-level objects (world, player)
     * - Automatically updated by quest_composite_add/remove
     * @warning Never modify directly - use composite API
     */
    quest_component_t* parent;

    /**
     * @brief Human-readable metadata
     * @owned_by This component (freed when component is destroyed)
     * @content Typical uses:
     * - brief: Short name ("Sword of Truth")
     * - details: Long description ("An ancient blade...")
     * @see quest_info.h for string management
     */
    quest_info_t* info;

    /**
    * @brief Bitmask of component capabilities/states
    * @usage
    * - Bits 0-7: Common features (shared across all components)
    * - Bits 8-15: Type-specific features
    * @see quest_features.h for standard feature definitions
    */
    quest_bitmask_t features;
} quest_component_t;

/**
 * @brief Container type that can parent other components
 *
 * @extends quest_component_t (inherits base fields)
 * @composition_pattern Composite nodes vs. Leaf nodes:
 * - Leaf:   Component without children (e.g., items)
 * - Composite: Has children (e.g., inventories, rooms)
 *
 * Memory Layout:
 * +------------------------+
 * | quest_composite_t      |
 * +------------------------+
 * | base (quest_component_t|
 * +------------------------+
 * | children[0]            | -> child_1
 * | ...                    |
 * | children[15]           | -> NULL
 * +------------------------+
 * | child_count            |
 * +------------------------+
 *
 * @performance Characteristics:
 * - O(1) add/remove via swap-with-last
 * - O(n) searches (linear scan)
 * - 64 bytes child pointers (on 32-bit arch)
 */
typedef struct {
    /**
     * @brief Inherited component properties
     * @note Must be first member to enable:
     * - Type casting: (quest_component_t*)composite
     * - Memory alignment with base type
     */
    quest_component_t base;

    /**
     * @brief Fixed-capacity child array
     * @layout
     * [0] -> child_1
     * ...
     * [child_count-1] -> last_child
     * [child_count] -> NULL (unused slot)
     * ...
     * [QUEST_COMPOSITE_MAX_CHILDREN-1] -> NULL
     * @invariant Maintained by API:
     * - All used slots are contiguous from [0]
     * - All unused slots are NULL
     */
    quest_component_t* children[QUEST_COMPOSITE_MAX_CHILDREN];

    /**
     * @brief Active children counter
     * @rules
     * - Always <= QUEST_COMPOSITE_MAX_CHILDREN
     * - On add: Incremented if successful
     * - On remove: Decremented
     * @optimization Used instead of NULL checks for faster iteration
     */
    quest_size_t child_count;

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
 * @brief Sets multiple feature flags at once
 * @param comp Target component (must not be NULL)
 * @param features Bitmask of features to enable (multiple bits allowed)
 * @return New feature bitmask
 *
 * @code
 * // Make object visible and interactable in one call
 * quest_component_set_features(sword, COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE);
 * @endcode
 */
quest_bitmask_t quest_component_set_features(quest_component_t* comp, quest_bitmask_t features);

/**
 * @brief Clears multiple feature flags at once
 * @param comp Target component (must not be NULL)
 * @param features Bitmask of features to disable (multiple bits allowed)
 * @return New feature bitmask
 *
 * @code
 * // Make door non-interactable and invisible
 * quest_component_clear_features(door,
 *     COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE);
 * @endcode
 */
quest_bitmask_t quest_component_clear_features(quest_component_t* comp, quest_bitmask_t features);

/**
 * @brief Checks if a component has specific feature flag(s) enabled
 * @param comp Component to check (must not be NULL)
 * @param feature Bitmask of features to check (can be single or combined flags)
 * @return true if ANY of the specified features are set, false otherwise
 *
 * @usage
 * // Check single feature
 * if (quest_component_has_feature(door, COMP_FEATURE_VISIBLE)) {
 *     render(door);
 * }
 *
 * // Check multiple features (OR condition)
 * if (quest_component_has_feature(chest,
 *     COMP_FEATURE_INTERACTABLE | COMP_FEATURE_VISIBLE)) {
 *     make_clickable(chest);
 * }
 *
 * @note
 * - For ALL features check, compare with full mask:
 *   (comp->features & mask) == mask
 * - Feature bits are organized as:
 *   - Bits 0-7: Common features (shared across all components)
 *   - Bits 8-15: Type-specific features
 *
 * @examples
 * // Door-specific check
 * bool is_locked = quest_component_has_feature(door, DOOR_FEATURE_LOCKED);
 *
 * // Common feature check
 * bool is_visible = quest_component_has_feature(any_obj, COMP_FEATURE_VISIBLE);
 *
 * @warning
 * - Does not validate feature type vs component type
 * - For type-safe checking see quest_component_has_type_feature()
 */
bool quest_component_has_feature(const quest_component_t* comp, quest_feature_t feature);

/**
 * @brief Checks if ALL specified features are enabled
 * @param comp Target component
 * @param features Bitmask of features to check (multiple bits allowed)
 * @return true if all specified features are active
 *
 * @code
 * if (quest_component_has_all_features(door,
 *     COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE)) {
 *     printf("Door is visible and interactable\n");
 * }
 * @endcode
 */
bool quest_component_has_all_features(const quest_component_t* comp, quest_bitmask_t features);

/**
 * @brief Checks if ANY specified features are enabled
 * @param comp Target component
 * @param features Bitmask of features to check (multiple bits allowed)
 * @return true if at least one specified feature is active
 */
bool quest_component_has_any_features(const quest_component_t* comp, quest_bitmask_t features);

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
 * @brief Gets the human-readable type name of a component
 * @param comp Component to examine (may be NULL)
 * @return String name of the component's type, or "NULL" if invalid
 *
 * @note Safely handles NULL components and invalid type values
 * @see quest_objects.h for the complete type list
 *
 * @code
 * // Cleaner usage without digging through rtti:
 * printf("Type: %s\n", quest_type_to_string((quest_component_t*)my_component));
 * @endcode
 */
const char* quest_type_to_string(quest_component_t* comp);

/**
 * @brief Safely retrieves the brief description of a component
 * @param comp Component to query (may be NULL)
 * @return The brief description string, or "(no brief)" if invalid
 *
 * @note This is a null-safe wrapper around comp->info->brief
 * @warning Returns a default string if either comp or comp->info is NULL
 *
 * @code
 * // Example usage:
 * printf("Item: %s\n", quest_info_brief((quest_component_t*)item));
 * @endcode
 */
const char* quest_info_brief(quest_component_t* comp);

/**
 * @brief Safely retrieves the detailed description of a component
 * @param comp Component to query (may be NULL)
 * @return The detailed description string, or "(no details)" if invalid
 *
 * @note This is a null-safe wrapper around comp->info->details
 * @warning Returns a default string if either comp or comp->info is NULL
 *
 * @code
 * // Example usage:
 * printf("Description: %s\n", quest_info_details((quest_component_t*)item));
 * @endcode
 */
const char* quest_info_details(quest_component_t* comp);

/**
 * @brief Dumps detailed component information to a stream
 * @param comp Component to dump (must not be NULL)
 * @param stream Output stream (must not be NULL)
 *
 * @output_format
 * [TYPE] "Brief Description" |LLLLLLLL:HHHHHHHH|
 *   Details... (trimmed to 40 chars)
 *   RTTI: [type:NAME serial:0xFFFF time:0xFFFF]
 *   Parent: [TYPE] "Parent Name" or NULL
 */
void quest_component_dump(const quest_component_t* comp, FILE* stream);

/**
 * @brief Dumps composite structure with children hierarchy
 * @param comp Composite to dump (must not be NULL)
 * @param stream Output stream (must not be NULL)
 *
 * @output_format
 * COMPOSITE [TYPE] "Name" (X children) |LLLLLLLL:HHHHHHHH|
 * ==================================
 * [Component dump of base]
 * ----------------------------------
 * CHILDREN:
 * [00] [TYPE] "Name" |LLLLLLLL:HHHHHHHH| @addr
 */
void quest_composite_dump(const quest_composite_t* comp, FILE* stream);

#endif
