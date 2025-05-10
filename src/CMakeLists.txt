cmake_minimum_required(VERSION 3.5)

#suppress compiler checks for cross compilation using OW2 and C under ARM environments
set(CMAKE_C_COMPILER_WORKS 1)

project(
    questlib
    VERSION 0.1.0
    LANGUAGES C
)

# watcom compiler options
# https://users.pja.edu.pl/~jms/qnx/help/watcom/compiler-tools/cpopts.html
add_compile_options(
    -za99               # undocumented switch enable partial C99 compatibility
    -ml                 # memory model options - large model
)

file(GLOB SOURCES
    *.c
    STR/*.c
)

message(Source list="${SOURCES}")

add_executable(quest ${SOURCES})
