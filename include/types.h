#ifndef TYPES_H
#define TYPES_H

#include <GL/gl.h>

typedef struct _GLConfig {
    int gwin;
    GLuint texture;
    int width, height;
    int x_start, x_end;
    int y_start, y_end;
    double scale;
    double cx, cy;
    int color_rotate;
    int saturation;
    int invert;
    int max_iter;
} GLConfig;

typedef struct _WorkQueue {
    struct _WorkQueue *next;
    void *c;
} WorkQueue;

WorkQueue *workQueue_new();
void workQueue_push(WorkQueue **wq, void *restrict val);
void *restrict workQueue_pop(WorkQueue **wq);

#endif
