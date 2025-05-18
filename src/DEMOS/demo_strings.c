#include "demo_strings.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../STR/str_scrubbing.h"
#include "../STR/str_processing.h"
#include "../STR/str_file.h"

int demo_strings(int argc, char** argv) {
    char filepath[63];
    char word[63];
    str_size_t sz;
    char s[] = "  \n\tKill,  the\n   C3PO!     with   \ta    axe ! ?       ";
    char* list[] = {"1apple","2apple","3apple","4apple","5apple","6apple", 0};
    int i = 0; while(list[i]) printf("%s\n", list[i++]);

    printf("DEMO STRINGS...\n");
    printf("%s=%i\n", s, strlen(s));
    printf("%s=%i\n", s, str_remove_punctuation(s));
    printf("%s=%i\n", s, str_trim_whitespace(s));
    printf("%s=%i\n", s, str_count_words(s));
    printf("%s=%i\n", s, str_enumarate_words(s, list));
    i = 0; while(list[i]) printf("%s\n", list[i++]);

    printf("Enter the file name: ");
    scanf("%s", filepath);
    dos_file_handle_t f = dos_open_file(filepath, ACCESS_READ_ONLY);
    //printf("counting...%i\n",f);
    //printf("%s=%i lines\n", filepath, str_file_count_lines(f));
    //printf("counting...%i\n",f);
    //printf("%s=%i words\n", filepath, str_file_count_words(f));
    sz = str_file_read_word(f, word);
    printf("%s=%li", word, sz);
    dos_close_file(f);
    return EXIT_SUCCESS;
}
