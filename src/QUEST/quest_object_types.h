#ifndef QUEST_OBJECT_TYPES_H
#define QUEST_OBJECT_TYPES_H

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,

    QUEST_LOCATION = 10,
    QUEST_LOCATION_END = QUEST_LOCATION + 1,

    QUEST_LOCK = QUEST_LOCATION_END,
    QUEST_LOCK_RUSTY = QUEST_LOCK + 1,
    QUEST_LOCK_END = QUEST_LOCK_RUSTY +1,

    QUEST_KEY = QUEST_LOCK_END + 1

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
