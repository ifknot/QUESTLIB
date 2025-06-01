#include <stdio.h>

/*
#include "TDD/test_tdd.h"
#include "DOS/test_dos_services.h"
#include "STR/test_str.h"
#include "FILE/test_file.h"
#include "MEM/test_mem_tools.h"
#include "MEM/test_mem_arena.h"
*/
#include "PARSE/test_parse_dictionary.h"

RUN_TESTS( /*TDD_TESTS,
    DOS_SERVICES_TESTS,
    FILE_TESTS,
    STR_TESTS,
    TOOLS_TESTS,
    ARENA_TESTS*/
    DICTIONARY_TESTS
)

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

    return run_tests();

}
