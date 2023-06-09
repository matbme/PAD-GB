#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include "types.h"

void keypress_handler(
    GLConfig *conf, unsigned char key, int x, int y, void set_texture(), void screen_dump()) {
    switch (key) {
    case 'q':
        glFinish();
        glutDestroyWindow(conf->gwin);
        break;

    case 27: // ESC
        conf->scale = 1. / 256;
        conf->cx    = -.6;
        conf->cy    = 0;
        set_texture();
        break;

    case 'r':
        conf->color_rotate = (conf->color_rotate + 1) % 6;
        set_texture();
        break;

    case '>':
    case '.':
        conf->max_iter += 128;
        if (conf->max_iter > 1 << 15)
            conf->max_iter = 1 << 15;
        printf("max iter: %d\n", conf->max_iter);
        set_texture();
        break;

    case '<':
    case ',':
        conf->max_iter -= 128;
        if (conf->max_iter < 128)
            conf->max_iter = 128;
        printf("max iter: %d\n", conf->max_iter);
        set_texture();
        break;

    case 'c':
        conf->saturation = 1 - conf->saturation;
        set_texture();
        break;

    case 's':
        screen_dump();
        break;

    case 'z':
        conf->max_iter = 4096;
        set_texture();
        break;

    case 'x':
        conf->max_iter = 128;
        set_texture();
        break;

    case ' ':
        conf->invert = !conf->invert;
        set_texture();
        break;

    default:
        set_texture();
        break;
    }
}
