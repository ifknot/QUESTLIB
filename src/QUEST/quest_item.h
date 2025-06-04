#ifndef QUEST_ITEM_H
#define QUEST_ITEM_H

#include "../MEM/mem_arena.h"

#include "quest_constants.h"
#include "quest_types.h"
#include "quest_errors.h"

// base object in quest hierarchy

typedef enum { // 4 primal types
    QUEST_ITEM,
    QUEST_LOCATION,
    QUEST_CHARACTER,
    QUEST_PLAYER
} quest_rtti_t; // type information not limited to these 4 but beware overlapping values


typedef struct quest_item_s quest_item_t;

// every object in quest is-a item, every item has-a parent, type, state, info and 0 or more other items
typedef struct quest_item_s { // base object
    quest_item_t* parent;
    quest_rtti_t type;
    quest_state_t state;
    quest_info_t info;
    quest_item_t* children[QUEST_MAX_ITEMS];    // null terminated unordered list of child items
} quest_item_t;

quest_item_t* quest_item_create(mem_arena_t* arena, quest_item_t* parent, quest_rtti_t type, quest_state_t state, char* brief, char* details);

quest_error_t quest_item_add(quest_item_t* parent, quest_item_t* item);

quest_item_t* quest_item_find(quest_item_t* parent, char* brief);

quest_error_t quest_item_remove(quest_item_t* item);

quest_error_t quest_item_move(quest_item_t* source, quest_item_t* parent, quest_item_t* item);

#endif
