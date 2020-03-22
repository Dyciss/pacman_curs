#pragma once

#include "directions.h"

enum Food {
    SMALL,
    MEDIUM,
    LARGE
};

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

void set_ghost_count(Game *game, int count);
void init_field(Game *game, int width, int height);