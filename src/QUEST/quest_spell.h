#ifndef QUEST_SPELL_H
#define QUEST_SPELL_H

#include "quest_composite.h"

/**
 * @brief Any component in a quest world can have spell placed upon it.
 * @details There are 4 nybbles available in the quest_size_t to represent
 * the quest world's 4 families of spells:
 * earth, wind, water, fire
 * 0 is no spell in this family
 * leaving 15 spells for each family a total of 60 in game spells for quest_spell_t
 * @note only 1 nybble can be used otherwise the spell is corrupted
 * to undo the spell an undo spell can be applied where the codes must match or...
 * be 0xF in the corrrect nybble for the spell family as the universal undo spell
 * 0x000F all fire undo
 * 0x00F0 all water undo
 * 0x0F00 all wind undo
 * 0xF000 all earth undo
 * 0xFFFF universal all spells undo
 */
typedef struct quest_spell_t {
    quest_component_t base;
    quest_size_t hex;     // 14 spells each 4 categories earth, water, fire, death
    quest_size_t strength; // is a function of the spell caster's magical power and character type
} quest_spell_t;

void  quest_spell_init(
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex,
    quest_size_t strength
);

quest_spell_t* quest_spell_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex,
    quest_size_t strength
);

bool quest_component_is_spellbound(quest_component_t* comp);

/**
 * @brief any item in a quest world can have a spell cast upon it, unless:
 * 1. component already spellbound with a stronger spell
 * 2. component has a no spell cast upon it
 * If a stronger spell is cast it replaces the weaker spell which is returned
 * If there was no prexisting spell then NULL is returned.
 */
quest_spell_t* quest_spell_cast(quest_component_t* comp, quest_spell_t* spell);


#endif
