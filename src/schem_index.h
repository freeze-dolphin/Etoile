#ifndef ETOILE_SCHEM_INDEX_H
#define ETOILE_SCHEM_INDEX_H

#include "utils.h"

#include <cyaml/cyaml.h>

typedef enum index_type {
    ENTRY_TYPE_LEVEL, ENTRY_TYPE_PACK
} index_type_t;

typedef struct arcpkg_entry {
    const char *directory;
    const char *identifier;
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
                "identifier", CYAML_FLAG_POINTER,
                arcpkg_entry_t, identifier, 0, CYAML_UNLIMITED),
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

#endif //ETOILE_SCHEM_INDEX_H
