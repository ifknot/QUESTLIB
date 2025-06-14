#ifndef QUEST_SPELL_H
#define QUEST_SPELL_H

#include "quest_composite.h"

/**
 * @brief Any component in a quest world can have spell placed upon it.
 * @details There are 4 nybbles available in the quest_size_t to represent
 * the quest world's 4 families of spells:
 * earth, wind, water, fire
 *
 * 0 is no spell in this family
 * leaving 15 spells for each family a total of 60 in game spells for quest_spell_t
 * to undo the spell an undo spell with 0xF in the corrrect nybble for the spell family
 * 0x000F all fire undo
 * 0x00F0 all water undo
 * 0x0F00 all wind undo
 * 0xF000 all earth undo
 *
 * spells can be combined, some will nullify others will amplify
 * eg combining 4 of each undo spell will giver 0xFFFF the universal spell undo
 * eg combining earth and fire such as earth root and fire eternal will root an object to the spot and set it alight!
 * eg combining water and wind such as water drench and wind artcic will freeze and object but make it slide
 * eg combining water and fire such as drench and eternal will nullify the spell
 */
typedef struct quest_spell_t {
    quest_component_t base;
    quest_size_t hex;     // 14 spells each 4 categories earth, water, fire, death
} quest_spell_t;

void  quest_spell_init(
    quest_spell_t* spell,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex
);

quest_spell_t* quest_spell_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t hex
);

bool quest_component_is_spellbound(quest_composite_t* comp);

/**
 * @brief any composite can have a spell cast upon it...
 *
 * ...unless it is already spellbound
 * @return true if cast or false if not
 */
bool quest_spell_cast(quest_composite_t* comp, quest_spell_t* spell);

#endif
