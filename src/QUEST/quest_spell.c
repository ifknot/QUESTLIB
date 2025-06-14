#include "quest_spell.h"

void  quest_spell_init(
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex,
    quest_size_t strength
) {
    quest_component_init(&spell->base, parent, type, info);
    spell->hex = hex;
    spell->strength = strength;
}

quest_spell_t* quest_spell_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t code,
    quest_size_t strength
) {
    return 0;
}

bool quest_component_is_spellbound(quest_component_t* comp) {
    return false;
}

quest_spell_t* quest_spell_cast(quest_component_t* comp, quest_spell_t* spell) {
    return 0;
}
