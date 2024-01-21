#ifndef ETOILE_UTILS_H
#define ETOILE_UTILS_H

#include "schem_index.h"
#include "schem_proj.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <cyaml/cyaml.h>
#include <cjson/cJSON.h>
#include <zip.h>

#define DBG(str) if (flag_dbg) printf("[DEBUG] %s\n", str);

#define DBG_I(str, i) if (flag_dbg) printf("[DEBUG] %d: %s\n", i, str);

#define ERR_AND_EXIT(...) \
fprintf(stderr, __VA_ARGS__); \
fprintf(log_file, __VA_ARGS__); \
exit(EXIT_FAILURE);

#define STDERR_ONLY_AND_EXIT(...) \
fprintf(stderr, __VA_ARGS__); \
exit(EXIT_FAILURE);

#define VALIDATION_ALLOC(ptr) \
if (ptr == NULL) { \
    fprintf(stderr, "Failed to allocate memory\n"); \
    free(ptr); \
    exit(EXIT_FAILURE); \
}

#define VALIDATION_CJSON(ptr, obj) \
if (ptr == NULL) { \
    fprintf(stderr, "Failed to generate songlist\n"); \
    cJSON_Delete(obj); \
    exit(EXIT_FAILURE); \
}

#ifdef WIN32

#include <io.h>
#include <direct.h>

#define F_OK 0
#define access _access
#define mkdir _mkdir
#define MKDIR_ARGS(path) path

#endif

#ifdef linux

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MKDIR_ARGS(path) path, 0700

#endif

static const cyaml_config_t cyaml_cfg = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
        .flags = CYAML_CFG_IGNORE_UNKNOWN_KEYS,
};

bool make_dir(const char *path);

bool is_file_exist(char *fname);

int is_contained_in_file(char *filename, char *target);

void trim(char *str);

void to_snake_case(char *str);

void c_dir_to_snake_case(char *str);

char *read_all_from_file(FILE *f);

bool is_endwith(const char *str, char *suf);

char *itoa(int x);

time_t get_current_time(char *formatted_time);

void *parse_yaml_with_schema(unsigned char *yaml, unsigned int *ety_len, cyaml_schema_value_t schema);

cJSON *gen_song_json(const char *id,
                     time_t date,
                     const char *set,
                     const char *version,
                     arcpkg_chart_t *charts,
                     uint64_t charts_count);

int get_terminal_width();

char *progress_formatter(unsigned int ety_len, const char *extra);

char **get_arctap_se_list(struct zip_t *zip, const char *c_dir, int *length);

#endif //ETOILE_UTILS_H
