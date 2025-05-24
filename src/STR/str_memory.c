#include "str_memory.h"

#include <string.h>
#include <assert.h>

char* str_make_string(mem_arena_t* arena, str_size_t string_size) {
    char* string = mem_arena_alloc(arena, string_size);
    assert(string);
    string[0] = 0; // null string
    return string;
}

char* str_make_copy(mem_arena_t* arena, const char* source) {
    char* string = str_make_string(arena, strlen(source));
    return strcpy(string, source);
}

char** str_make_string_array(mem_arena_t* arena, str_size_t array_size, str_size_t string_size) {
    char** string_array = mem_arena_alloc(arena, array_size * sizeof(char*));
    for(int i = 0; i < array_size; ++i) {
       string_array[i] = str_make_string(arena, string_size);
    }
    return string_array;
}
