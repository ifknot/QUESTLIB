#include "quest_composite.h"

#include <assert.h>
#include <stdlib.h>

#include "quest_info.h"
#include "quest_rtti.h"
#include "quest_types.h"

#include <stdio.h>

void quest_component_init(quest_component_t* comp, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(comp && "NULL component!");
    assert(info && "NULL string information!");

    comp->rtti = quest_rtti_create(type);
    comp->parent = parent;
    comp->info = info;
}

quest_component_t* quest_component_create(mem_arena_t* arena, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(arena && "NULL memory arena!");

    quest_component_t* comp = (quest_component_t*)mem_arena_calloc(arena, sizeof(quest_component_t));
    assert(comp && "NULL component - arena allocation failed!");

    quest_component_init(comp, parent, type, info);

    return comp;
}

void quest_composite_init(quest_composite_t* comp, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(comp && "NULL composite!");
    assert(info && "NULL string information!");

    quest_component_init(&comp->base, parent, type, info);

    comp->child_count = 0;
    for (int i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        comp->children[i] = NULL;
    }
}

quest_composite_t* quest_composite_create(mem_arena_t* arena, quest_component_t* parent, quest_type_t type, quest_info_t* info) {
    assert(arena && "NULL memory arena!");

    quest_composite_t* comp = (quest_composite_t*)mem_arena_calloc(arena, sizeof(quest_composite_t));
    assert(comp && "NULL component - arena allocation failed!");

    quest_composite_init(comp, parent, type, info);

    return comp;
}

void quest_composite_add(quest_composite_t* parent, quest_component_t* child) {
    assert(parent && "NULL parent!");
    assert(child && "NULL child!");

    assert(parent->child_count < QUEST_COMPOSITE_MAX_CHILDREN && "FULL children array!");
    parent->children[parent->child_count++] = child;
    child->parent = (quest_component_t*)parent;
}

quest_component_t* quest_composite_remove(quest_composite_t* parent, quest_component_t* child) {
    assert(parent && "NULL parent!");
    assert(child && "NULL child!");
    assert(parent->child_count > 0 && "EMPTY children array!");

    for (quest_size_t i = 0; i < parent->child_count; ++i) {    // linear seach
        if (parent->children[i] == child) {
            parent->children[i] = parent->children[--parent->child_count];  // over-write with last element
            parent->children[parent->child_count] = NULL; // no hidden dangling pointers
            child->parent = NULL;
            return child;
        }
    }
    return NULL;
}

quest_component_t* quest_composite_find(quest_composite_t* parent, quest_fingerprint_t fingerprint) {
    assert(parent && "NULL parent!");
    assert(parent->child_count > 0 && "EMPTY children array!");

    for (quest_size_t i = 0; i < parent->child_count; ++i) {
        if (parent->children[i]->rtti.fingerprint == fingerprint) {
            return parent->children[i];
        }
    }
    return NULL;
}

quest_size_t quest_composite_transfer_all(quest_composite_t* dst, quest_composite_t* src) {
    assert(src && "NULL source!");
    assert(dst && "NULL destination!");
    assert(src != dst && "SAME source and destination!");
    assert(src->child_count > 0 && "EMPTY source!");
    assert(dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN && "FULL destination!");

    quest_size_t transfer_count = 0;
    while(src->child_count > 0 && dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN) { // Process from the end of src to beginning for safer removal

        quest_composite_add(dst, quest_composite_remove(src, src->children[src->child_count - 1])); // Add to destination - handles parent pointer and count

        transfer_count++;
    }

    return transfer_count;
}

quest_size_t quest_composite_transfer_type(quest_composite_t* dst, quest_composite_t* src, quest_type_t target_type) {
    assert(src && "NULL source!");
    assert(dst && "NULL destination!");
    assert(src != dst && "SAME source and destination!");
    assert(src->child_count > 0 && "EMPTY source!");
    assert(dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN && "FULL destination!");

    quest_size_t transfer_count = 0;
    quest_size_t src_index = src->child_count;

    while(src_index > 0 && dst->child_count < QUEST_COMPOSITE_MAX_CHILDREN) {
        src_index--; // Start from last element
        quest_component_t* child = src->children[src_index];
        if(child->rtti.parts.type == target_type) {
            src->children[src_index] = src->children[--src->child_count]; // Remove from source first (swapping not needed when processing back-to-front)
            src->children[src->child_count] = NULL;
            quest_composite_add(dst, child); // Add to destination
            transfer_count++;
        }
    }

    return transfer_count;
}

void quest_component_dump(const quest_component_t* comp, FILE* stream) {
    assert(comp && "NULL component!");
    assert(stream && "NULL file stream!");

    quest_rtti_dump(comp->rtti, stream);
    if (comp->parent) {
        fprintf(stream, "|parent:%p", comp->parent);
        quest_rtti_dump(comp->parent->rtti, stream);
    } else {
        fprintf(stream, "|parent:NULL");
    }
}

void quest_composite_dump(const quest_composite_t* comp, FILE* stream) {
    assert(comp && "NULL component!");
    assert(stream && "NULL file stream!");

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
