/**
 * @file quest_rtti.c
 * @brief RTTI System Implementation
 *
 * Uses a union to overlay a 32-bit fingerprint with its 16-bit components.
 */

#include "quest_rtti.h"
#include "quest_types.h"
#include "quest_uid.h"

quest_rtti_t quest_rtti_create(quest_type_t type) {
    quest_rtti_t rtti;
    rtti.parts.type = type;
    rtti.parts.uid = quest_next_uid();
    return rtti;
}

quest_fingerprint_t quest_rtti_fingerprint(quest_rtti_t rtti) {
    return rtti.fingerprint;
}

quest_type_t quest_rtti_type(quest_rtti_t rtti) {
    return rtti.parts.type;
}

quest_size_t quest_rtti_uid(quest_rtti_t rtti) {
    return rtti.parts.uid;
}

bool quest_is_typeof(quest_rtti_t rtti, quest_type_t type) {
    return  type == rtti.parts.type;
}
