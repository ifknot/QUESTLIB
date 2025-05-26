#ifndef TEST_PARSE_H
#define TEST_PARSE_H

#include <stdio.h>

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"

#include "parse_dictionary.h"
#include "parse_types.h"

#define PARSE_TESTS &test_parse_dictionary

TEST(test_parse_dictionary) {
    tdd_verbose = true;
    mem_arena_t* arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);
    parse_dictionary_t* d = parse_dictionary_create(arena, 10);
    ASSERT(parse_dictionary_size(d) == 0);
    ASSERT(parse_dictionary_capacity(d) == 10);
    parse_dictionary_add(d, "incomprehensibilities", 12); // will be truncated
    ASSERT(parse_dictionary_size(d) == 1);
    parse_dictionary_add(d, "apple", 12);
    ASSERT(parse_dictionary_size(d) == 2);
    parse_dictionary_sort(d);
    V(parse_dictionary_dump(stdout, d););
    parse_dictionary_remove(d, 0);
    ASSERT(parse_dictionary_size(d) == 1);
    V(parse_dictionary_dump(stdout, d););

    mem_arena_delete(arena);
}

#endif
