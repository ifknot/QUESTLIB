#include "demo_strings.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../STR/str_scrubbing.h"

int demo_strings(int argc, char** argv) {
    char s[] = "  \n\tKill  the\n   Troll     with   \tthe    axe        ";
    printf("DEMO STRINGS...\n");
    printf("%s=%i\n", s, strlen(s));
    str_trim_whitespace(s);
    printf("%s=%i\n", s, strlen(s));
    return EXIT_SUCCESS;
}
