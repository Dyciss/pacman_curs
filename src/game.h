#pragma once

#include <GL/freeglut.h>
#include "render.h"
#include "main.h"

void render_Game();
void mouse_Game(float x, float y);
void keyboard_special_Game(int key, int x, int y);

void init_Game();
void free_Game();