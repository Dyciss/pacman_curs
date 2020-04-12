#pragma once

typedef struct {
    void (*render)();
    void (*mouse)(float x, float y);
    void (*keyboard_special)(int key, int x, int y);
    void (*keyboard)(unsigned char key, int x, int y);
    void (*init_Page)();
    void (*free_Page)();
} Page;