#include "game/gamemodel.h"
#include "settings/settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define scoreboard_len 10

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

void add_to_scoreboard(Game *game) {
    struct Record r = (struct Record){.name = settings_field(User)->text,
                                      .score = game->score,
                                      .difficulty = game->difficalty};
    char fname[8] = ".score";
    strncat(fname, settings_field(Difficalty)->text, 1);
    FILE *score_file = fopen(fname, "r");
    FILE *tmp = tmpfile();
    if (score_file == NULL) {
        score_file = fopen(fname, "w");
        fputs(record2str(r), score_file);
        fclose(score_file);
        return;
    }
    char *line = (char *)calloc(
        settings_field(User)->max_len + 1 + 20 + 15 + 15, sizeof(char));
    int added = 0;
    int i;
    for (i = 0; i < scoreboard_len; i++) {
        if (fgets(line, settings_field(User)->max_len + 20 + 15 + 15,
                  score_file) == NULL) {
            break;
        }
        struct Record current = str2record(line);
        if (!added && r.score >= current.score) {
            fputs(record2str(r), tmp);
            added = 1;
        }
        fputs(line, tmp);
    }
    if (!added && i < scoreboard_len) {
        fputs(record2str(r), tmp);
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
}
