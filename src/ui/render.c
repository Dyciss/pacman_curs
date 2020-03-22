#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>

#include "directions.h"
#include "render.h"
#include "windowsize.h"

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

void render_string(char *str, float x, float y, void *font, Color color) {
    glColor3ub(color.r, color.g, color.b);
    glRasterPos2f(x, y);
    glutBitmapString(font, (unsigned char *)str);
}

int in_button(Button *btn, float x, float y) {
    return (x > btn->x1) && (x < btn->x2) && (y > btn->y1) && (y < btn->y2);
}

Button *new_Button(char *text) {
    Button *btn = (Button *)malloc(sizeof(Button));
    btn->symbols = strlen(text);
    btn->text = text;
    return btn;
}

void *free_Button(Button *btn) { free(btn); }

void render_button(Button *btn, float x_center, float y_center,
                   Color color_text, Color color_border) {
    float x_text = x_center - btn->symbols * FONT_WIDTH;
    float y_text = y_center - FONT_HEIGHT / 2;
    render_string(btn->text, x_text, y_text, FONT, color_text);

    btn->x1 = x_text - FONT_WIDTH;
    btn->x2 = x_text + FONT_WIDTH + 2 * FONT_WIDTH * btn->symbols;
    btn->y1 = y_text - FONT_HEIGHT;
    btn->y2 = y_text + FONT_HEIGHT_UPPER_CASE;

    glColor3ub(color_border.r, color_border.g, color_border.b);
    glBegin(GL_LINE_LOOP);
    glVertex2f(btn->x1, btn->y1);
    glVertex2f(btn->x2, btn->y1);
    glVertex2f(btn->x2, btn->y2);
    glVertex2f(btn->x1, btn->y2);
    glEnd();
}

void render_Circle(int size_px, float x_center, float y_center, Color color) {
    glColor3ub(color.r, color.g, color.b);
    int n = (size_px / 2 - 1) / 2;
    int y = 0;
    for (int i = n; i > 0; i--, y++) {
        glBegin(GL_LINES);
        glVertex2f(x_center - (size_px / 2.0 - i) / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glVertex2f(x_center + (size_px / 2.0 - i) / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glEnd();
    }
    for (int i = 0; i < size_px - 2 * n; i++, y++) {
        glBegin(GL_LINES);
        glVertex2f(x_center - size_px / 2.0 / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glVertex2f(x_center + size_px / 2.0 / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glEnd();
    }
    for (int i = 1; i <= n; i++, y++) {
        glBegin(GL_LINES);
        glVertex2f(x_center - (size_px / 2.0 - i) / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glVertex2f(x_center + (size_px / 2.0 - i) / (float)window_width(),
                   y_center + (size_px / 2.0 - y) / (float)window_height());
        glEnd();
    }
}

void render_Pacman(int size_px, float x_center, float y_center, Color color) {
    render_Circle(size_px, x_center, y_center, color);
}
void render_Pacman_mouth(int size_px, float x_center, float y_center,
                         Color color, Direction direction) {
    glColor3ub(color.r, color.g, color.b);
    int n = (size_px / 2 - 1) / 2;
    if (direction == RIGHT) {
        int i = 2;
        int y = n;
        while (i <= 2 * n) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - i) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glEnd();
            i += 2;
            y++;
        }
        i -= 2; // end of last while
        if (size_px % 2 == 1)
            i -= 2; // trick with pixels (even/odd)
        while (i >= 2) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - i) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glEnd();
            i -= 2;
            y++;
        }
    }
    if (direction == LEFT) {
        int i = 2;
        int y = n;
        while (i <= 2 * n) {
            glBegin(GL_LINES);
            glVertex2f(x_center - (size_px / 2.0 - i) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glVertex2f(x_center - (size_px / 2.0) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glEnd();
            i += 2;
            y++;
        }
        i -= 2; // end of last while
        if (size_px % 2 == 1)
            i -= 2; // trick with pixels (even/odd)
        while (i >= 2) {
            glBegin(GL_LINES);
            glVertex2f(x_center - (size_px / 2.0 - i) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glVertex2f(x_center - (size_px / 2.0) / (float)window_width(),
                       y_center + (size_px / 2.0 - y) / (float)window_height());
            glEnd();
            i -= 2;
            y++;
        }
    }
    if (direction == BOTTOM) {
        int i = 2;
        int y = n;
        while (i <= 2 * n) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center - (size_px / 2.0 - i) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center - (size_px / 2.0) / (float)window_height());
            glEnd();
            i += 2;
            y++;
        }
        i -= 2; // end of last while
        if (size_px % 2 == 1)
            i -= 2; // trick with pixels (even/odd)
        while (i >= 2) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center - (size_px / 2.0 - i) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center - (size_px / 2.0) / (float)window_height());
            glEnd();
            i -= 2;
            y++;
        }
    }
    if (direction == TOP) {
        int i = 2;
        int y = n;
        while (i <= 2 * n) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center + (size_px / 2.0 - i) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center + (size_px / 2.0) / (float)window_height());
            glEnd();
            i += 2;
            y++;
        }
        i -= 2; // end of last while
        if (size_px % 2 == 1)
            i -= 2; // trick with pixels (even/odd)
        while (i >= 2) {
            glBegin(GL_LINES);
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center + (size_px / 2.0 - i) / (float)window_height());
            glVertex2f(x_center + (size_px / 2.0 - y) / (float)window_width(),
                       y_center + (size_px / 2.0) / (float)window_height());
            glEnd();
            i -= 2;
            y++;
        }
    }
}

void render_Eyes(int size_px, float x_center, float y_center,
                 enum Direction direction) {
    render_Circle(
        size_px * 0.3, x_center - (size_px / 4) / (float)window_width(),
        y_center + (size_px * 0.2) / (float)window_height(), WHITE); // EYES
    render_Circle(
        size_px * 0.3, x_center + (size_px / 4) / (float)window_width(),
        y_center + (size_px * 0.2) / (float)window_height(), WHITE); // EYES

    if (direction == BOTTOM) {
        render_Circle(
            0.15 * size_px, x_center - (size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.1) / (float)window_height(), BLACK);
        render_Circle(
            0.15 * size_px, x_center + (size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.1) / (float)window_height(), BLACK);
    }
    if (direction == TOP) {
        render_Circle(
            0.15 * size_px, x_center - (size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.3) / (float)window_height(), BLACK);
        render_Circle(
            0.15 * size_px, x_center + (size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.3) / (float)window_height(), BLACK);
    }
    if (direction == RIGHT) {
        render_Circle(
            0.15 * size_px,
            x_center + (size_px * 0.1 + size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.2) / (float)window_height(), BLACK);
        render_Circle(
            0.15 * size_px, x_center - (size_px * 0.15) / (float)window_width(),
            y_center + (size_px * 0.2) / (float)window_height(), BLACK);
    }
    if (direction == LEFT) {
        render_Circle(
            0.15 * size_px,
            x_center - (size_px * 0.1 + size_px / 4) / (float)window_width(),
            y_center + (size_px * 0.2) / (float)window_height(), BLACK);
        render_Circle(
            0.15 * size_px, x_center + (size_px * 0.15) / (float)window_width(),
            y_center + (size_px * 0.2) / (float)window_height(), BLACK);
    }
}

void render_Ghost(int size_px, float x_center, float y_center, Color color,
                  Direction direction) {
    glColor3ub(color.r, color.g, color.b);
    int y = (int)(y_center * window_height() + size_px / 2);
    for (int i = size_px / 5; i > 0; i--) {
        glBegin(GL_LINES);
        glVertex2f(x_center - (size_px / 2 - i) / (float)window_width(),
                   y / (float)window_height());
        glVertex2f(x_center + (size_px / 2 - i) / (float)window_width(),
                   y / (float)window_height());
        glEnd();
        y--;
    }
    for (y; y > (int)(y_center * window_height() - size_px * 0.6 / 2); y--) {
        glBegin(GL_LINES);
        glVertex2f(x_center - (size_px / 2) / (float)window_width(),
                   y / (float)window_height());
        glVertex2f(x_center + (size_px / 2) / (float)window_width(),
                   y / (float)window_height());
        glEnd();
    }
    glBegin(GL_TRIANGLES);
    glVertex2f(x_center - (size_px / 2) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center - (size_px / 2 - size_px / 4) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center - (size_px / 2) / (float)window_width(),
               y_center - (size_px / 2) / (float)window_height());

    glVertex2f(x_center - (1.3 * size_px / 8) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center + (1.3 * size_px / 8) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center, y_center - (size_px / 2) / (float)window_height());

    glVertex2f(x_center + (size_px / 2) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center + (size_px / 2 - size_px / 4) / (float)window_width(),
               y_center - (0.3 * size_px) / (float)window_height());
    glVertex2f(x_center + (size_px / 2) / (float)window_width(),
               y_center - (size_px / 2) / (float)window_height());
    glEnd();
    render_Eyes(size_px, x_center, y_center, direction);
}
void render_Food(int size_px, float x_center, float y_center, Color color,
                 enum Food food) {
    glColor3ub(color.r, color.g, color.b);
    if (food == SMALL) {
        glBegin(GL_POLYGON);
        glVertex2f(x_center - (size_px / 7) / (float)window_width(),
                   y_center + (size_px / 7) / (float)window_height());
        glVertex2f(x_center + (size_px / 7) / (float)window_width(),
                   y_center + (size_px / 7) / (float)window_height());
        glVertex2f(x_center + (size_px / 7) / (float)window_width(),
                   y_center - (size_px / 7) / (float)window_height());
        glVertex2f(x_center - (size_px / 7) / (float)window_width(),
                   y_center - (size_px / 7) / (float)window_height());
        glEnd();
    } else if (food == MEDIUM) {
        render_Circle(size_px / 2, x_center, y_center, color);
    } else if (food == LARGE) {
        render_Circle(size_px * 0.3,
                      x_center + (size_px / 5) / (float)window_width(),
                      y_center + (size_px / 5) / (float)window_height(), color);
        render_Circle(size_px * 0.3,
                      x_center + (size_px / 5) / (float)window_width(),
                      y_center - (size_px / 5) / (float)window_height(), color);
        render_Circle(size_px * 0.3,
                      x_center - (size_px / 5) / (float)window_width(),
                      y_center - (size_px / 5) / (float)window_height(), color);
        render_Circle(size_px * 0.3,
                      x_center - (size_px / 5) / (float)window_width(),
                      y_center + (size_px / 5) / (float)window_height(), color);
    }
}
void render_Wall(int size_px, float x_center, float y_center, Color color) {
    render_Circle(size_px, x_center, y_center, RED);
    size_px -= size_px / 4;
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_POLYGON);
    glVertex2f(x_center - (size_px / 2) / (float)window_width(),
               y_center + (size_px / 2) / (float)window_height());
    glVertex2f(x_center + (size_px / 2) / (float)window_width(),
               y_center + (size_px / 2) / (float)window_height());
    glVertex2f(x_center + (size_px / 2) / (float)window_width(),
               y_center - (size_px / 2) / (float)window_height());
    glVertex2f(x_center - (size_px / 2) / (float)window_width(),
               y_center - (size_px / 2) / (float)window_height());
    glEnd();
}