#include <stdlib.h>

#include "test_main.h"

int main(int argc, char** argv) {

    return (run_tests()) ? EXIT_FAILURE : EXIT_SUCCESS;

}


/* how it might look...

mem_arena_t* quest_arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);

parse_dictionary_t* global_dictionary parse_dictionary_create_from_file(quest_arena , "TOWER.DIC"); // JSON token word pairs

location_dicts[] load_location_dicts("LOCALS.DIC");       // JSON file location number token word pairs

location_info[] load_location_info("LOCALS.INF");         // JSON file descriptions,

quest_create(&quest, "Tower Quest", &global_dictionary);

quest_location_t* start = quest_add_location(&quest, location_info[0], container, location_dicts[0])
*/
