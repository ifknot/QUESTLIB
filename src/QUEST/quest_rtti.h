#ifndef QUEST_RTTI_H
#define QUEST_RTTI_H

#include <stdint.h>

// extend as needed - every object in a quest game universe must have its own unique RTTI fingerprint
// the fingerprint is 32 bits wide and made up a 16 bit object type and a 16 bit unique instantiation identifier

typedef struct  private_quest_uid_t quest_uid_t;

typedef union {
    uint32_t rtti;
    uint16_t type;
    uint16_t uid;
} quest_rtti_t;


inline

#endif
