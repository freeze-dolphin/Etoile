#ifndef ETOILE_ARGS_H
#define ETOILE_ARGS_H

#include "version.h"

#include <cargs.h>
#include <stdlib.h>

void print_help_msg(bool prn_version);

static struct cag_option options[] = {
        {.identifier = 'i',
                .access_letters = "i",
                .access_name = "input",
                .value_name = "ARCPKG",
                .description = "Path to .arcpkg file\t<REQUIRED>"},

        {.identifier = 'o',
                .access_letters = "o",
                .access_name = "songs",
                .value_name = "SONGS",
                .description = "Path to songs dir\t<REQUIRED>"},

        {.identifier = 'b',
                .access_letters = "b",
                .access_name = "bg",
                .value_name = "BG",
                .description = "Path to img/bg dir\t<REQUIRED>"},

        {.identifier = 'p',
                .access_letters = "p",
                .access_name = "pack",
                .value_name = "PACK",
                .description = "Name of target pack\t<REQUIRED>"},

        {.identifier = 'l',
                .access_letters = "l",
                .access_name = "lua",
                .value_name = "LUA",
                .description = "Path to lua script to be applied on aff"},

        {.identifier = 'v',
                .access_letters = "v",
                .access_name = "version",
                .value_name = "VER",
                .description = "The version when this package was added (default: 1.0)"},

        {.identifier = 'f',
                .access_letters = NULL,
                .access_name = "force",
                .description = "Force to overwrite existing songlist"},


        {.identifier = 'F',
                .access_letters = NULL,
                .access_name = "fix-constant",
                .description = "Auto fix constant below 1.0"},

        {.identifier = 'h',
                .access_letters = "h",
                .access_name = "help",
                .description = "Shows the help and versions"},

        {.identifier = 'd',
                .access_letters = NULL,
                .access_name = "debug",
                .description = ""}};

#endif //ETOILE_ARGS_H
