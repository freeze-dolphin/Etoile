#ifndef ETOILE_SCHEM_PROJ_H
#define ETOILE_SCHEM_PROJ_H

#include "utils.h"

#include <cyaml/cyaml.h>

typedef enum side {
    CONFLICT = 0, LIGHT = 1, COLORLESS = 2
} side_t;

typedef struct skin {
    side_t side;
} skin_t;

typedef struct arcpkg_chart {
    const char *chart_path;
    const char *audio_path;
    const char *jacket_path;
    const char *bpm_text;
    const char *bg;
    const char *title;
    const char *charter;
    const char *artist;
    const char *illustrator;
    unsigned int base_bpm;
    skin_t skin;
    unsigned long preview;
    unsigned long preview_end;
    float chart_constant;
} arcpkg_chart_t;

typedef struct arcpkg_proj_setting {
    arcpkg_chart_t *charts;
    uint64_t charts_count;
} arcpkg_proj_setting_t;

static const cyaml_strval_t side_strings[] = {
        {"conflict",  CONFLICT},
        {"light",     LIGHT},
        {"colorless", COLORLESS},
};

static const cyaml_schema_field_t skin_fields_schema[] = {
        CYAML_FIELD_ENUM(
                "side", CYAML_FLAG_OPTIONAL,
                skin_t, side, side_strings,
                CYAML_ARRAY_LEN(side_strings)),
        CYAML_FIELD_END
};

static const cyaml_schema_field_t chart_fields_schema[] = {
        CYAML_FIELD_STRING_PTR(
                "chartPath", CYAML_FLAG_POINTER,
                arcpkg_chart_t, chart_path, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "audioPath", CYAML_FLAG_POINTER,
                arcpkg_chart_t, audio_path, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "jacketPath", CYAML_FLAG_POINTER,
                arcpkg_chart_t, jacket_path, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "bpmText", CYAML_FLAG_POINTER,
                arcpkg_chart_t, bpm_text, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "backgroundPath", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                arcpkg_chart_t, bg, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "title", CYAML_FLAG_POINTER,
                arcpkg_chart_t, title, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "alias", CYAML_FLAG_POINTER,
                arcpkg_chart_t, charter, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "composer", CYAML_FLAG_POINTER,
                arcpkg_chart_t, artist, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "illustrator", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                arcpkg_chart_t, illustrator, 0, CYAML_UNLIMITED),
        CYAML_FIELD_UINT("baseBpm", CYAML_FLAG_DEFAULT, arcpkg_chart_t, base_bpm),
        CYAML_FIELD_MAPPING("skin", CYAML_FLAG_OPTIONAL, arcpkg_chart_t, skin, skin_fields_schema),
        CYAML_FIELD_UINT("previewStart", CYAML_FLAG_OPTIONAL, arcpkg_chart_t, preview),
        CYAML_FIELD_UINT("previewEnd", CYAML_FLAG_OPTIONAL, arcpkg_chart_t, preview_end),
        CYAML_FIELD_FLOAT("chartConstant", CYAML_FLAG_OPTIONAL, arcpkg_chart_t, chart_constant),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t chart_schema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            arcpkg_chart_t, chart_fields_schema),
};

static const cyaml_schema_field_t proj_fields_schema[] = {
        CYAML_FIELD_IGNORE("lastOpenedChartPath", CYAML_FLAG_OPTIONAL),
        CYAML_FIELD_SEQUENCE("charts", CYAML_FLAG_POINTER, arcpkg_proj_setting_t, charts, &chart_schema, 0, 4),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t proj_schema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
                            arcpkg_proj_setting_t, proj_fields_schema),
};

#endif //ETOILE_SCHEM_PROJ_H
