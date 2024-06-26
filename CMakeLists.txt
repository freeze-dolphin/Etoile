cmake_minimum_required(VERSION 3.22)
project(Etoile C)

option(DEBUG "Source debug mode" ON)
option(TESTING "Testing switch" ON)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DEBUG OFF)
    set(TESTING OFF)
endif ()

if (DEBUG)
    add_definitions(-DTEST_DEBUG)
endif ()

#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-static")

unset(DEBUG CACHE)
unset(TESTING CACHE)

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
        GIT_REPOSITORY https://github.com/kuba--/zip
        GIT_TAG 550905d883b29f0b23e433fdb97f6299b628d4a9)
FetchContent_MakeAvailable(zip)

## libcyaml
pkg_check_modules(cyaml REQUIRED IMPORTED_TARGET libcyaml)

## cJSON
pkg_check_modules(cjson REQUIRED IMPORTED_TARGET libcjson)

## MagickWand
pkg_check_modules(MagickWand REQUIRED IMPORTED_TARGET MagickWand)

## lua
pkg_check_modules(lua REQUIRED IMPORTED_TARGET lua-5.4)

# define exec
add_executable(Etoile
        src/main.c
        src/args.h
        src/args.c
        src/utils.h
        src/utils.c
        src/schem_index.h
        src/schem_proj.h
        src/version.h)

# link lib
target_link_libraries(Etoile m) # Math
target_link_libraries(Etoile cargs)
target_link_libraries(Etoile zip)
target_link_libraries(Etoile cjson)
target_link_libraries(Etoile cyaml)
target_link_libraries(Etoile ${MagickWand_LIBRARIES})
target_include_directories(Etoile PUBLIC ${MagickWand_INCLUDE_DIRS})
target_compile_options(Etoile PUBLIC ${MagickWand_CFLAGS_OTHER})
target_link_libraries(Etoile ${lua_LIBRARIES})
target_include_directories(Etoile PUBLIC ${lua_INCLUDE_DIRS})
target_compile_options(Etoile PUBLIC ${lua_CFLAGS_OTHER})

if (TESTING)
    include(CTest)

    add_library(utils src/utils.c)
    target_link_libraries(utils m)
    target_link_libraries(utils cargs)
    target_link_libraries(utils zip)
    target_link_libraries(utils cjson)
    target_link_libraries(utils cyaml)
    target_link_libraries(utils ${lua_LIBRARIES})
    target_include_directories(utils PUBLIC ${lua_INCLUDE_DIRS})
    target_compile_options(utils PUBLIC ${lua_CFLAGS_OTHER})

    add_executable(test_utils test/test_utils.c)
    target_link_libraries(test_utils utils)

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

endif ()
