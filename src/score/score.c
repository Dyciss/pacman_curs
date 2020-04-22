#include "game/gamemodel.h"
#include "settings/settings.h"
#include "ui/render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define scoreboard_len 5

#define score_digits_max 25
// max len of line in score file
#define file_line_len                                                          \
    (settings_field(User)->max_len + 1 + score_digits_max + 15 + 15)

struct Record {
    char *name;
    int score;
    int difficulty;
};

static char *record2str(struct Record r) {
    char *str = (char *)calloc(file_line_len + 1, sizeof(char));
    sprintf(str, "Name: %s\tScore: %i\n", r.name, r.score);
    return str;
}

static struct Record str2record(char *str) {
    struct Record r;
    r.name = (char *)calloc(settings_field(User)->max_len + 1, sizeof(char));
    sscanf(str, "Name: %s\tScore: %i", r.name, &r.score);
    return r;
}

int add_to_scoreboard(Game *game) {
    //
    // returns place in scoreboard, 0 if no place
    //
    struct Record r = (struct Record){.name = settings_field(User)->text,
                                      .score = game->score,
                                      .difficulty = game->difficulty};

    char fname[8] = ".score";
    strncat(fname, settings_field(Difficulty)->text, 1);

    FILE *score_file = fopen(fname, "r");
    FILE *tmp = tmpfile();

    if (score_file == NULL) {
        score_file = fopen(fname, "w");
        char *r_line = record2str(r);
        fputs(r_line, score_file);
        free(r_line);
        fclose(score_file);
        fclose(tmp);
        return 1;
    }

    char *line = (char *)calloc(file_line_len + 1, sizeof(char));
    int added = 0;
    int i;
    int place = 0;

    for (i = 0; i < scoreboard_len; i++) {
        if (fgets(line, file_line_len + 1, score_file) == NULL) {
            break;
        }
        struct Record current = str2record(line);
        if (!added && r.score >= current.score) {
            char *r_line = record2str(r);
            fputs(r_line, tmp);
            free(r_line);
            added = 1;
            place = i + 1;
        }
        if (i + added < scoreboard_len) {
            fputs(line, tmp);
        }
    }

    free(line);
    line = NULL;

    if (!added && i < scoreboard_len) {
        place = i + 1;
        char *r_line = record2str(r);
        fputs(r_line, tmp);
        free(r_line);
    }

    fclose(score_file);
    score_file = fopen(fname, "wb");
    rewind(tmp);

    char buffer[100];
    int read;

    while ((read = fread(buffer, sizeof(char), 100, tmp))) {
        fwrite(buffer, sizeof(char), read, score_file);
    }

    fclose(tmp);
    fclose(score_file);
    return place;
}

void print_scores() {
    char fname[8] = ".score";
    strncat(fname, settings_field(Difficulty)->text, 1);
    FILE *score_file = fopen(fname, "r");
    if (!score_file) {
        render_string("No data", -7 * FONT_WIDTH, 1 - FONT_HEIGHT_UPPER_CASE,
                      FONT, WHITE);
        return;
    }

    char *line = (char *)calloc(file_line_len + 1, sizeof(char));
    char text[256] = {'\0'};
    for (int i = 0; i < scoreboard_len; i++) {
        if (fgets(line, file_line_len + 1, score_file) == NULL) {
            break;
        }
        struct Record current = str2record(line);
        snprintf(text, 255, "%i) %i - %s", i + 1, current.score, current.name);
        render_string(text, -FONT_WIDTH * strlen(text),
                      1 - FONT_HEIGHT_UPPER_CASE * (i + 1), FONT, WHITE);
    }
    fclose(score_file);
    free(line);
}