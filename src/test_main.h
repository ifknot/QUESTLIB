#ifndef TEST_MAIN_H
#define TEST_MAIN_H


//#include "TDD/test_tdd_framework.h"
//#include "STAT/test_stats.h"
//#include "DOS/test_dos_services.h"
//#include "STR/test_str.h"
//#include "FILE/test_file.h"
//#include "MEM/test_mem_tools.h"
//#include "MEM/test_mem_arena.h"
//#include "PARSE/test_parse_dictionary.h"
//#include "QUEST/test_quest_rtti_uid.h"
//#include "QUEST/test_quest_composite.h"
//#include "QUEST/test_quest_connector.h"
//#include "QUEST/test_quest_location.h"
#include "QUEST/test_quest_prng.h"

RUN_TESTS(
    //TDD_FRAMEWORK_TESTS,
    //STATS_TEST_SUITE
    //DOS_SERVICES_TESTS,
    //FILE_TESTS,
    //STR_TESTS,
    //TOOLS_TESTS,
    //ARENA_TESTS*
    //DICTIONARY_TESTS
    //QUEST_RTTI_TESTS,
    //QUEST_COMPOSITE_TESTS
    //QUEST_CONNECTOR_TESTS
    //QUEST_LOCATION_TESTS
    QUEST_PRNG_TESTS
)

#endif

/**
 * Usage:
 * @code
 *
 * int main(int argc, char** argv) {
 *
 * #ifndef __LARGE__
 *    printf("Incorrect memory model is selected.\n");
 *    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
 *    return 0;
 * #endif
 *
 *   return run_tests();
 *
 * }
 */
