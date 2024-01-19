#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cyaml/cyaml.h>
#include <time.h>

typedef enum index_type {
    ENTRY_TYPE_LEVEL, ENTRY_TYPE_PACK
} index_type_t;

typedef struct arcpkg_entry {
    const char *directory;
    const char *setting_file;
    index_type_t type;
} arcpkg_entry_t;

static const cyaml_strval_t entry_type_strings[] = {
        {"level", ENTRY_TYPE_LEVEL},
        {"pack",  ENTRY_TYPE_PACK},
};

static const cyaml_schema_field_t entry_fields_schema[] = {
        CYAML_FIELD_STRING_PTR(
                "directory", CYAML_FLAG_POINTER,
                arcpkg_entry_t, directory, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "settingsFile", CYAML_FLAG_POINTER,
                arcpkg_entry_t, setting_file, 0, CYAML_UNLIMITED),
        CYAML_FIELD_ENUM("type", CYAML_FLAG_DEFAULT,
                         arcpkg_entry_t, type, entry_type_strings, CYAML_ARRAY_LEN(entry_type_strings)),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t entry_schema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            arcpkg_entry_t, entry_fields_schema),
};

static const cyaml_schema_value_t index_schema = {
        CYAML_VALUE_SEQUENCE(CYAML_FLAG_POINTER, arcpkg_entry_t,
                             &entry_schema, 0, CYAML_UNLIMITED)
};

int main(void) {
    cyaml_err_t err;
    struct arcpkg_entry *ety;
    unsigned ety_count;
    static const unsigned char *yaml =
            "- directory: LivingWill\n"
            "  identifier: Pilcrow_.LivingWill\n"
            "  settingsFile: project.arcproj\n"
            "  type: level\n";

    static const cyaml_config_t config = {
            .log_level = CYAML_LOG_NOTICE,
            .mem_fn = cyaml_mem,
            .log_fn = cyaml_log,
            .flags = CYAML_CFG_IGNORE_UNKNOWN_KEYS,
    };

    err = cyaml_load_data(yaml,
                          strlen((char *) yaml),
                          &config,
                          &index_schema,
                          (void **) &ety,
                          &ety_count);
    if (err != CYAML_OK) {
        fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
        return EXIT_FAILURE;
    }

    printf("Entry count: %u\n", ety_count);
    for (unsigned i = 0; i < ety_count; i++) {
        printf("Entry:\n");
        printf("\tdir: %s\n", ety[i].directory);
        printf("\tsetting: %s\n", ety[i].setting_file);
        printf("\ttype: %i\n", ety[i].type);

        free((void *) ety[i].directory);
        free((void *) ety[i].setting_file);
    }

    free(ety);

    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);

    // 格式化时间为字符串
    char formattedTime[20];
    strftime(formattedTime, 20, "%Y-%m-%d %H:%M:%S", localTime);

    // 打印时间字符串
    printf("%s\n", formattedTime);

    return EXIT_SUCCESS;
}
