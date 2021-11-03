#pragma once


class Page {
public:
    virtual void render() {}
    virtual void mouse(float x, float y) {}
    virtual void keyboard_special(int key, int x, int y) {}
    virtual void keyboard(unsigned char key, int x, int y) {}
};
