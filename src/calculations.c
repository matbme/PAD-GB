#include "calculations.h"

void calc_mandel(GLConfig *conf) {
    int i, j, iter, min, max;
    double x, y, zx, zy, zx2, zy2;
    unsigned short *hsv =
        malloc(conf->width * conf->height * sizeof(unsigned short));

    min = conf->max_iter;
    max = 0;
    for (i = 0; i < conf->height; i++) {
        y = (i - conf->height / 2) * conf->scale + conf->cy;
        for (j = 0; j < conf->width; j++) {
            x    = (j - conf->width / 2) * conf->scale + conf->cx;
            iter = 0;

            zx = hypot(x - .25, y);
            if (x < zx - 2 * zx * zx + .25)
                iter = conf->max_iter;
            if ((x + 1) * (x + 1) + y * y < 1 / 16)
                iter = conf->max_iter;

            zx  = 0;
            zy  = 0;
            zx2 = 0;
            zy2 = 0;
            while (iter < conf->max_iter && zx2 + zy2 < 4) {
                zy  = 2 * zx * zy + y;
                zx  = zx2 - zy2 + x;
                zx2 = zx * zx;
                zy2 = zy * zy;
                iter += 1;
            }
            if (iter < min)
                min = iter;
            if (iter > max)
                max = iter;
            hsv[(i * conf->width) + j] = iter;
        }
    }

    for (i = 0; i < conf->height; i += 1) {
        for (j = 0; j < conf->width; j += 1) {
            unsigned char *px = conf->tex + (((i * conf->width) + j) * 4);
            hsv_to_rgba(conf, hsv[(i * conf->width) + j], min, max, px);
        }
    }

    free(hsv);
}

void hsv_to_rgba(GLConfig *conf, int hue, int min, int max, unsigned char *px) {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    if (min == max)
        max = min + 1;
    if (conf->invert)
        hue = max - (hue - min);
    if (!conf->saturation) {
        r = 255 * (max - hue) / (max - min);
        g = r;
        b = r;
    } else {
        double h = fmod(
            conf->color_rotate + 1e-4 + 4.0 * (hue - min) / (max - min), 6);
        double c = VAL * conf->saturation;
        double X = c * (1 - fabs(fmod(h, 2) - 1));

        r = 0;
        g = 0;
        b = 0;

        switch ((int)h) {
        case 0:
            r = c;
            g = X;
            break;
        case 1:
            r = X;
            g = c;
            break;
        case 2:
            g = c;
            b = X;
            break;
        case 3:
            g = X;
            b = c;
            break;
        case 4:
            r = X;
            b = c;
            break;
        default:
            r = c;
            b = X;
            break;
        }
    }

    /* Using an alpha channel neatly solves the problem of aligning
     * rows on 4-byte boundaries (at the expense of memory, of
     * course). */
    px[0] = r;
    px[1] = g;
    px[2] = b;
    px[3] = 255; /* Alpha channel. */
}
