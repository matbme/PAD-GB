#ifndef CONTROLS_H
#define CONTROLS_H

#include "types.h"

void keypress_handler(
    GLConfig *conf, unsigned char key, int x, int y, void set_texture(), void screen_dump());

#endif
