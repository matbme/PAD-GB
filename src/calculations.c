#include "calculations.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calc_mandel(struct thread_args *args) {
    GLConfig *conf = args->conf;

    while (1) {
        struct task *task = malloc(sizeof(struct task));
        int found_task    = 0;
        pthread_mutex_lock(args->mutex);
        for (int i = 0; i < args->total_tasks; i++) {
            if (IS_TASK(args->tasks[i])) {
                memcpy(task, &args->tasks[i], sizeof(struct task));
                CLEAR_TASK(args->tasks[i]);
                found_task = 1;
                break;
            }
        }
        pthread_mutex_unlock(args->mutex);

        if (!found_task) /* We're done */
            pthread_exit(0);

        int xmin = task->xmin;
        int xmax = task->xmax;
        int ymin = task->ymin;
        int ymax = task->ymax;

        int iter, min, max;
        double x, y, zx, zy, zx2, zy2;

        int width  = xmax - xmin + 1;
        int height = ymax - ymin + 1;

        unsigned short *hsv = malloc(width * height * sizeof(unsigned short));

        for (int i = ymin; i < ymax + 1; i++) {
            y = (i - (float)height / 2) * conf->scale + conf->cy;
            for (int j = xmin; j < xmax + 1; j++) {
                x    = (j - (float)width / 2) * conf->scale + conf->cx;
                iter = 0;

                zx = hypot(x - .25, y);
                if (x < zx - 2 * zx * zx + .25)
                    iter = conf->max_iter;
                if ((x + 1) * (x + 1) + y * y < 1. / 16)
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

                hsv[((i - ymin) * width) + (j - xmin)] = iter;
            }
        }

        unsigned char *ltex = calloc(width * height * 4, sizeof(unsigned char));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                unsigned char *px = ltex + (((i * width) + j) * 4);
                hsv_to_rgba(conf, hsv[(i * width) + j], 0, conf->max_iter, px);
            }
        }

        struct result *result = malloc(sizeof(struct result));
        result->tex           = ltex;
        result->xmin          = task->xmin;
        result->xmax          = task->xmax;
        result->ymin          = task->ymin;
        result->ymax          = task->ymax;
        pthread_mutex_lock(args->result_mutex);
        workQueue_push(args->wq, result);
        pthread_cond_signal(args->done);
        pthread_mutex_unlock(args->result_mutex);

        free(task);
        free(hsv);
    }
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
        double c = BRIGHTNESS * conf->saturation;
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
