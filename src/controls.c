#include "controls.h"

void keypress_handler(
    GLConfig *conf, unsigned char key, int x, int y, void set_texture()) {
    switch (key) {
    case 'q':
        glFinish();
        glutDestroyWindow(conf->gwin);
        break;

    case 27: // ESC
        conf->scale = 1. / 256;
        conf->cx    = -2.9;
        conf->cy    = -1.5;
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

    case 'z':
        conf->max_iter = 4096;
        set_texture();
        break;

    case 'x':
        conf->max_iter = 128;
        set_texture();
        break;

    case GLUT_KEY_LEFT: /* left */
        conf->cx -= 30 * conf->scale;
        set_texture();
        break;

    case GLUT_KEY_UP: /* up */
        conf->cy += 30 * conf->scale;
        set_texture();
        break;

    case GLUT_KEY_RIGHT: /* right */
        conf->cx += 30 * conf->scale;
        set_texture();
        break;

    case GLUT_KEY_DOWN: /* down */
        conf->cy -= 30 * conf->scale;
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

void mouseclick_handler(
    GLConfig *conf, int button, int state, int x, int y, void set_texture()) {
    if (state != GLUT_UP)
        return;

    conf->cx += (x - conf->width / 2.) * conf->scale;
    conf->cy -= (y - conf->height / 2.) * conf->scale;

    switch (button) {
    case GLUT_LEFT_BUTTON: /* zoom in */
        if (conf->scale > fabs((double)x) * 1e-16 &&
            conf->scale > fabs((double)y) * 1e-16)
            conf->scale /= 2;
        break;
    case GLUT_RIGHT_BUTTON: /* zoom out */
        conf->scale *= 2;
        break;
        /* any other button recenters */
    }

    set_texture();
}
