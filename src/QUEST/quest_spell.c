#include "quest_spell.h"

void  quest_spell_init(
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t code,
    quest_size_t strength
) {
    quest_component_init(&spell->base, parent, type, info);
    spell->code = code;
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
  
}

void quest_undo_spell_init() {
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_spell_t* source 
}

quest_undo_spell_t* quest_undo_spell_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_spell_t* source 
);

bool quest_component_is_spellbound(quest_component_t* comp);

quest_spell_t* quest_spell_cast(quest_component_t* comp, quest_spell_t* spell);


quest_spell_t* quest_spell_undo(quest_component_t* comp, quest_undo_spell_t* undo_spell);

