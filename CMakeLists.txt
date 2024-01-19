cmake_minimum_required(VERSION 3.25)
project(Etoile C)

option(cc_win "Cross compile for Windows" OFF)
if (cc_win STREQUAL "1")
    message("Environment variable `CROSS_COMPILE_WINDOWS` defined, cross compiling for Windows...")
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
endif ()

set(CMAKE_C_STANDARD 11)

file(STRINGS "src/version.h" VERSION_CONTENT REGEX "#define PROJECT_VERSION_.*")

string(REGEX MATCH "#define PROJECT_VERSION_MAJOR ([0-9]*)" _ ${VERSION_CONTENT})
set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define PROJECT_VERSION_MINOR ([0-9]*)" _ ${VERSION_CONTENT})
set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "#define PROJECT_VERSION_PATCH ([0-9]*)" _ ${VERSION_CONTENT})
set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_1})

# dependencies

include(FetchContent)
find_package(PkgConfig REQUIRED)

## cargs
FetchContent_Declare(cargs
        GIT_REPOSITORY https://github.com/likle/cargs.git
        GIT_TAG stable)
FetchContent_MakeAvailable(cargs)

## zip
FetchContent_Declare(zip
        GIT_REPOSITORY https://github.com/freeze-dolphin/zip
        GIT_TAG b62ca4289301fb65e2cfcd389e2bb298ac0b6d6d)
FetchContent_MakeAvailable(zip)

## libcyaml
pkg_check_modules(cyaml REQUIRED IMPORTED_TARGET libcyaml)

## cJSON
pkg_check_modules(cjson REQUIRED IMPORTED_TARGET libcjson)

## MagickWand
pkg_check_modules(MAGICKWAND REQUIRED IMPORTED_TARGET MagickWand)

# define exec
add_executable(Etoile
        src/main.c
        src/args.h
        src/args.c
        src/utils.h
        src/utils.c
        src/schem_index.h
        src/schem_proj.h)

# link lib
target_link_libraries(Etoile m)
target_link_libraries(Etoile cargs)
target_link_libraries(Etoile zip)
target_link_libraries(Etoile cjson)
target_link_libraries(Etoile cyaml)
target_link_libraries(Etoile ${MAGICKWAND_LIBRARIES})
target_include_directories(Etoile PUBLIC ${MAGICKWAND_INCLUDE_DIRS})
target_compile_options(Etoile PUBLIC ${MAGICKWAND_CFLAGS_OTHER})

# playground

add_executable(playground eva/playground.c)
target_link_libraries(playground cyaml)

# testing

include(CTest)

add_library(utils src/utils.c)
target_link_libraries(utils m)

add_executable(test_utils test/test_utils.c)
target_link_libraries(test_utils utils)
target_link_libraries(test_utils cargs)
target_link_libraries(test_utils zip)
target_link_libraries(test_utils cjson)
target_link_libraries(test_utils cyaml)

add_test(
        NAME "utils - is_contained_in_file"
        COMMAND $<TARGET_FILE:test_utils> "is_contained_in_file")
add_test(
        NAME "utils - trim"
        COMMAND $<TARGET_FILE:test_utils> "trim")
add_test(
        NAME "utils - to_snake_case"
        COMMAND $<TARGET_FILE:test_utils> "to_snake_case")
add_test(
        NAME "utils - is_file_exist"
        COMMAND $<TARGET_FILE:test_utils> "is_file_exist")
add_test(
        NAME "utils - is_endwith"
        COMMAND $<TARGET_FILE:test_utils> "is_endwith")
add_test(
        NAME "utils - progress_formatter"
        COMMAND $<TARGET_FILE:test_utils> "progress_formatter")
