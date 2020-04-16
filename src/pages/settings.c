#include "settings/settings.h"
#include "main.h"
#include "pages/page.h"
#include "ui/render.h"

#include <ctype.h>
#include <string.h>

static Button *menu_btn = NULL;

static Input *difficalty_inp = NULL;
static Input *user_name_inp = NULL;
static Input *load_from_inp = NULL;

static Input *active_inp = NULL;

static Color input_color(Input *inp, enum setting_field f) {
    return inp == active_inp ? GOLD : validate_field(f) ? WHITE : RED;
}

static void update_page() {
    glClear(GL_COLOR_BUFFER_BIT);
    render_button(menu_btn, 1 - FONT_WIDTH * (menu_btn->symbols + 1),
                  -1 + FONT_HEIGHT_UPPER_CASE, WHITE, WHITE);

    char level_info[] = "Difficalty (0, 1, 2, 3): ";
    render_string(level_info, -2 * FONT_WIDTH * (sizeof level_info),
                  -FONT_HEIGHT, FONT, WHITE);

    render_input(difficalty_inp, FONT_WIDTH * (difficalty_inp->symbols), 0, WHITE,
                 input_color(difficalty_inp, Difficalty));

    char name_info[] = "Nickname: ";
    render_string(name_info, -2 * FONT_WIDTH * (sizeof name_info),
                  -2 * FONT_HEIGHT_UPPER_CASE - FONT_HEIGHT, FONT, WHITE);

    render_input(user_name_inp, FONT_WIDTH * (user_name_inp->symbols),
                 -2 * FONT_HEIGHT_UPPER_CASE, WHITE,
                 input_color(user_name_inp, User));

    char load_from[] = "Enter saved game/map: ";
    render_string(load_from, -2 * FONT_WIDTH * (sizeof load_from),
                  -4 * FONT_HEIGHT_UPPER_CASE - FONT_HEIGHT, FONT, WHITE);

    render_input(load_from_inp, FONT_WIDTH * (load_from_inp->symbols),
                 -4 * FONT_HEIGHT_UPPER_CASE, WHITE,
                 input_color(load_from_inp, Load_file));

    glutSwapBuffers();
}

static void render() { update_page(); }

static void mouse(float x, float y) {
    if (in_input(difficalty_inp, x, y)) {
        active_inp = difficalty_inp;
    } else if (in_input(user_name_inp, x, y)) {
        active_inp = user_name_inp;
    } else if (in_input(load_from_inp, x, y)) {
        active_inp = load_from_inp;
    } else {
        active_inp = NULL;

        if (in_button(menu_btn, x, y)) {
            set_program_state(Menu);
            return;
        }
    }
    update_page();
}

static void keyboard_special(int key, int x, int y) {}
static void keyboard(unsigned char key, int x, int y) {
    if (key == '\x1B') { //escape btn
        set_program_state(Menu);
        return;
    }

    if (active_inp == NULL)
        return;

    size_t len = strlen(active_inp->text);
    if (key == '\b' && len > 0) {
        active_inp->text[len - 1] = '\0';
    } else if (len < active_inp->symbols && !isspace(key) && isprint(key)) {
        active_inp->text[len] = key;
    }

    update_page();
}

static void bind_input(Input **inp, enum setting_field f) {
    struct field *field = settings_field(f);
    *inp = new_Input(field->text, field->max_len);
}

static void init() {
    menu_btn = new_Button("Menu");

    bind_input(&difficalty_inp, Difficalty);
    bind_input(&user_name_inp, User);
    bind_input(&load_from_inp, Load_file);
}

static void free_Settings() {
    free_Button(menu_btn);
    free_Input(difficalty_inp);
    free_Input(user_name_inp);
    free_Input(load_from_inp);
}

Page settings_Page() {
    return (Page){.render = render,
                  .mouse = mouse,
                  .keyboard = keyboard,
                  .keyboard_special = keyboard_special,
                  .init_Page = init,
                  .free_Page = free_Settings};
}