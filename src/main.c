#include <stdio.h>

#include "DOS/test_dos_services.h"
//#include "STR/test_str.h"
//#include "TDD/test_tdd.h"
//#include "FILE/test_file.h"

//#include "PARSE/test_parse.h"

RUN_TESTS(
    DOS_SERVICES_TESTS
    //STR_TESTS
    //TDD_TESTS
    //FILE_TESTS
)

int main(int argc, char** argv) {

#ifndef __LARGE__
    printf("Incorrect memory model is selected.\n");
    printf("Rebuild RETROLIB using the large memory model with the -ml compiler option.\n");
    return 0;
#endif

    return run_tests();

}
