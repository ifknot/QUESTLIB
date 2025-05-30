#include <stdio.h>


//#include "STR/test_str.h"
//#include "PARSE/test_parse.h"
//#include "TDD/test_tdd.h"
#include "FILE/test_file.h"

RUN_TESTS(
    //STR_TESTS
    //TDD_TESTS
    FILE_TESTS
)

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

    return run_tests();

}
