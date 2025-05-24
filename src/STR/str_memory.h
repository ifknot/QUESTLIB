#ifndef STR_MEMORY_H
#define STR_MEMORY_H

#include "../MEM/mem_arena.h"
#include "str_types.h"

char* str_make_string(mem_arena_t* arena, str_size_t string_size);

char* str_make_copy(mem_arena_t* arena, const char* string);

char** str_make_string_array(mem_arena_t* mem_arena, str_size_t array_size, str_size_t string_size);

#endif
