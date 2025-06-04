#ifndef QUEST_COMPOSITE_T
#define QUEST_COMPOSITE_T

// common base approach to part whole hierachy of in game objects and RTTI

#include "quest_constants.h"
#include "quest_object_types.h"
#include "quest_rtti.h"

typedef struct quest_component_s quest_component_t;

typedef struct quest_component_s {
    quest_component_t* parent;
    quest_rtti_t type;
    quest_info_t info;
} quest_component_t ;

typedef struct {
    quest_component_t base;
} quest_leaf_t;

typedef struct {
    quest_component_t base;
    quest_component_t* children[QUEST_MAX_LEAF];    // a bounded null terminated list of children
    quest_size_t size; // number of children
} quest_composite_t;

#endif
