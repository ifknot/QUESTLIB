#include "quest_info.h"

quest_info_t* quest_info_link(mem_arena_t* arena, char* string_view_brief, char* string_view_details) {
    quest_info_t* info = (quest_info_t*)mem_arena_calloc(arena, sizeof(quest_info_t));
    assert(info);
    info->brief = string_view_brief;
    info->details = string_view_details;
    return info;
}

quest_info_t* quest_info_create(mem_arena_t* arena, str_size_t size_brief, str_size_t size_deatils) {
    quest_info_t* info = (quest_info_t*)mem_arena_calloc(arena, sizeof(quest_info_t));
    assert(info);
    info->brief = str_make_string(arena, size_brief);
    info->details = str_make_string(arena, size_brief);
    return info;
}

quest_info_t* quest_info_copy(mem_arena_t* arena, const char* string_brief, const char* string_details) {
    quest_info_t* info = (quest_info_t*)mem_arena_calloc(arena, sizeof(quest_info_t));
    assert(info);
    info->brief = str_make_copy(arena, string_brief);
    info->details = str_make_copy(arena, string_details);
    return info;
}
