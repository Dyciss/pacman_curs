#pragma once

#include <string.h>

enum setting_field { User, Difficalty, Load_file, LAST_FIELD };
struct field {
    char *text;
    size_t max_len;
};

struct field *settings_field(enum setting_field f);
int validate_field(enum setting_field f);

void settings_init();
void settings_free();