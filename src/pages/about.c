#include "./ui/render.h"
#include "main.h"
#include "page.h"
#include <GL/freeglut.h>

static char about_text[] = "About program:\n"
                    "There is smt interesting I think\n"
                    "PACMAN!!";

static Button *menu_btn = NULL;

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_string(about_text, -1 + FONT_WIDTH, 1.0 - FONT_HEIGHT_UPPER_CASE,
                  FONT, WHITE);
    render_button(menu_btn, 0, 0, WHITE, WHITE);
}

static void mouse(float x, float y) {
    if (in_button(menu_btn, x, y)) {
        set_program_state(Menu);
    }
}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {}

static void init_About() { menu_btn = new_Button("Go to Menu"); }

static void free_About() { free_Button(menu_btn); }

Page about_Page() {
    return (Page) {
        .render = render, .mouse = mouse, .keyboard = keyboard,
        .keyboard_special = keyboard_special, .init_Page = init_About,
        .free_Page = free_About
    };
}