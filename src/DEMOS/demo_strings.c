#include "demo_strings.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>

#include "../STR/str_scrubbing.h"

int demo_strings(int argc, char** argv) {
    char s[] = "  Kill  the Troll with the axe  ";
    printf("DEMO STRINGS...\n");
    printf("%s\n", s);
    printf("chars %i\n", str_trim_character(s, ' '));
    printf("%s\n", s);
    return EXIT_SUCCESS;
}
