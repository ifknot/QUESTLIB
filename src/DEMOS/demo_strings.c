#include "demo_strings.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../STR/str_scrubbing.h"
#include "../STR/str_processing.h"

int demo_strings(int argc, char** argv) {
    char s[] = "  \n\tKill,  the\n   C3PO!     with   \ta    axe ! ?       ";
    printf("DEMO STRINGS...\n");
    printf("%s=%i\n", s, strlen(s));
    printf("%s=%i\n", s, str_remove_punctuation(s));
    printf("%s=%i\n", s, str_trim_whitespace(s));
    printf("%s=%i\n", s, str_count_words(s));
    printf("%s=%i\n", s, str_enumarate_words(s, 0));

    return EXIT_SUCCESS;
}
