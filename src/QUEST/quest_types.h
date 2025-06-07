/**
 * @file quest_types.h
 * @brief Location data structures for Quest game engine
 *
 * Defines core types for storing location descriptions and metadata.
 */
#ifndef QUEST_TYPES_H
#define QUEST_TYPES_H

#include <stdint.h>

/**
 * @brief Type for size/length values throughout the Quest engine
 *
 * Uses 16-bit unsigned integers for memory efficiency in large world maps.
 * Maximum value: 65,535 (UINT16_MAX)
 */
typedef uint16_t quest_size_t;

/**
 * @brief 32-bit RTTI fingerprint (type + unique instance ID).
 *
 * Layout:
 * - Bits 0-15: Unique instance ID (auto-incremented).
 * - Bits 16-31: Object type ID (user-defined).
 */
typedef uint32_t quest_fingerprint_t;

/** @brief 16-bit object type identifier (e.g., "Player", "Enemy"). */
typedef uint16_t quest_type_t;

/**
 * @brief Dual-description container for game locations
 *
 * Contains both short and long form descriptions for:
 * - Room/area titles (brief)
 * - Detailed environmental descriptions (details)
 *
 * @note Both strings should be null-terminated UTF-8.
 * @warning Memory management policy:
 * - Allocated by location loader
 * - Freed by location unloader
 * - Never modified after creation
 */
typedef struct {
    char* brief;    /**< Short identifier (e.g., "Dark Forest Entrance")
                        - Max 255 chars recommended */
    char* details;  /**< Full description (e.g., "The twisted oaks creak...")
                        - Supports multi-paragraph text */
} quest_info_t;

#endif

/*
typedef struct quest_item_s quest_item_t;

typedef struct quest_item_s {
    int    id;
    char*  brief;
    char*  details;
    quest_item_t* items[8];  // pointers to upto 8 items - within this item
} quest_item_t;

typedef struct {    // a location is an item with location info
    quest_item_t* item;
    char   exits;            // 8 bits of direction options |N|E|S|W|U|D|L|R| north, south, east, west, up, down, left, right 1 if exit valid 0 otherwise
    int    destination[8];   // 8 location IDs associated with each valid exit
    quest_dictionary_t* local_dict; // parse dictionary specific to this room additional to the global dictionary (to avoid users spamming the input with verbs and nouns to cheat on the story)
} quest_location_t;

typedef struct { // D&D 5e, the six ability scores (Strength, Dexterity, Constitution, Intelligence, Wisdom, and Charisma)
    char strength;
    char dexterity;
    char constitution;
    char intelligence;
    char wisdom;
    char charisma;
} quest_character_core_t;

typedef struct {   //  a character is an item with character information
    quest_item_t* item;    // id class Abilities eg fighter ability to use a battle axe, while a wizard able to cast spells.
    char life;    // reduced by the cost of living increased by food fluid and manna spells
    quest_character_skills_t* skills[8]; // upto eight skills can be added and enhanced or reduced
    quest_character_core_t core;
} quest_character_t;    // e.g. Troll, Gaurd,

typedef struct { // a player is a character with player information
    quest_character_t* character;
    int items_max;
} quest_player_t;

*/
