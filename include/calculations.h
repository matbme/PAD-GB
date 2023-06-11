#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#include "types.h"

#define VAL 255

struct mandel_args {
    GLConfig *conf;
    int xmin, xmax;
    int ymin, ymax;
    pthread_mutex_t *mutex;
};

void calc_mandel(struct mandel_args *args);
void hsv_to_rgba(GLConfig *conf, int hue, int min, int max, unsigned char *px);

#endif
