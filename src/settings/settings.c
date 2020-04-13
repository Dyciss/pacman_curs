#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"

struct field Settings[LAST_FIELD];

struct field *settings_field(enum setting_field f) {
    return &Settings[f];
}

void settings_init() {
    Settings[Load_file].max_len = 64;
    Settings[User].max_len = 32;
    Settings[Level].max_len = 1;

    for (int field = 0; field < LAST_FIELD; field++) {
        Settings[field].text =
            (char *)calloc(Settings[field].max_len + 1, sizeof(char));
    }
}

void settings_free() {
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
    case Level:
        return (Settings[Level].text[0] < '4' &&
                Settings[Level].text[0] >= '0');
    case User:
        return Settings[User].text[0] != '\0'; // len != 0
    case Load_file:
        return file_exist(Settings[Load_file].text);
    default:
        return 1;
    }
}