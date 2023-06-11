#ifndef TYPES_H
#define TYPES_H

#include <GL/gl.h>

typedef struct _GLConfig {
    unsigned char *tex;
    int gwin;
    GLuint texture;
    int width, height;
    double scale;
    double cx, cy;
    int color_rotate;
    int saturation;
    int invert;
    int max_iter;
} GLConfig;

typedef struct _ThreadConfig {
    int max_threads;
    /* TODO: Render strategy:
     *   - Squares with same size
     *   - Squares with fixed size
     *   - Lines
     *   - As whole image (calculate in parallel, display all at once)
    */
} ThreadConfig;

#endif
