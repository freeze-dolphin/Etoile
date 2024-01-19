#include "args.h"
#include "utils.h"
#include "schem_index.h"

#include <cjson/cJSON.h>
#include <zip.h>
#include <wand/MagickWand.h>

int main(int argc, char *argv[]) {
    const char *path_arcpkg = NULL, *path_songs_dir = NULL, *path_bg_dir = NULL, *s_packname = NULL, *s_version = NULL;
    bool flag_dbg = false, flag_force = false, flag_auto_fix_constant = false;

    char *s_addition_version = "1.0";

    cag_option_context context;
    cag_option_init(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
    while (cag_option_fetch(&context)) {
        switch (cag_option_get_identifier(&context)) {
            case 'i':
                path_arcpkg = cag_option_get_value(&context);
                break;
            case 'o':
                path_songs_dir = cag_option_get_value(&context);
                break;
            case 'b':
                path_bg_dir = cag_option_get_value(&context);
                break;
            case 'p':
                s_packname = cag_option_get_value(&context);
                break;
            case 'v':
                s_version = cag_option_get_value(&context);
                break;
            case 'f':
                flag_force = true;
                break;
            case 'F':
                flag_auto_fix_constant = true;
                break;
            case 'h':
                print_help_msg(true);
                return EXIT_SUCCESS;
            case 'd':
                flag_dbg = true;
                break;
        }
    }

    if (s_version != NULL) {
        s_addition_version = calloc(sizeof(char), strlen(s_version) + 1);
        strcpy(s_addition_version, s_version);
    }

    int param_index = cag_option_get_index(&context);
    if (param_index < argc) {
        fprintf(stderr, "[ERROR] Arguments did not match\n");
        print_help_msg(false);
        return EXIT_SUCCESS;
    }

    // validation

    if (path_arcpkg == NULL || path_songs_dir == NULL || path_bg_dir == NULL || s_packname == NULL) {
        fprintf(stderr, "[ERROR] Required arguments did not met\n");
        print_help_msg(false);
        return EXIT_SUCCESS;
    }

    if (!is_endwith(path_arcpkg, ".arcpkg")) {
        fprintf(stderr, "[ERROR] An .arcpkg file is required\n");
        print_help_msg(false);
        return EXIT_SUCCESS;
    }

    char *path_songlist = calloc(sizeof(char), strlen(path_songs_dir) + 10);
    sprintf(path_songlist, "%s/songlist", path_songs_dir);

    char *path_log = calloc(sizeof(char), strlen(path_songs_dir) + 12);
    sprintf(path_log, "%s/etoile.log", path_songs_dir);
    FILE *log_file = fopen(path_log, "w");

    if (!is_file_exist((char *) path_songs_dir)) {
        bool mkdir_err = make_dir(path_songs_dir);
        if (mkdir_err) {
            fprintf(stderr, "[ERROR] Failed to create dir '%s'\n", path_songs_dir);
            return EXIT_FAILURE;
        }
    }

    if (!is_file_exist((char *) path_bg_dir)) {
        fprintf(stderr, "[ERROR] Unable to locate bg dir '%s'\n", path_bg_dir);
        return EXIT_FAILURE;
    }

    FILE *f_songlist = NULL;
    if (!is_file_exist((char *) path_songlist)) {
        goto write_songlist;
    } else if (!flag_force) {
        printf("songlist existed, overwrite? (Y/N): ");
        char resp;
        scanf("%c", &resp);
        getchar();
        if (resp == 'y' || resp == 'Y') {
            goto write_songlist;
        } else {
            return EXIT_SUCCESS;
        }
    } else {
        fprintf(log_file, "[INFO] Overwriting '%s'\n", path_songlist);

        write_songlist:
        f_songlist = fopen(path_songlist, "w");
        if (f_songlist == NULL) {
            fprintf(stderr, "[ERROR] Cannot create file 'songlist'\n");
            fclose(f_songlist);
            return EXIT_FAILURE;
        }
        fclose(f_songlist);
    }

    f_songlist = fopen(path_songlist, "r+");

    if (f_songlist == NULL) {
        fprintf(stderr, "[ERROR] Cannot open file 'songlist'\n");
        fclose(f_songlist);
        return EXIT_FAILURE;
    }

    // init

    char formatted_time[20];
    time_t cur_time = get_current_time(formatted_time);

    MagickWandGenesis();

    cJSON *json_songlist = cJSON_CreateObject();
    VALIDATION_CJSON(json_songlist, json_songlist)

    cJSON *json_songs = cJSON_CreateArray();
    VALIDATION_CJSON(json_songs, json_songlist)
    cJSON_AddItemToObject(json_songlist, "songs", json_songs);

    // open arcpkg

    unsigned char *s_index_yml = NULL;

    struct zip_t *zip = zip_open(path_arcpkg, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

    { // open 'index.yml'
        size_t bufsize;
        zip_entry_open(zip, "index.yml");
        bufsize = zip_entry_size(zip);
        s_index_yml = calloc(sizeof(unsigned char), bufsize + 1);
        zip_entry_noallocread(zip, (void *) s_index_yml, bufsize);
        zip_entry_close(zip);
    }

    { // parse 'index.yml'
        unsigned int ety_len;
        arcpkg_entry_t *ety = parse_yaml_with_schema(s_index_yml, &ety_len, index_schema);

        int i;
        for (i = 0; i < ety_len; i++) {
            if (ety[i].type != ENTRY_TYPE_LEVEL) continue; // only support level

            const char *c_dir = ety[i].directory;
            char *c_dir_snake = malloc(strlen(c_dir) * sizeof(char) + 1);
            const char *c_id = ety[i].identifier;
            const char *c_settings = ety[i].setting_file;

            sprintf(c_dir_snake, "%s", c_dir);

            to_snake_case((char *) c_dir_snake);

#ifdef DEBUG
            DBG(c_dir)
            DBG(c_dir_snake)
            DBG(c_settings)
#endif

            printf(progress_formatter(ety_len), i + 1, ety_len, c_id, path_songs_dir, c_dir_snake);

            char *proj_setting_path = calloc(sizeof(char), strlen(c_dir) + strlen(c_settings) + 2);
            VALIDATION_ALLOC(proj_setting_path)
            sprintf(proj_setting_path, "%s/%s", c_dir, c_settings);
            DBG(proj_setting_path)

            char *out_proj_path = calloc(sizeof(char), strlen(path_songs_dir) + strlen(c_dir) + 2);
            VALIDATION_ALLOC(out_proj_path)
            sprintf(out_proj_path, "%s/%s", path_songs_dir, c_dir_snake);
            DBG_I(out_proj_path, 1)

            unsigned char *s_proj_setting_yml = NULL;
            { // open project settings
                size_t bufsize;
                zip_entry_open(zip, proj_setting_path);
                bufsize = zip_entry_size(zip);
                s_proj_setting_yml = calloc(sizeof(unsigned char), bufsize + 1);
                zip_entry_noallocread(zip, (void *) s_proj_setting_yml, bufsize);
                zip_entry_close(zip);
            }
            { // parse project settings
                arcpkg_proj_setting_t *proj = parse_yaml_with_schema(s_proj_setting_yml, NULL, proj_schema);

                bool aborted = false;

                int j;
                for (j = 0; j < proj->charts_count; j++) {
                    const char *bg = proj->charts[j].bg;
                    const char *chart_path = proj->charts[j].chart_path;
                    const char *audio_path = proj->charts[j].audio_path;
                    const char *jacket_path = proj->charts[j].jacket_path;

                    bool bg_null = true;

                    char *bg_snake = NULL;
                    if (bg != NULL) {
                        bg_null = false;
                        bg_snake = calloc(sizeof(char), strlen(bg) + 1);
                        VALIDATION_ALLOC(bg_snake)
                        strcpy(bg_snake, bg);
                        to_snake_case((char *) bg_snake);
                    }

#ifdef DEBUG
                    float cons = proj->charts[j].chart_constant;
                    double intpart, fracpart = modf(cons, &intpart);
                    char *constant = malloc(6);
                    sprintf(constant, "constant: %.1lf + %.1lf", intpart, fracpart);
                    DBG(constant)
#endif
                    if (proj->charts[j].chart_constant < 1.0) {
                        if (flag_auto_fix_constant) {
                            // TODO 2
                            fprintf(log_file,
                                    "[WARN] Chart constant of '%s' (from '%s', constant = %lf) is below one. Fixing it to 1.0...\n",
                                    chart_path,
                                    c_id,
                                    proj->charts[j].chart_constant);
                            proj->charts[j].chart_constant = 1.0F;
                        } else {
                            fprintf(log_file, "[WARN] Chart constant of '%s' (from '%s', constant = %lf) is below one, ignored\n",
                                    chart_path,
                                    c_id,
                                    proj->charts[j].chart_constant);
                            aborted = true;
                            continue;
                        }
                    }

                    int rating_class = atoi(chart_path); // NOLINT(*-err34-c)
                    if (!isdigit(chart_path[0]) || 0 > rating_class || 3 < rating_class) {
                        fprintf(stderr, "[ERROR] Cannot detect chart rating class for '%s' (from '%s'), ignored\n", chart_path, c_id);
                        aborted = true;
                        continue;
                    }

                    if (!is_file_exist(out_proj_path)) {
                        make_dir(out_proj_path);
                    }

                    { // extract resources
                        char *proj_bg_path = NULL;
                        if (!bg_null) {
                            proj_bg_path = calloc(sizeof(char), strlen(c_dir) + strlen(bg) + 2);
                            VALIDATION_ALLOC(proj_bg_path)
                            sprintf(proj_bg_path, "%s/%s", c_dir, bg);
                        }

                        char *proj_chart_path = calloc(sizeof(char), strlen(c_dir) + strlen(chart_path) + 2);
                        char *proj_audio_path = calloc(sizeof(char), strlen(c_dir) + strlen(audio_path) + 2);
                        char *proj_jacket_path = calloc(sizeof(char), strlen(c_dir) + strlen(jacket_path) + 2);
                        VALIDATION_ALLOC(proj_chart_path)
                        VALIDATION_ALLOC(proj_audio_path)
                        VALIDATION_ALLOC(proj_jacket_path)
                        sprintf(proj_chart_path, "%s/%s", c_dir, chart_path);
                        sprintf(proj_audio_path, "%s/%s", c_dir, audio_path);
                        sprintf(proj_jacket_path, "%s/%s", c_dir, jacket_path);

#ifdef DEBUG
                        if (!bg_null) DBG(proj_bg_path)
                        DBG(proj_chart_path)
                        DBG(proj_audio_path)
                        DBG(proj_jacket_path)
#endif

                        char *out_chart_path = calloc(sizeof(char), strlen(out_proj_path) + strlen(chart_path) + 2);
                        VALIDATION_ALLOC(out_chart_path)
                        sprintf(out_chart_path, "%s/%s", out_proj_path, chart_path);

                        char *out_bg_path = NULL;
                        if (!bg_null) {
                            out_bg_path = calloc(sizeof(char), strlen(path_bg_dir) + strlen(bg_snake) + 2);
                            VALIDATION_ALLOC(out_bg_path)
                            sprintf(out_bg_path, "%s/%s", path_bg_dir, bg_snake);
                        }

                        char *base_slot = NULL;
                        switch (rating_class) {
                            case 0:
                            case 1:
                            case 3:
                                base_slot = calloc(sizeof(char), 2);
                                VALIDATION_ALLOC(base_slot)
                                sprintf(base_slot, "%d", rating_class);
                                break;
                            default:
                                base_slot = calloc(sizeof(char), 5);
                                VALIDATION_ALLOC(base_slot)
                                strcpy(base_slot, "base");
                                break;
                        }

                        /* TODO 1
                         * if PST, PRS and BYD use the same audio / jacket,
                         * it should be optimized to use 'base' as audio / jacket
                         */
                        char *out_audio_path = calloc(sizeof(char), strlen(out_proj_path) + strlen(base_slot) + 6);
                        char *out_jacket_path = calloc(sizeof(char), strlen(out_proj_path) + strlen(base_slot) + 6);
                        char *out_jacket_256_path = calloc(sizeof(char), strlen(out_proj_path) + strlen(base_slot) + 10);
                        sprintf(out_audio_path, "%s/%s.ogg", out_proj_path, base_slot);
                        sprintf(out_jacket_path, "%s/%s.jpg", out_proj_path, base_slot);
                        sprintf(out_jacket_256_path, "%s/%s_256.jpg", out_proj_path, base_slot);

#ifdef DEBUG
                        if (!bg_null) DBG(out_bg_path)
                        DBG(out_chart_path)
                        DBG(out_audio_path)
                        DBG(out_jacket_path)
#endif

                        if (!bg_null) { // extract bg
                            if (is_file_exist(out_bg_path)) { // warn the user
                                if (!flag_force) {
                                    printf("Background img '%s' (from '%s') has existed in your bg dir, overwrite? (Y/N): ", bg_snake,
                                           c_id);
                                    char resp;
                                    scanf("%c", &resp);
                                    getchar();
                                    if (resp != 'y' && resp != 'Y') goto skip_bg_extract;
                                } else {
                                    fprintf(log_file, "[INFO] Overwriting background img '%s' (from '%s')\n", bg_snake, c_id);
                                }
                            }
                            zip_entry_open(zip, proj_bg_path);
                            zip_entry_fread(zip, out_bg_path);
                            zip_entry_close(zip);
                        }

                        skip_bg_extract:
                        { // extract chart
                            zip_entry_open(zip, proj_chart_path);
                            zip_entry_fread(zip, out_chart_path);
                            zip_entry_close(zip);
                        }

                        { // extract audio
                            zip_entry_open(zip, proj_audio_path);
                            zip_entry_fread(zip, out_audio_path);
                            zip_entry_close(zip);
                        }

                        { // extract jacket
                            zip_entry_open(zip, proj_jacket_path);
                            zip_entry_fread(zip, out_jacket_path);
                            zip_entry_close(zip);

                            MagickWand *magick_wand = NewMagickWand();
                            { // resize to 256x using the wand
                                MagickReadImage(magick_wand, out_jacket_path);
                                MagickResizeImage(magick_wand, 256, 256, LanczosFilter, 1.0);
                                MagickWriteImage(magick_wand, out_jacket_256_path);
                            }
                            magick_wand = DestroyMagickWand(magick_wand);
                        }
                    }
                }
                { // append songlist
                    if (!aborted) {
                        cJSON *json_song = gen_song_json(c_dir_snake, cur_time, s_packname, s_addition_version, proj->charts,
                                                         proj->charts_count);
                        VALIDATION_CJSON(json_song, json_songlist)

                        cJSON_AddItemToArray(json_songs, json_song);
                    }
                }
            }
        }
    }
    zip_close(zip);

    fclose(log_file);

    char *songlist_rst = cJSON_Print(json_songlist);
    fwrite(songlist_rst, sizeof(char), strlen(songlist_rst), f_songlist);
    fclose(f_songlist);
    MagickWandTerminus();

    printf("[INFO] Operation success, timestamp: %s\n", formatted_time);

    return EXIT_SUCCESS;
}
