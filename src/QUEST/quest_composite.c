#include "quest_composite.h"

#include <assert.h>
#include <stdlib.h>

#include "quest_errors.h"
#include "quest_info.h"
#include "quest_rtti.h"
#include "quest_types.h"

#include <stdio.h>

quest_error_t quest_component_init(quest_composite_t* comp, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(comp);
    comp->base.rtti = quest_rtti_create(type);
    comp->base.parent = parent;
    comp->base.info = info;
    return QUEST_SUCCESS;
}

quest_component_t* quest_component_create(mem_arena_t* arena, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(arena);
    quest_component_t* comp = (quest_component_t*)mem_arena_calloc(arena, sizeof(quest_component_t));
    assert(comp);
    comp->rtti = quest_rtti_create(type);
    comp->parent = parent;
    comp->info = info;
    return comp;
}

quest_error_t quest_composite_init(quest_composite_t* comp, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(comp);
    comp->base.rtti = quest_rtti_create(type);
    comp->base.parent = parent;
    comp->base.info = info;

    comp->child_count = 0;
    for (int i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        comp->children[i] = NULL;
    }
    return QUEST_SUCCESS;
}

quest_composite_t* quest_composite_create(mem_arena_t* arena, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(arena);
    quest_composite_t* comp = (quest_composite_t*)mem_arena_calloc(arena, sizeof(quest_composite_t));
    assert(comp);
    quest_composite_init(comp, parent, type, info);
    return comp;
}

quest_error_t quest_composite_add(quest_composite_t* parent, quest_component_t* child) {
    assert(parent && child);
    if (parent->child_count == QUEST_COMPOSITE_MAX_CHILDREN) {
        return QUEST_ITEM_LIST_FULL;
    }
    parent->children[parent->child_count++] = child;
    child->parent = (quest_component_t*)parent;
    return QUEST_SUCCESS;
}

quest_error_t quest_composite_remove(quest_composite_t* parent, quest_component_t* child) {
    assert(parent && parent->child_count);
    for (quest_size_t i = 0; i < parent->child_count; ++i) {    // linear seach
        if (parent->children[i] == child) {
            parent->children[i] = parent->children[--parent->child_count];  // over-write with last element
            parent->children[parent->child_count] = NULL; // no hidden dangling pointers
            child->parent = NULL;
            return QUEST_SUCCESS;
        }
    }
    return QUEST_ITEM_NOT_FOUND;
}

quest_component_t* quest_composite_find(quest_composite_t* parent, quest_fingerprint_t fingerprint) {
    assert(parent && parent->child_count);
    for (quest_size_t i = 0; i < parent->child_count; ++i) {
        if (parent->children[i]->rtti.fingerprint == fingerprint) {
            return parent->children[i];
        }
    }
    return 0;
}

quest_error_t quest_composite_transfer_all(quest_composite_t* dst, quest_composite_t* src) {
    assert(src && dst && src != dst);
    if(src->child_count == 0) {
        return QUEST_ITEM_LIST_EMPTY;
    }
    if(dst->child_count == QUEST_COMPOSITE_MAX_CHILDREN) {
        return QUEST_ITEM_LIST_FULL;
    }
    quest_size_t i = dst->child_count;
    quest_size_t j = 0;
    while(src->child_count && dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN) {
        dst->children[i] = src->children[j];
        src->children[j++] = NULL; // no hidden dangling pointers
        dst->children[i++]->parent = (quest_component_t*)dst;
        src->child_count--;
        dst->child_count++;
    }
    return (src->child_count == 0) ? QUEST_SUCCESS : QUEST_ITEM_LIST_PARTIAL;
}

quest_error_t quest_composite_transfer_type(quest_composite_t* dst, quest_composite_t* src, quest_type_t target_type) {
    assert(src && dst && src != dst);
    if(src->child_count == 0) {
        return QUEST_ITEM_LIST_EMPTY;
    }
    if(dst->child_count == QUEST_COMPOSITE_MAX_CHILDREN) {
        return QUEST_ITEM_LIST_FULL;
    }
    quest_size_t i = dst->child_count;  // destination index
    quest_size_t j = src->child_count;                 // source index
    const quest_size_t src_initial_count = src->child_count;
    while(dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN) {
        if(src->children[j]->rtti.parts.type == target_type) {
            dst->children[i] = src->children[j];
            dst->children[i++]->parent = (quest_component_t*)dst;
            src->children[j] = src->children[--src->child_count];  // over write w last
            src->children[src->child_count] = NULL; // no hidden dangling pointers
            dst->child_count++;
        }
        if(j == 0) {
            break;
        }
        j--;
    }
    return (src->child_count == src_initial_count) ? QUEST_FAILURE : QUEST_SUCCESS;
}

void quest_component_dump(const quest_component_t* comp, FILE* stream) {
    assert(comp && stream);
    quest_rtti_dump(comp->rtti, stream);
    fprintf(stream, "|parent:"); // Dump parent reference (as fingerprint or NULL)
    if (comp->parent) {
        quest_rtti_dump(comp->parent->rtti, stream);
    } else {
        fprintf(stream, "NULL");
    }
}

void quest_composite_dump(const quest_composite_t* comp, FILE* stream) {
    assert(comp && stream);
    fprintf(stream, "COMPOSITE[children=%zu]:", comp->child_count); // Dump header
    quest_component_dump(&comp->base, stream);
    fprintf(stream, "\n  CHILDREN:\n"); // Dump children
    for (quest_size_t i = 0; i < comp->child_count; i++) {
        fprintf(stream, "    [%02zu] ", i);
        if (comp->children[i]) {
            quest_component_dump(comp->children[i], stream);
        } else {
            fprintf(stream, "NULL_SLOT");
        }
        fprintf(stream, "\n");
    }
}
