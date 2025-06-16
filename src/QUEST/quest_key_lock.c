#include "quest_key_lock.h"

void quest_key_init(
    quest_key_t* key, 
    quest_component_t* parent,
    quest_type_t type, 
    quest_info_t* info,
    quest_size_t durability
) {      
    quest_component_init(&key->base, parent, type, info);
    key->durability = (durability > 100) ? 100 : durability;    // capped at 100%
}

quest_key_t* quest_key_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info, 
    quest_size_t durability
) {
    assert(arena && "NULL memory arena!");
    
    quest_key_t* key = mem_arena_calloc(arena, sizeof(quest_key_t));
    quest_key_init(key, type, info, durability);
    return key;
}

void quest_lock_init(
    quest_lock_t* lock, 
    quest_component_t* parent,
    quest_type_t type, 
    quest_info_t* info,
    quest_size_t key_max
) {    
    assert(key_max && "LOCK with no possible key!");
    
    quest_component_init(&lock->base, parent, type, info);
    lock->code = quest_prng_generate(quest_global_ctx);
    lock->key_max = key_max;
}

quest_lock_t* quest_lock_create(
    mem_arena_t* arena, 
    quest_type_t type,
    quest_info_t* info,
    quest_size_t key_max
) {
    assert(arena && "NULL memory arena!");
    
    quest_lock_t* lock = mem_arena_calloc(arena, sizeof(quest_lock_t));
    quest_lock_init(lock, parent, type, info, key_max);
    return lock;
}

bool quest_key_lock_imprint(
    quest_lock_t* lock,
    quest_key_t* key
) {
    assert(lock && "NULL lock!");
    assert(key && "NULL key!");

    if(key->key_code != 0 || lock->key_count == lock->key_max) {
        return false;            
    } 
    key->code = lock->code;
    lock->key_count++;
    return true;
}

bool quest_lock_is_locked(quest_lock_t* lock) {
    return quest_component_has_feature((quest_component_t*)lock, QUEST_LOCK_IS_LOCKED);
}

bool quest_lock_try_unlock(quest_lock_t* lock, quest_key_t* key) {
    if(quest_lock_is_locked(lock) && lock->code == key->code) {
        quest_component_clear_features((quest_component_t*)lock, QUEST_LOCK_IS_LOCKED);
        return true;
    }
    return false;
}

bool quest_lock_try_lock(quest_lock_t* lock, quest_key_t* key) {
    if(!quest_lock_is_locked(lock) && lock->code == key->code) {
        quest_component_set_features((quest_component_t*)lock, QUEST_LOCK_IS_LOCKED);
        return true;
    }
    return false;
}
