#include "../src/utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BEGIN_TEST if (0) {
#define TEST(b) } else if (strcmp(argv[1], b) == 0) {
#define END_TEST }
#define eql(a, b) strcmp(a, b) == 0


/*
 * unit test for `src/utils.c`
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error argc\n");
        return EXIT_FAILURE;
    }

    BEGIN_TEST

    TEST("is_contained_in_file")
        {
            char *filename = "../README.md";
            char *target = "# Étoile";

            int result = is_contained_in_file(filename, target);
            if (result == -1) {
                fprintf(stderr, "Cannot open file %s\n", filename);
                return EXIT_FAILURE;
            }
            assert(result);
        }

    TEST("trim")
        {
            char str[100] = "            \"id\": \"arghena\",         ";
            trim(str);
            fprintf(stderr, "str: %s\n", str);
            assert(strcmp(str, "\"id\": \"arghena\",") == 0);
        }


    TEST("to_snake_case")
        {
            char str1[30] = "SimpleFolds";
            char str2[30] = "xenovcipher";
            char str3[30] = "Unlimited Hyperlink";

            to_snake_case(str1);
            to_snake_case(str2);
            to_snake_case(str3);

            assert(eql(str1, "simplefolds") &&
                   eql(str2, "xenovcipher") &&
                   eql(str3, "unlimited_hyperlink"));
        }


    TEST("is_file_exist")
        {
            assert(is_file_exist("../README.md"));
        }


    TEST("is_endwith")
        {
            char str1[20] = "a.arcpkg";
            char str2[20] = "a.arc1pkg";
            char str3[20] = "a.arcpkg ";
            char str4[20] = "a. arcpkg";

            char arcpkg[8] = ".arcpkg";

            assert(is_endwith(str1, arcpkg) &&
                   !is_endwith(str2, arcpkg) &&
                   !is_endwith(str3, arcpkg) &&
                   !is_endwith(str4, arcpkg));
        }

    TEST("progress_formatter")
        {
            char *fmt = progress_formatter(1145141919);
            printf("fmt=%s\n", fmt);
            assert(eql(fmt, "[%10d/%10d] Working on '%s' (%s/%s)\n"));
        }

    END_TEST

    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop
