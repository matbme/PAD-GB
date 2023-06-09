#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <stdlib.h>
#include <math.h>

#include "types.h"

#define VAL 255


void calc_mandel(GLConfig *conf);
void hsv_to_rgba(GLConfig *conf, int hue, int min, int max, unsigned char *px);

#endif
