#pragma once

#include <GL/freeglut.h>
#include <string.h>

#include "windowsize.h"
#include "directions.h"

#define FONT GLUT_BITMAP_9_BY_15
#define FONT_HEIGHT (15 / (float) window_height())
#define FONT_HEIGHT_UPPER_CASE (FONT_HEIGHT * 2)
#define FONT_WIDTH (9 / (float) window_width())
#define WHITE ((Color) {234, 220, 177})
#define GAME_BG ((Color) {42, 30, 120})
#define GOLD ((Color) {245, 238, 0})
#define RED ((Color) {255, 0, 0})
#define BLACK ((Color) {0, 0, 0})
typedef struct {
    GLubyte r;
    GLubyte g;
    GLubyte b;
} Color;


enum Food {
    SMALL,
    MEDIUM,
    LARGE
};


typedef struct Button Button;


void render_string(char *str, float x, float y, void *font, Color color);

void render_button(Button *btn, float x_center, float y_center, Color color_text, Color color_border);
int in_button(Button *btn, float x, float y);
Button *new_Button(char *text);
void *free_Button(Button *btn);

void render_Pacman(int size_px, float x_center, float y_center, Color color);
void render_Pacman_mouth(int size_px, float x_center, float y_center, Color color, Direction direction);

void render_Ghost(int size_px, float x_center, float y_center, Color color, Direction direction);

void render_Food(int size_px, float x_center, float y_center, Color color, enum Food food);

void render_Wall(int size_px, float x_center, float y_center, Color color);


