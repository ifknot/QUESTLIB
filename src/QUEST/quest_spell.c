#include "quest_spell.h"

#include "quest_objects.h"

void  quest_spell_init(
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex
) {
    quest_component_init(&spell->base, parent, type, info);
    spell->hex = hex;
}

quest_spell_t* quest_spell_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex
) {
    assert(arena && "NULL memory arena!");

    quest_spell_t* spell = mem_arena_calloc(arena, sizeof(quest_spell_t));
    quest_spell_init(spell, parent, type, info, hex);
    return spell;
}

bool quest_is_spellbound(quest_composite_t* comp) {
    return quest_composite_count_type(comp, QUEST_SPELL);
}

bool quest_spell_cast(quest_composite_t* comp, quest_spell_t* spell) {
    if(!quest_is_spellbound(comp)) {
        quest_composite_add(comp, (quest_component_t*)spell);
        return true;
    }
    return false;
}
