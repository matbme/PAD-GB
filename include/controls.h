#ifndef CONTROLS_H
#define CONTROLS_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "types.h"

void keypress_handler(GLConfig *conf,
                      unsigned char key,
                      int x,
                      int y,
                      void set_texture(),
                      void screen_dump());
void mouseclick_handler(
    GLConfig *conf, int button, int state, int x, int y, void set_texture());

#endif
