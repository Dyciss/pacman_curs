#include <stdlib.h>
#include <string.h>

#include "settings.h"

struct field Settings[LAST_FIELD];

struct field *settings_field(enum setting_field f) {
    return &Settings[f];
}

void settings_init() {
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