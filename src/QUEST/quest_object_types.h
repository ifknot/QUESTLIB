#ifndef QUEST_OBJECT_TYPES_H
#define QUEST_OBJECT_TYPES_H

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_LOCATION,
    QUEST_CONTAINER,
    QUEST_ITEM_SWORD,
    QUEST_CHEST,
    QUEST_ITEM_GOLD,
    QUEST_ITEM_RING,
    QUEST_INVENTORY,
    QUEST_ITEM_SCROLL,
    QUEST_ITEM_POTION,
    QUEST_MERCHANT,
    QUEST_DUNGEON,
    QUEST_CHARACTER,
    QUEST_PLAYER,
    QUEST_CONNECTOR_PASSAGE,
    QUEST_CONNECTOR_STAIRS,
    QUEST_ITEM_KEY,
    QUEST_DOOR_WOODEN
} quest_object_t;

/*
 to extend for a game

 typedef enum {
    QUEST_TROLL = QUEST_OBJECT,
    QUEST_WIZARD,
    QUEST_KEY,
    QUEST_SWORD
    ...
 } my_quest_object_t
*/

#endif
