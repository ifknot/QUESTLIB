#include "str_scrubbing.h"

#include <stdint.h>
//#include <stdio.h>
#include <stdbool.h>

uint16_t str_trim_character(char* zsource, char target) {
    uint16_t i = 0; // search index;
    uint16_t j = 0; // copy index;
    bool leading = (zsource[i++] == target) ? true : false;
    while(leading && zsource[i] == target && zsource[i++]) {}
    while (zsource[i]) {
        if(zsource[i] == target) {
            zsource[j++] = zsource[i++]; // allow 1 copy
            while(zsource[i] == target) { // skip any others
                i++;
            };
        }
        else { // let the rest through
            zsource[j++] = zsource[i++];
        }
    }
    j = (zsource[j - 1] == target) ? j - 1 : j; // check for trailing target
    zsource[j] = 0; // terminate the copied string
    return i - j;
}

uint16_t str_trim_characters(char* zsource, char* ztargets) {
    uint16_t i = 0; // source index
    while (ztargets[i]) { // convert all targets to target[0]
       uint16_t j = 0;
       while(zsource[j]) {
           if(zsource[j] == ztargets[i]) {
               zsource[j] = ztargets[0];
           }
           j++;
       }
       i++;
    };
    return str_trim_character(zsource, ztargets[0]);
}

uint16_t str_remove_characters(char* zsource, char* const zcharacters) {
    uint16_t trimmed = 0;

    return trimmed;
}
