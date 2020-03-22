#pragma once

#include "render.h"
#include "directions.h"

struct creature {
    // x in range [1; width]
    // y in range [1; height]
    // (0, 0) - not in field position
    int x;
    int y;
    float window_x;
    float window_y;
    Direction direction;
    int animation_status;
    int speed; // moves per minute
};

struct cell // = Nothing | Wall | Food(Type) | Pacman | Ghost(Id int)
{
    enum {
        Nothing=0,
        Wall,
        Food,
        Pacman,
        Ghost
    } object;
    union
    {
        enum Food food_type;
        int ghost_id; // index in Game.ghosts [0, ghost_count]
    };
    
};

#define NOTHING_CELL ((struct cell) {0})
#define PACMAN_CELL ((struct cell) {Pacman})

typedef struct Game
{
    int width;
    int height;
    float window_x; // bottom right corner
    float window_y;
    int alpha; // = f(window, width, height)
    int px_creature; // = f(alpha)

    int ghost_count; // = f(field)
    int alive;
    
    struct creature *pacman;
    struct creature **ghosts;
    
    struct cell **field;
} Game;

void map_xy_to_window_xy(int alpha, int x, int y, float *xw, float *yw);
void sync_sizing_props(Game *game);
Game *canonical_game();