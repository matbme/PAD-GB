#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#include "types.h"

#define VAL 255

#define IS_TASK(t) \
    (t.xmin == -1 && t.xmax == -1 && t.ymin == -1 && t.ymax == -1 ? 0 : 1)
#define CLEAR_TASK(t) \
    ({t.xmin = -1; t.xmax = -1; t.ymin = -1; t.ymax = -1;})
#define CLONE_TASK(t) \
    ({xmin = t.xmin, xmax = t.xmax, ymin = t.ymin, ymax = t.ymax})

struct task {
    int xmin, xmax;
    int ymin, ymax;
};

struct result {
    int xmin, xmax;
    int ymin, ymax;
    unsigned char *tex;
};

struct thread_args {
    GLConfig *conf;
    pthread_mutex_t *mutex;
    pthread_mutex_t *result_mutex;
    pthread_cond_t *done;
    int total_tasks;
    struct task *tasks;
    WorkQueue **wq;
};

void calc_mandel(struct thread_args *args);
void hsv_to_rgba(GLConfig *conf, int hue, int min, int max, unsigned char *px);

#endif
