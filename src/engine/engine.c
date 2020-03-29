#include "gamemodel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 28
#define Y 32
struct cell field[Y][X] = {0};

void rebirth(Game *game) {
    //
    // creatures position <- start position
    //
    game->field[game->pacman->x - 1][game->pacman->y - 1] = game->pacman->under;
    for (int i = 0; i < game->ghost_count; i++) {
        game->field[game->ghosts[i]->x - 1][game->ghosts[i]->y - 1] =
            game->ghosts[i]->under;
    }
    game->field[game->pacman->start_position.x - 1]
               [game->pacman->start_position.y - 1] = PACMAN_CELL;
    for (int i = 0; i < game->ghost_count; i++) {
        game->field[game->ghosts[i]->start_position.x - 1]
                   [game->ghosts[i]->start_position.y - 1] = GHOST_CELL(i);
        game->ghosts[i]->x = game->ghosts[i]->start_position.x;
        game->ghosts[i]->y = game->ghosts[i]->start_position.y;
    }
    game->pacman->x = game->pacman->start_position.x;
    game->pacman->y = game->pacman->start_position.y;

}

void set_level(Game *game) {
    rebirth(game);
    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            if (game->field[x][y].object == Eaten_Food) {
                game->field[x][y].object = Food;
            }
        }
    }

    for (int i = 0; i < game->ghost_count; i++) {
        game->ghosts[i]->speed = 450;
        game->ghosts[i]->direction = LEFT;
        game->ghosts[i]->animation_status = 0;
    }
    game->pacman->animation_status = 0;
    game->pacman->direction = RIGHT;
    game->pacman->speed = 450;
}

void set_Ghost_direction(Game *game, int ghost_id) {
    struct creature *ghost = game->ghosts[ghost_id];
    ghost->direction = (rand()) % 4 + 1;
}

Game *new_Game() {
    for (int x = 0; x < X; x++) {
        field[1][x] = WALL_CELL;
        field[Y - 1][x] = WALL_CELL;
        field[3][x] = WALL_CELL;
        field[4][x] = WALL_CELL;
        field[6][x] = WALL_CELL;
        field[7][x] = WALL_CELL;
        field[8][x] = WALL_CELL;
        field[10][x] = WALL_CELL;
        field[11][x] = WALL_CELL;
        field[13][x] = WALL_CELL;
        field[19][x] = WALL_CELL;
        field[20][x] = WALL_CELL;
        field[22][x] = WALL_CELL;
        field[23][x] = WALL_CELL;
        field[25][x] = WALL_CELL;
        field[26][x] = WALL_CELL;
        field[27][x] = WALL_CELL;
        field[28][x] = WALL_CELL;
        field[29][x] = WALL_CELL;
        field[31][x] = WALL_CELL;
    }
    field[2][0] = WALL_CELL;
    field[2][7] = WALL_CELL;
    field[2][8] = WALL_CELL;
    field[2][19] = WALL_CELL;
    field[2][20] = WALL_CELL;
    field[2][27] = WALL_CELL;

    field[3][1] = NOTHING_CELL;
    field[4][1] = NOTHING_CELL;
    field[3][6] = NOTHING_CELL;
    field[4][6] = NOTHING_CELL;
    field[3][9] = NOTHING_CELL;
    field[4][9] = NOTHING_CELL;
    field[3][18] = NOTHING_CELL;
    field[4][18] = NOTHING_CELL;
    field[3][21] = NOTHING_CELL;
    field[4][21] = NOTHING_CELL;
    field[3][26] = NOTHING_CELL;
    field[4][26] = NOTHING_CELL;
    field[5][0] = WALL_CELL;
    field[5][27] = WALL_CELL;
    field[6][3] = NOTHING_CELL;
    field[6][6] = NOTHING_CELL;
    field[6][12] = NOTHING_CELL;
    field[6][15] = NOTHING_CELL;
    field[6][21] = NOTHING_CELL;
    field[6][24] = NOTHING_CELL;
    field[7][0] = NOTHING_CELL;
    field[7][1] = NOTHING_CELL;
    field[7][3] = NOTHING_CELL;
    field[7][6] = NOTHING_CELL;
    field[7][12] = NOTHING_CELL;
    field[7][15] = NOTHING_CELL;
    field[7][21] = NOTHING_CELL;
    field[7][24] = NOTHING_CELL;
    field[7][26] = NOTHING_CELL;
    field[7][27] = NOTHING_CELL;
    field[8][3] = NOTHING_CELL;
    field[8][6] = NOTHING_CELL;
    field[8][12] = NOTHING_CELL;
    field[8][15] = NOTHING_CELL;
    field[8][21] = NOTHING_CELL;
    field[8][24] = NOTHING_CELL;
    field[9][4] = WALL_CELL;
    field[9][5] = WALL_CELL;
    field[9][13] = WALL_CELL;
    field[9][14] = WALL_CELL;
    field[9][22] = WALL_CELL;
    field[9][23] = WALL_CELL;
    field[10][3] = NOTHING_CELL;
    field[10][9] = NOTHING_CELL;
    field[10][18] = NOTHING_CELL;
    field[10][24] = NOTHING_CELL;
    field[11][0] = NOTHING_CELL;
    field[11][1] = NOTHING_CELL;
    field[11][3] = NOTHING_CELL;
    field[11][9] = NOTHING_CELL;
    field[11][18] = NOTHING_CELL;
    field[11][24] = NOTHING_CELL;
    field[11][26] = NOTHING_CELL;
    field[11][27] = NOTHING_CELL;
    field[12][2] = WALL_CELL;
    field[12][25] = WALL_CELL;
    field[13][0] = NOTHING_CELL;
    field[13][1] = NOTHING_CELL;
    field[13][3] = NOTHING_CELL;
    field[13][9] = NOTHING_CELL;
    field[13][13] = NOTHING_CELL;
    field[13][14] = NOTHING_CELL;
    field[13][18] = NOTHING_CELL;
    field[13][24] = NOTHING_CELL;
    field[13][26] = NOTHING_CELL;
    field[13][27] = NOTHING_CELL;
    field[14][2] = WALL_CELL;
    field[14][4] = WALL_CELL;
    field[14][5] = WALL_CELL;
    field[14][6] = WALL_CELL;
    field[14][7] = WALL_CELL;
    field[14][8] = WALL_CELL;
    field[14][10] = WALL_CELL;
    field[14][17] = WALL_CELL;
    field[14][19] = WALL_CELL;
    field[14][20] = WALL_CELL;
    field[14][21] = WALL_CELL;
    field[14][22] = WALL_CELL;
    field[14][23] = WALL_CELL;
    field[14][25] = WALL_CELL;
    field[15][2] = WALL_CELL;
    field[15][4] = WALL_CELL;
    field[15][5] = WALL_CELL;
    field[15][10] = WALL_CELL;
    field[15][17] = WALL_CELL;
    field[15][22] = WALL_CELL;
    field[15][23] = WALL_CELL;
    field[15][25] = WALL_CELL;
    field[16][2] = WALL_CELL;
    field[16][4] = WALL_CELL;
    field[16][5] = WALL_CELL;
    field[16][7] = WALL_CELL;
    field[16][8] = WALL_CELL;
    field[16][10] = WALL_CELL;
    field[16][17] = WALL_CELL;
    field[16][19] = WALL_CELL;
    field[16][20] = WALL_CELL;
    field[16][22] = WALL_CELL;
    field[16][23] = WALL_CELL;
    field[16][25] = WALL_CELL;
    field[17][0] = WALL_CELL;
    field[17][1] = WALL_CELL;
    field[17][2] = WALL_CELL;
    field[17][4] = WALL_CELL;
    field[17][5] = WALL_CELL;
    field[17][7] = WALL_CELL;
    field[17][8] = WALL_CELL;
    field[17][10] = WALL_CELL;
    field[17][11] = WALL_CELL;
    field[17][12] = WALL_CELL;
    field[17][13] = WALL_CELL;
    field[17][14] = WALL_CELL;
    field[17][15] = WALL_CELL;
    field[17][16] = WALL_CELL;
    field[17][17] = WALL_CELL;
    field[17][19] = WALL_CELL;
    field[17][20] = WALL_CELL;
    field[17][22] = WALL_CELL;
    field[17][23] = WALL_CELL;
    field[17][25] = WALL_CELL;
    field[17][26] = WALL_CELL;
    field[17][27] = WALL_CELL;
    field[18][7] = WALL_CELL;
    field[18][8] = WALL_CELL;
    field[18][19] = WALL_CELL;
    field[18][20] = WALL_CELL;
    field[19][3] = NOTHING_CELL;
    field[19][12] = NOTHING_CELL;
    field[19][15] = NOTHING_CELL;
    field[19][24] = NOTHING_CELL;
    field[20][0] = NOTHING_CELL;
    field[20][1] = NOTHING_CELL;
    field[20][3] = NOTHING_CELL;
    field[20][12] = NOTHING_CELL;
    field[20][15] = NOTHING_CELL;
    field[20][24] = NOTHING_CELL;
    field[20][26] = NOTHING_CELL;
    field[20][27] = NOTHING_CELL;
    field[21][2] = WALL_CELL;
    field[21][13] = WALL_CELL;
    field[21][14] = WALL_CELL;
    field[21][25] = WALL_CELL;
    field[22][0] = NOTHING_CELL;
    field[22][1] = NOTHING_CELL;
    field[22][3] = NOTHING_CELL;
    field[22][9] = NOTHING_CELL;
    field[22][18] = NOTHING_CELL;
    field[22][24] = NOTHING_CELL;
    field[22][26] = NOTHING_CELL;
    field[22][27] = NOTHING_CELL;
    field[23][3] = NOTHING_CELL;
    field[23][9] = NOTHING_CELL;
    field[23][18] = NOTHING_CELL;
    field[23][24] = NOTHING_CELL;
    field[24][0] = WALL_CELL;
    field[24][27] = WALL_CELL;
    field[25][1] = NOTHING_CELL;
    field[25][6] = NOTHING_CELL;
    field[25][12] = NOTHING_CELL;
    field[25][15] = NOTHING_CELL;
    field[25][21] = NOTHING_CELL;
    field[25][26] = NOTHING_CELL;
    field[26][1] = NOTHING_CELL;
    field[26][6] = NOTHING_CELL;
    field[26][12] = NOTHING_CELL;
    field[26][15] = NOTHING_CELL;
    field[26][21] = NOTHING_CELL;
    field[26][26] = NOTHING_CELL;
    field[27][1] = NOTHING_CELL;
    field[27][6] = NOTHING_CELL;
    field[27][9] = NOTHING_CELL;
    field[27][10] = NOTHING_CELL;
    field[27][11] = NOTHING_CELL;
    field[27][12] = NOTHING_CELL;
    field[27][15] = NOTHING_CELL;
    field[27][16] = NOTHING_CELL;
    field[27][17] = NOTHING_CELL;
    field[27][21] = NOTHING_CELL;
    field[27][26] = NOTHING_CELL;
    field[28][1] = NOTHING_CELL;
    field[28][6] = NOTHING_CELL;
    field[28][9] = NOTHING_CELL;
    field[28][18] = NOTHING_CELL;
    field[28][21] = NOTHING_CELL;
    field[28][26] = NOTHING_CELL;
    field[29][1] = NOTHING_CELL;
    field[29][6] = NOTHING_CELL;
    field[29][9] = NOTHING_CELL;
    field[29][18] = NOTHING_CELL;
    field[29][21] = NOTHING_CELL;
    field[29][26] = NOTHING_CELL;
    field[30][0] = WALL_CELL;
    field[30][27] = WALL_CELL;

    field[24][14] = PACMAN_CELL;
    field[12][13] = GHOST_CELL(0);
    field[15][11] = GHOST_CELL(1);
    field[15][16] = GHOST_CELL(2);
    field[14][14] = GHOST_CELL(3);

    field[15][15] = FOOD_CELL(SMALL);

    Game *game = (Game *)malloc(sizeof(Game));
    game->alive = 0;
    game->lives = 3;
    game->countdown.ms = 1000;
    game->countdown.n = 10;

    init_field(game, X, Y);
    int ghost_count = 0;
    game->pacman = (struct creature *)calloc(1, sizeof(struct creature));
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            game->field[x][y] = field[y][x];
            if (field[y][x].object == Ghost) {
                ghost_count++;
            } else if (field[y][x].object == Pacman) {
                game->pacman->start_position.x = x + 1;
                game->pacman->start_position.y = y + 1;
            }
        }
    }

    set_ghost_count(game, ghost_count);

    for (int i = 0; i < ghost_count; i++) {
        game->ghosts[i] = (struct creature *)calloc(1, sizeof(struct creature));
    }

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (field[y][x].object == Ghost) {
                game->ghosts[field[y][x].ghost_id]->start_position.x = x + 1;
                game->ghosts[field[y][x].ghost_id]->start_position.y = y + 1;
            }
        }
    }
    game->pacman->x = game->pacman->start_position.x;
    game->pacman->y = game->pacman->start_position.y;

    for (int i = 0; i < game->ghost_count; i++) {
        game->ghosts[i]->x = game->ghosts[i]->start_position.x;
        game->ghosts[i]->y = game->ghosts[i]->start_position.y;
    }
    set_level(game);
    init_countdown(game);
    start_countdown(game);

    return game;
}
