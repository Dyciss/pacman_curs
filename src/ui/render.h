#pragma once

#include "direction.h"
#include "game/gamemodel.h"
#include "ui/windowsize.h"

#include <GL/freeglut.h>
#include <string.h>

#define FONT GLUT_BITMAP_9_BY_15
#define FONT_HEIGHT (15 / (float)window_height())
#define FONT_HEIGHT_UPPER_CASE (FONT_HEIGHT * 2)
#define FONT_WIDTH (9 / (float)window_width())
#define WHITE ((Color){234, 220, 177})
#define GAME_BG ((Color){0, 0, 0})
#define GOLD ((Color){255, 255, 100})
#define RED ((Color){255, 0, 0})
#define WALL_COLOR ((Color){64, 64, 64})
#define BLACK ((Color){0, 0, 0})

typedef struct {
    GLubyte r;
    GLubyte g;
    GLubyte b;
} Color;

typedef struct Button {
    // left-bottom
    float x1;
    float y1;

    // right-top
    float x2;
    float y2;

    char *text;
    size_t symbols; // strlen(text)
} Button;

// Input struct is the same
typedef Button Input;

void render_string(char *str, float x, float y, void *font, Color color);

void render_button(Button *btn, float x_center, float y_center,
                   Color color_text, Color color_border);
int in_button(Button *btn, float x, float y);
Button *new_Button(char *text);
void free_Button(Button *btn);

Input *new_Input(char *text, size_t max_symbols);
void free_Input(Input *inp);
void render_input(Input *inp, float x_center, float y_center, Color color_text,
                  Color color_border);
int in_input(Input *inp, float x, float y);

void render_Pacman(int size_px, float x_center, float y_center, Color color);
void render_Pacman_mouth(int size_px, float x_center, float y_center,
                         Color color, Direction direction);

void render_Ghost(int size_px, float x_center, float y_center, Color color,
                  Direction direction);
void render_Eyes(int size_px, float x_center, float y_center,
                 enum Direction direction);

void render_Food(int size_px, float x_center, float y_center, Color color,
                 enum Food food);

void render_Wall(int size_px, float x_center, float y_center, Color color);