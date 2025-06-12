#ifndef QUEST_LOCK_H
#define QUEST_LOCK_H

typedef enum {
    LOCK_IS_LOCKED   = 0x0001,    
    LOCK_IS_RUSTY    = 0x0002,
    HAS_HAS_SPELL   `= 0x0004      // has one or more spells on it
} quest_door_features_t;

typedef struct quest_lock_t {
    quest_composite_t base;    
    quest_bitmask_t active_features; // defaults to unlocked not rusty 
    quest_size_t combination;
} quest_lock_t;

void quest_lock_init(    // defaults to an open lock
    quest_lock_t* door,
    quest_type_t type,
    quest_info_t* info
);

quest_door_t* quest_lock_create(
    mem_arena_t* arena,
    quest_lock_t* lock,
    quest_type_t type,
    quest_info_t* info,
);

#endif
