#include <GL/freeglut.h>
#include "./ui/render.h"
#include "main.h"

char about_text[] = 
    "About program:\n"
    "There is smt interesting I think\n"
    "PACMAN!!";

Button *menu_btn;

void render_About() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_string(about_text, -1 + FONT_WIDTH, 1.0 - FONT_HEIGHT_UPPER_CASE, FONT,  WHITE);
    render_button(menu_btn, 0, 0, WHITE, WHITE);
}

void mouse_About(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    }
}

void init_About() {
    menu_btn = new_Button("Go to Menu");
}

void free_About() {
    free_Button(menu_btn);
}
