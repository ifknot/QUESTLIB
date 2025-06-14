#ifndef QUEST_OBJECT_TYPES_H
#define QUEST_OBJECT_TYPES_H

// RTTI code one to one realtionship for the quest objects as expressed as C structs in the quest folder
typedef enum  {
    QUEST_NULL = 0,
    QUEST_CONTAINER, // ie quest_composite
    QUEST_LOCATION,
    QUEST_DOOR,
    QUEST_LOCK,
    QUEST_KEY,
    QUEST_SPELL,
    // todo...
    QUEST_SWORD,
    QUEST_CHEST,
    QUEST_GOLD,
    QUEST_RING,
    QUEST_SCROLL,
    QUEST_POTION,
    QUEST_INVENTORY,
    QUEST_PLAYER,
    QUEST_MERCHANT,
    QUEST_DUNGEON

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
