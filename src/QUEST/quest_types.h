#ifndef QUEST_LOCATION_H
#define QUEST_LOCATION_H

typedef struct {
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
    quest_item_t* items[8];    // upto 8 items, such as sword, chest, spell, bread etc etc
    quest_character_core_t core;
} quest_character_t;    // e.g. Troll, Gaurd, 

typedef struct { // a player is a character with player information
    quest_character_t* character;
    int items_max;
} quest_player_t;

#endif
