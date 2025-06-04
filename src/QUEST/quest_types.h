#ifndef QUEST_LOCATION_H
#define QUEST_LOCATION_H

typedef struct {
    int    id;              
    char*  brief;
    char*  details;
    char   exits;            // 8 bits of direction options |N|E|S|W|U|D|L|R| north, south, east, west, up, down, left, right 1 if exit valid 0 otherwise 
    int    destination[8];   // 8 location IDs associated with each valid exit 
    quest_item_t* items[8];  // pointers to upto 10 items 
    quest_dictionary_t* local_dict; // parse dictionary specific to this room additional to the global dictionary (to avoid users spamming the input with verbs and nouns to cheat on the story)
} quest_location_t;

typedef struct { // D&D 5e, the six ability scores (Strength, Dexterity, Constitution, Intelligence, Wisdom, and Charisma) 
    char strength; 
    char dexterity; 
    char constitution;
    char intelligence;
    char wisdom;
    char charisma; 
} quest_player_core_t;

typedef struct {
    
} quest_character_t;

typedef struct {
    int id;
    quest_character_t* character;
    int items_max;
    quest_item_t* items[8];
} quest_player_t;

#endif
