#ifndef QUEST_SPELL_H
#define QUEST_SPELL_H

#include "quest_composite.h"

/**
 * @brief any component in a quest world can have spell placed upon it
 */
typedef struct {
    quest_component_t base;    
    quest_size_t spell_code; // 14 spells each 4 categories earth, water, fire, death 
    quest_size_t spell_strength; 
} quest_spell_t;

typedef struct {
    quest_component_t base;    
    quest_size_t spell_code; // can be over-riding eg 00F0 for all water spells
} quest_undo_spell_t;

void  quest_spell_init();

quest_spell_t* quest_spell_create();

void quest_undo_spell_init();

quest_undo_spell_t* quest_undo_spell_create( ... quest_spell_t source);

bool quest_component_is_spellbound();

/**
 * @brief any item in a quest world can have a spell cast upon it, unless:
 * 1. component already spellbound with a stronger spell
 * 2. component has a no spell cast upon it
 * If a stronger spell is cast it replaces the weaker spell which is returned
 * If there was no prexisting spell then NULL is returned.
 */
quest_spell_t* quest_spell_cast(quest_component_t* comp, quest_spell_t* spell);

/**
 * @brief in order to undo (remove) a spell a *matching* undo spell must be cast
 * after which the item is open to having another spell cast upon it.
 * returns NULL on success
 * returns the original undo spell if spell mismatch or not spellbound in the first place 
 */
quest_spell_t* quest_spell_undo(quest_component_t* comp, quest_undo_spell_t* undo_spell);

#endif
