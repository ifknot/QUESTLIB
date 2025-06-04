#include <stdio.h>
#include <stdlib.h>

mem_arena_t* quest_arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);

parse_dictionary_t* global_dictionary parse_dictionary_create_from_file(quest_arena , "TOWER.DIC");

location_dicts[] load_location_dicts("LOCALS.DIC");

location_info[] load_location_strings("LOCALS.INF");

quest_create(&quest, "Tower Quest", &global_dictionary);

quest_location_t* start = quest_add_location(&quest, location_info[0], container, location_dicts[0])

int main(int argc, char** argv) {

    return EXIT_SUCCESS;

}
