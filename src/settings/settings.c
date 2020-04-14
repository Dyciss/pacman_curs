#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"

char settings_fname[] = ".pacman-settings";

struct field Settings[LAST_FIELD];

struct field *settings_field(enum setting_field f) {
    return &Settings[f];
}

#define CHECK(result, scanf)                                                   \
    {                                                                          \
        r = scanf;                                                             \
        if (r == EOF) {                                                        \
            fclose(file);                                                      \
            return;                                                            \
        }                                                                      \
    }
void settings_init() {
    // change it below (fscanf, fprintf) also!
    Settings[Load_file].max_len = 64;
    Settings[User].max_len = 32;
    Settings[Difficalty].max_len = 1;

    for (int field = 0; field < LAST_FIELD; field++) {
        Settings[field].text =
            (char *)calloc(Settings[field].max_len + 1, sizeof(char));
    }

    FILE *file = fopen(settings_fname, "r");
    if (file == NULL)
        return;

    int r = 0;
    CHECK(r, fscanf(file, "%1s", Settings[Difficalty].text));
    CHECK(r, fscanf(file, "%32s", Settings[User].text));
    CHECK(r, fscanf(file, "%64s", Settings[Load_file].text));
    fclose(file);
}
#undef CHECK

void settings_free() {
    FILE *file = fopen(settings_fname, "w");
    fprintf(file, "%s\n", Settings[Difficalty].text);
    fprintf(file, "%s\n", Settings[User].text);
    fprintf(file, "%s\n", Settings[Load_file].text);
    fclose(file);

    for (int field = 0; field < LAST_FIELD; field++) {
        free(Settings[field].text);
    }
}

static int file_exist(char *f) {
    FILE *file = fopen(f, "r");
    if (file == NULL)
        return 0;
    fclose(file);
    return 1;
}

int validate_field(enum setting_field f) {
    switch (f) {
    case Difficalty:
        return (Settings[Difficalty].text[0] < '4' &&
                Settings[Difficalty].text[0] >= '0');
    case User:
        return Settings[User].text[0] != '\0'; // len != 0
    case Load_file:
        return file_exist(Settings[Load_file].text);
    default:
        return 1;
    }
}