#include "args.h"
#include <cargs.h>
#include <cyaml/cyaml.h>
#include <cjson/cJSON.h>
#include <lua5.4/lua.h>

void print_help_msg(bool prn_version) {
    char *proj_ver = (char *) calloc(sizeof(char), 32);
    sprintf(proj_ver, "%d.%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);

    char *lua_ver = (char *) calloc(sizeof(char), 32);
    sprintf(lua_ver, "%s.%s", LUA_VERSION_MAJOR, LUA_VERSION_MINOR);

    const char *ver_msg[] = {"Versions:\nÉtoile\t\t", proj_ver,
                             "\nlibcyaml\t", cyaml_version_str,
                             "\ncJSON\t\t", cJSON_Version(),
                             "\nLua\t\t", lua_ver,
                             "\n"};
    const char *usage_msg[] = {"\nUsage:\n",
                               "Etoile [-options] <-i path-to-arcpkg-file> <-o songs-dir> <-b bg-dir> <-p pack-name>\n\n"};

    if (prn_version) {
        int length, i;
        // length = *(&help_msg + 1) - help_msg;
        for (length = sizeof(ver_msg) / sizeof(ver_msg[0]), i = 0; i < length; i++) {
            printf("%s", ver_msg[i]);
        }
    }

    int length, i;
    // length = *(&help_msg + 1) - help_msg;
    for (length = sizeof(usage_msg) / sizeof(usage_msg[0]), i = 0; i < length; i++) {
        printf("%s", usage_msg[i]);
    }
    cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
}