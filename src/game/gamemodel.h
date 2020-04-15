#pragma once

#include "direction.h"

enum Food { SMALL = 1, ENERGIZER = 2, FRUIT = 3, EXTRALIVE = 4 };

struct cell // = Nothing | Wall | Food(Type) | Pacman | Ghost(Id int) |
            // Eaten_Food(type)
{
    enum {
        Nothing = 0,
        Wall = 1,
        Food = 2,
        Pacman = 3,
        Ghost = 4,
        Eaten_Food = 5
    } object;
    union {
        enum Food food_type;
        int ghost_id; // index in Game.ghosts [0, ghost_count]
    };
};

struct creature {
    // x in range [1; width]
    // y in range [1; height]
    // (0, 0) - not in field position
    int x;
    int y;
    Direction direction;
    int animation_status;
    int speed; // moves per minute
    struct cell under;
    struct {
        int x;
        int y;
    } start_position;
};

#define NOTHING_CELL ((struct cell){.object = Nothing, .ghost_id = -1})
#define PACMAN_CELL ((struct cell){.object = Pacman, .ghost_id = -1})
#define GHOST_CELL(id) ((struct cell){.object = Ghost, .ghost_id = (id)})
#define WALL_CELL ((struct cell){.object = Wall, .ghost_id = -1})
#define FOOD_CELL(type) ((struct cell){.object = Food, .food_type = (type)})

struct fruit {
    int x;
    int y;
    int when_food_count;
    int is_eaten;
    int moves_uneaten; // moves of pacman
};

typedef struct Game {
    int width;
    int height;
    float window_x; // bottom right corner
    float window_y;
    int alpha;       // = f(window, width, height)
    int px_creature; // = f(alpha)

    int ghost_count; // = f(field)

    int alive; // should the game be rendered, it's true if we are in game page
    int lives; // pacman lives, lives==0 <=> game ended

    int level; // == stage
    int difficalty; // - constant

    int fruits_count;
    struct fruit* fruits; // should be sorted by when_food_count

    struct {
        int x;
        int y;
        int when_level;
        int moves_uneaten; // pacman moves
        int moves_unvisible;
        int eaten;
    } extra_live;

    struct {
        int count_start;
        int count_now;
        int next_fruit_index;
    } foods;

    struct {
        int active;
        int runned; // is current tick runned
        int ms;     // countdown tick in ms
        int n;      // countdown count of ticks
        // when countdown_current_n > countdown_n countdown stops
        int current_n;
    } countdown;

    int pause;

    struct creature *pacman;
    struct creature **ghosts;

    struct cell **field;
} Game;

void start_countdown(Game *game);
void stop_countdown(Game *game);
void start_pause(Game *game);
void stop_pause(Game *game);
void check_fruits(Game *game);
int Game2file(Game *game, char *fname);
int file2Game(Game *game, char *fname);
void free_Game(Game *game);
void rebirth(Game *game);
void set_level(Game *game);