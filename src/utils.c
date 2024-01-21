#include "utils.h"

bool is_file_exist(char *fname) {
    return access(fname, F_OK) == 0;
}

bool make_dir(const char *path) {
    return mkdir(MKDIR_ARGS(path));
}

/**
 * Check if the specific string is contained in file as a line
 * @param filename
 * @param ttg Target string WITHOUT '\n'
 * @return -1 if error opening file, 1 if found and 0 if not found
 */
int is_contained_in_file(char *filename, char *ttg) {
    char *target = NULL;
    target = malloc(strlen(ttg) + 1);
    strcpy(target, ttg);

    strcat(target, "\n");

    FILE *file;
    file = fopen(filename, "r");
    char line[100];

    if (file == NULL) {
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, target) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void trim(char *str) {
    int start = 0;
    unsigned int end = strlen(str) - 1;

    while (isspace(str[start])) {
        start++;
    }

    while (end >= start && isspace(str[end])) {
        end--;
    }

    unsigned int length = end - start + 1;

    if (start > 0) {
        memmove(str, str + start, length);
    }
    str[length] = '\0';
}

/**
 * Format a string into snake case
 * @param str ASCII chars **ONLY**
 */
void to_snake_case(char *str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (65 <= str[i] && str[i] <= 90) {
            str[i] += 0x20;
        } else if (!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '.') {
            str[i] = '_';
        }
    }
}

void c_dir_to_snake_case(char *str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (65 <= str[i] && str[i] <= 90) {
            str[i] += 0x20;
        } else if (!isalpha(str[i]) && !isdigit(str[i])) {
            str[i] = '_';
        }
    }
}

char *read_all_from_file(FILE *f) {
    int size = 1;
    char *s = (char *) malloc(size);
    if (s == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        free(s);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (fgets(buffer, sizeof buffer, f) != NULL) {
        char *tmp = realloc(s, (size += sizeof buffer));
        if (tmp == NULL) {
            fprintf(stderr, "Failed to allocate memory\n");
            free(s);
            exit(EXIT_FAILURE);
        }
        s = (char *) tmp;
        strcat(s, buffer);
    }
    return s;
}

bool is_endwith(const char *str, char *suf) {
    unsigned int str_len = strlen(str);
    unsigned int suffix_len = strlen(suf);

    return (str_len >= suffix_len && strcmp(str + str_len - suffix_len, suf) == 0);
}

char *itoa(int x) {
    char *str = calloc(sizeof(char), (int) log10(x) + 2);
    sprintf(str, "%d", x);
    return str;
}

time_t get_current_time(char *formatted_time) {
    time_t cur_time;
    struct tm *local_time;
    time(&cur_time);
    local_time = localtime(&cur_time);

    strftime(formatted_time, 20, "%Y-%m-%d %H:%M:%S", local_time);
    return cur_time;
}

void *parse_yaml_with_schema(unsigned char *yaml, unsigned int *ety_len, cyaml_schema_value_t schema) {
    cyaml_err_t cyaml_err;
    void *t_ety;

    cyaml_err = cyaml_load_data(yaml,
                                strlen((char *) yaml),
                                &cyaml_cfg,
                                &schema,
                                (void **) &t_ety,
                                ety_len);
    if (cyaml_err != CYAML_OK) {
        fprintf(stderr, "Failed to parse yaml: %s\n", cyaml_strerror(cyaml_err));
        cyaml_free(&cyaml_cfg, &schema, t_ety, *ety_len);
        exit(EXIT_FAILURE);
    }

    if (t_ety == NULL) {
        fprintf(stderr, "Failed to parse yaml: The result is NULL\n");
        exit(EXIT_FAILURE);
    }

    return t_ety;
}

void append_difficulty_json(cJSON *obj_song, cJSON *json_difficulties, arcpkg_chart_t *charts, int chart_index, int rating_class) {
    cJSON *json_difficulty;
    cJSON *json_rating_class, *json_charter, *json_illustrator, *json_rating, *json_rating_plus;
    int i = chart_index;

    json_difficulty = cJSON_CreateObject();
    VALIDATION_CJSON(json_difficulty, obj_song)
    cJSON_AddItemToArray(json_difficulties, json_difficulty);

    const char *charter = charts[i].charter;

    const char *illustrator = charts[i].illustrator;
    if (illustrator == NULL) {
        illustrator = malloc(2);
        strcpy((char *) illustrator, "");
    }

    float constant = charts[i].chart_constant;
    double intpart, fracpart = modf(constant, &intpart);

    bool rating_plus = fracpart >= 0.7 && intpart >= 9;
    int rating = (int) intpart;

    json_rating_class = cJSON_CreateNumber(rating_class);
    VALIDATION_CJSON(json_rating_class, obj_song)
    cJSON_AddItemToObject(json_difficulty, "ratingClass", json_rating_class);

    json_charter = cJSON_CreateString(charter);
    VALIDATION_CJSON(json_charter, obj_song)
    cJSON_AddItemToObject(json_difficulty, "chartDesigner", json_charter);

    json_illustrator = cJSON_CreateString(illustrator);
    VALIDATION_CJSON(json_illustrator, obj_song)
    cJSON_AddItemToObject(json_difficulty, "jacketDesigner", json_illustrator);

    json_rating = cJSON_CreateNumber(rating);
    VALIDATION_CJSON(json_rating, obj_song)
    cJSON_AddItemToObject(json_difficulty, "rating", json_rating);

    if (rating_plus) {
        json_rating_plus = cJSON_CreateBool(rating_plus);
        VALIDATION_CJSON(json_rating_plus, obj_song)
        cJSON_AddItemToObject(json_difficulty, "ratingPlus", json_rating_plus);
    }
}

void append_difficulty_json_placeholder(cJSON *obj_song, cJSON *json_difficulties, int rating_class) {
    cJSON *json_difficulty;
    cJSON *json_rating_class, *json_charter, *json_illustrator, *json_rating;
    json_difficulty = cJSON_CreateObject();
    VALIDATION_CJSON(json_difficulty, obj_song)
    cJSON_AddItemToArray(json_difficulties, json_difficulty);

    const char *charter = "";
    const char *illustrator = "";
    int rating = -1;

    json_rating_class = cJSON_CreateNumber(rating_class);
    VALIDATION_CJSON(json_rating_class, obj_song)
    cJSON_AddItemToObject(json_difficulty, "ratingClass", json_rating_class);

    json_charter = cJSON_CreateString(charter);
    VALIDATION_CJSON(json_charter, obj_song)
    cJSON_AddItemToObject(json_difficulty, "chartDesigner", json_charter);

    json_illustrator = cJSON_CreateString(illustrator);
    VALIDATION_CJSON(json_illustrator, obj_song)
    cJSON_AddItemToObject(json_difficulty, "jacketDesigner", json_illustrator);

    json_rating = cJSON_CreateNumber(rating);
    VALIDATION_CJSON(json_rating, obj_song)
    cJSON_AddItemToObject(json_difficulty, "rating", json_rating);
}

cJSON *gen_song_json(const char *id,
                     time_t date,
                     const char *set,
                     const char *version,
                     arcpkg_chart_t *charts,
                     uint64_t charts_count) {
    const char *title = charts[0].title;
    const char *artist = charts[0].artist;
    const char *bpm_text = charts[0].bpm_text;
    unsigned int base_bpm = charts[0].base_bpm;
    unsigned long preview = (void *) charts[0].preview == NULL ? 0 : charts[0].preview;
    unsigned long preview_end = (void *) charts[0].preview_end == NULL ? 5000 : charts[0].preview_end;
    skin_t skin = charts[0].skin;
    side_t side = skin.side;
    const char *bg = charts[0].bg;

    if ((void *) side == NULL) {
        side = CONFLICT;
    }

    if (bg == NULL) {
        switch (side) {
            case CONFLICT:
                bg = calloc(sizeof(char), 5 + 8 + 1);
                strcpy((char *) bg, "base_conflict");
                break;
            case LIGHT:
                bg = calloc(sizeof(char), 5 + 5 + 1);
                strcpy((char *) bg, "base_light");
                break;
            case COLORLESS:
                bg = calloc(sizeof(char), 8 + 1);
                strcpy((char *) bg, "epilogue");
                break;
        }
    }

    int j;
    for (j = 0; j < strlen(bg); j++) {
        if (bg[j] == '.' &&
            bg[j + 1] == 'j' &&
            bg[j + 2] == 'p' &&
            bg[j + 3] == 'g' &&
            bg[j + 4] == '\0') {
            break;
        }
    }
    char *bg_snake = calloc(sizeof(char), strlen(bg) + 1);
    strcpy(bg_snake, bg);
    to_snake_case((char *) bg_snake);
    bg_snake[j] = '\0';

    cJSON *obj_song = cJSON_CreateObject();
    VALIDATION_CJSON(obj_song, obj_song)

    cJSON *json_id = cJSON_CreateString(id);
    VALIDATION_CJSON(json_id, obj_song)
    cJSON_AddItemToObject(obj_song, "id", json_id);

    cJSON *obj_title_localized = cJSON_CreateObject();
    cJSON *json_title_en = cJSON_CreateString(title);
    VALIDATION_CJSON(obj_title_localized, obj_song)
    VALIDATION_CJSON(json_title_en, obj_song)
    cJSON_AddItemToObject(obj_title_localized, "en", json_title_en);
    cJSON_AddItemToObject(obj_song, "title_localized", obj_title_localized);

    cJSON *json_artist = cJSON_CreateString(artist);
    VALIDATION_CJSON(json_artist, obj_song)
    cJSON_AddItemToObject(obj_song, "artist", json_artist);

    cJSON *json_bpm_text = cJSON_CreateString(bpm_text);
    VALIDATION_CJSON(json_bpm_text, obj_song)
    cJSON_AddItemToObject(obj_song, "bpm", json_bpm_text);

    cJSON *json_bpm = cJSON_CreateNumber(base_bpm);
    VALIDATION_CJSON(json_bpm, obj_song)
    cJSON_AddItemToObject(obj_song, "bpm_base", json_bpm);

    cJSON *json_set = cJSON_CreateString(set);
    VALIDATION_CJSON(json_set, obj_song)
    cJSON_AddItemToObject(obj_song, "set", json_set);

    cJSON *json_purchase = cJSON_CreateString("");
    VALIDATION_CJSON(json_purchase, obj_song)
    cJSON_AddItemToObject(obj_song, "purchase", json_purchase);

    cJSON *json_audio_preview = cJSON_CreateNumber((double) preview);
    VALIDATION_CJSON(json_audio_preview, obj_song)
    cJSON_AddItemToObject(obj_song, "audioPreview", json_audio_preview);

    cJSON *json_audio_preview_end = cJSON_CreateNumber((double) preview_end);
    VALIDATION_CJSON(json_audio_preview_end, obj_song)
    cJSON_AddItemToObject(obj_song, "audioPreviewEnd", json_audio_preview_end);

    cJSON *json_side = cJSON_CreateNumber(side);
    VALIDATION_CJSON(json_side, obj_song)
    cJSON_AddItemToObject(obj_song, "side", json_side);

    cJSON *json_bg = cJSON_CreateString(bg_snake);
    VALIDATION_CJSON(json_bg, obj_song)
    cJSON_AddItemToObject(obj_song, "bg", json_bg);

    cJSON *json_remote_dl = cJSON_CreateBool(false);
    VALIDATION_CJSON(json_remote_dl, obj_song)
    cJSON_AddItemToObject(obj_song, "remote_dl", json_remote_dl);

    cJSON *json_date = cJSON_CreateNumber((double) date);
    VALIDATION_CJSON(json_date, obj_song)
    cJSON_AddItemToObject(obj_song, "date", json_date);

    cJSON *json_version = cJSON_CreateString(version);
    VALIDATION_CJSON(json_version, obj_song)
    cJSON_AddItemToObject(obj_song, "version", json_version);

    cJSON *json_difficulties = cJSON_CreateArray();
    VALIDATION_CJSON(json_difficulties, obj_song)
    cJSON_AddItemToObject(obj_song, "difficulties", json_difficulties);


    int rating_class_tag[4] = {-1, -1, -1, -1};

    int i, c_rating_class;
    for (i = 0; i < charts_count; i++) {
        c_rating_class = atoi(charts[i].chart_path); // NOLINT(*-err34-c)
        rating_class_tag[c_rating_class] = i;
    }

    for (i = 0; i < 4; i++) {
        if (rating_class_tag[i] != -1) {
            append_difficulty_json(obj_song, json_difficulties, charts, rating_class_tag[i], i);
        } else {
            if (i < 3) append_difficulty_json_placeholder(obj_song, json_difficulties, i);
        }
    }

    return obj_song;
}

char *progress_formatter(unsigned int ety_len, const char *extra) {
    //  Working on '%s' (%s/%s)\n
    // for example, `ety_len` eql to 234
    int ety_len_len = (int) log10(ety_len) + 1; // then `ety_len_len` is 3
    int ety_len_len_len = (int) log10(ety_len_len) + 1; // `ety_len_len_len` is the length of `ety_len_len`, which is 1
    char *orig_fmt = calloc(sizeof(char), 8 + strlen(extra) + 1);
    VALIDATION_ALLOC(orig_fmt)
    sprintf(orig_fmt, "%s%s", "[%d/%d] ", extra);

    char *progress_fmt = calloc(sizeof(char), strlen(orig_fmt) + 2 * ety_len_len_len + 1);
    VALIDATION_ALLOC(progress_fmt)

    sprintf(progress_fmt, "[%%%dd/%%%dd] ", ety_len_len, ety_len_len);
    strcat(progress_fmt, extra);
    return progress_fmt;
}

char **get_arctap_se_list(struct zip_t *zip, const char *c_dir, int *length) {
    char **s = NULL;
    unsigned long size = 0;

    ssize_t n = zip_entries_total(zip);
    int i;
    for (i = 0, *length = 0; i < n; i++) {
        zip_entry_openbyindex(zip, i);
        const char *name = zip_entry_name(zip);
        if (is_endwith(name, ".wav")) {
            char *c_dir_real, *se;
            c_dir_real = strtok((char *) name, "/");
            se = strtok(NULL, "/");

            if (strcmp(c_dir, c_dir_real) == 0) {
                if (*length > 0) {
                    s = realloc(s, sizeof(char *) * (size += strlen(se) + 1));
                } else {
                    s = calloc(sizeof(char *), (size += strlen(se) + 1));
                }
                VALIDATION_ALLOC(s)
                s[*length] = strdup(se);
                *length = *length + 1;
            }
        }
        zip_entry_close(zip);
    }

    return s;
}

#ifdef WIN32

int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return width;
}

#endif

#ifdef linux

int get_terminal_width() {
    FILE *p = popen("tput cols", "r");
    if (p == NULL) {
        return -1;
    }

    char output[16];
    fgets(output, sizeof(output), p);
    pclose(p);

    int width = atoi(output);
    return width;
}

#endif