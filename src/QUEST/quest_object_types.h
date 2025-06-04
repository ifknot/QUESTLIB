#ifndef QUEST_OBJECT_TYPES_H
#define QUEST_OBJECT_TYPES_H

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_LOCATION,
    QUEST_CHARACTER,
    QUEST_PLAYER,
    QUEST_OBJECT
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
