#include "quest_composite.h"

#include <assert.h>
#include <stdlib.h>

#include "quest_errors.h"
#include "quest_rtti.h"
#include "quest_types.h"

quest_error_t quest_composite_add(quest_composite_t* parent, quest_component_t* child) {
    assert(parent && child);
    if (parent->child_count == QUEST_COMPOSITE_MAX_CHILDREN) {
        return QUEST_ITEM_LIST_FULL;
    }
    parent->children[parent->child_count++] = child;
    child->parent = (quest_component_t*)parent;
    return QUEST_SUCCESS;
}

quest_error_t quest_composite_remove(quest_composite_t* parent, quest_fingerprint_t fingerprint) {
    assert(parent && parent->child_count);
    for (quest_size_t i = 0; i < parent->child_count; ++i) {    // linear seach
        if (parent->children[i]->rtti.fingerprint == fingerprint) {
            parent->children[i] = parent->children[--parent->child_count];  // over-write with last element
            parent->children[parent->child_count] = NULL; // no hidden dangling pointers
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

quest_error_t quest_composite_transfer_all(quest_composite_t* src, quest_composite_t* dst) {
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

quest_error_t quest_composite_transfer_type(quest_composite_t* src, quest_composite_t* dst, quest_type_t target_type) {
    assert(src && dst && src != dst);
    if(src->child_count == 0) {
        return QUEST_ITEM_LIST_EMPTY;
    }
    if(dst->child_count == QUEST_COMPOSITE_MAX_CHILDREN) {
        return QUEST_ITEM_LIST_FULL;
    }
    quest_size_t i = dst->child_count;
    quest_size_t j = 0;
    const quest_size_t src_initial_count = src->child_count;
    while(src->child_count && dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN) {
        if(src->children[j]->rtti.fingerprint == target_type) {
            dst->children[i] = src->children[j];
            dst->children[i++]->parent = (quest_component_t*)dst;
            src->children[j] = src->children[--src->child_count];  // over write w last
            src->children[src->child_count] = NULL; // no hidden dangling pointers
            dst->child_count++;
        }
        j++;
    }
    return (src->child_count == src_initial_count) ? QUEST_FAILURE : QUEST_SUCCESS;
}
