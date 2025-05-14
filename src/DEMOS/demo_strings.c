#include "demo_strings.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../STR/str_scrubbing.h"
#include "../STR/str_processing.h"
#include "../STR/str_tokens.h"

int demo_strings(int argc, char** argv) {
    char s[] = "  \n\tKill,  the\n   C3PO!     with   \ta    axe ! ?       ";
    char* list[] = {"1apple","2apple","3apple","4apple","5apple","6apple", 0};
    str_word_token_t dict[] = {{"",0},{"LOOK", TOK_LOOK}, {"L", TOK_LOOK}, {"EXAMINE", TOK_EXAMINE}, {"X", TOK_EXAMINE}};
    str_token_dictionary_t d;
    d.first = dict[0];
    d.last = dict[4]; 
    int i = 0; while(list[i]) printf("%s\n", list[i++]);

    printf("DEMO STRINGS...\n");
    printf("%s=%i\n", s, strlen(s));
    printf("%s=%i\n", s, str_remove_punctuation(s));
    printf("%s=%i\n", s, str_trim_whitespace(s));
    printf("%s=%i\n", s, str_count_words(s));
    printf("%s=%i\n", s, str_enumarate_words(s, list));
    i = 0; while(list[i]) printf("%s\n", list[i++]);
    printf("%s=%i",dict[0].word, dict[0].token);

    return EXIT_SUCCESS;
}
