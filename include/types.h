#ifndef TYPES_H
#define TYPES_H

#include <GL/gl.h>

typedef struct _GLConfig {
    unsigned char *tex;
    int gwin;
    GLuint texture;
    int width, height;
    double scale;
    double cx;
    double cy;
    int color_rotate;
    int saturation;
    int invert;
    int max_iter;
} GLConfig;

#endif
