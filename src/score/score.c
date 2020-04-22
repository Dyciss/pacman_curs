#include "game/gamemodel.h"
#include "settings/settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define scoreboard_len 5

struct Record {
    char *name;
    int score;
    int difficulty;
};

static char *record2str(struct Record r) {
    char *str = (char *)calloc(settings_field(User)->max_len + 1 + 20 + 15 + 15,
                               sizeof(char));
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
    char *line = (char *)calloc(
        settings_field(User)->max_len + 1 + 20 + 15 + 15, sizeof(char));
    int added = 0;
    int i;
    int place = 0;
    for (i = 0; i < scoreboard_len; i++) {
        if (fgets(line, settings_field(User)->max_len + 20 + 15 + 15,
                  score_file) == NULL) {
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
