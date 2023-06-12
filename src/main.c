#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calculations.h"
#include "controls.h"
#include "types.h"

#define THREADS 2

void set_texture();
void mouseclick(int button, int state, int x, int y);
void keypress(unsigned char key, int x, int y);

int dump = 1;
int old_width, old_height;

GLConfig conf = {.tex          = NULL,
                 .scale        = 1. / 512,
                 .cx           = -.6,
                 .cy           = 0,
                 .color_rotate = 0,
                 .saturation   = 1,
                 .invert       = 0,
                 .max_iter     = 256};

void render() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, conf.texture);

    glBegin(GL_QUADS);

    glTexCoord2d(0, 0);
    glVertex2i(0, 0);
    glTexCoord2d(1, 0);
    glVertex2i(conf.width, 0);
    glTexCoord2d(1, 1);
    glVertex2i(conf.width, conf.height);
    glTexCoord2d(0, 1);
    glVertex2i(0, conf.height);

    glEnd();

    glFlush();
    glFinish();
}

void screen_dump() {
    char fn[100];
    sprintf(fn, "screen%03d.ppm", dump++);
    FILE *fp = fopen(fn, "w");
    fprintf(fp, "P6\n%d %d\n255\n", conf.width, conf.height);
    for (int i = conf.height - 1; i >= 0; i -= 1) {
        for (int j = 0; j < conf.width; j += 1) {
            fwrite(&conf.tex[((i * conf.width) + j) * 4], 1, 3, fp);
        }
    }
    fclose(fp);
    printf("%s written\n", fn);
}

void alloc_tex() {
    if (conf.tex == NULL || conf.width != old_width ||
        conf.height != old_height) {
        free(conf.tex);
        conf.tex = malloc(conf.height * conf.width * 4 * sizeof(unsigned char));
        memset(conf.tex, 0,
               conf.height * conf.width * 4 * sizeof(unsigned char));
        printf("Tex new size: %d\n", conf.height * conf.width * 4);
        old_width  = conf.width;
        old_height = conf.height;
    }
}

void set_texture() {
    alloc_tex();

    if (conf.width == 0 || conf.height == 0)
        return;

    int block_width  = floor(conf.width / (float)THREADS);
    int block_height = floor(conf.height / (float)THREADS);

    int global_min = INT_MAX;
    int global_max = INT_MIN;

    pthread_t thrids[THREADS]        = {};
    struct mandel_args args[THREADS] = {};
    for (int i = 0; i < THREADS; i++) {
        pthread_barrier_t barrier;
        pthread_mutex_t mutex;

        pthread_barrier_init(&barrier, NULL, THREADS);
        pthread_mutex_init(&mutex, NULL);

        for (int j = 0; j < THREADS; j++) {
            args[j].conf  = &conf;
            args[j].xmin  = block_width * i;
            args[j].xmax  = block_width * (i + 1) - 1;
            args[j].ymin  = block_height * j;
            args[j].ymax  = block_height * (j + 1) - 1;
            args[j].global_min = &global_min;
            args[j].global_max = &global_max;
            args[j].barrier = &barrier;
            args[j].mutex = &mutex;

            pthread_create(&thrids[j], NULL, (void *)calc_mandel, &args[j]);
        }

        for (int t = 0; t < THREADS; t++)
            pthread_join(thrids[t], NULL);

        pthread_barrier_destroy(&barrier);
    }


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, conf.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, conf.width, conf.height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, conf.tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    render();
}

void keypress(unsigned char key, int x, int y) {
    keypress_handler(&conf, key, x, y, set_texture, screen_dump);
}

void mouseclick(int button, int state, int x, int y) {
    mouseclick_handler(&conf, button, state, x, y, set_texture);
}

void resize(int w, int h) {
    printf("resize %d %d\n", w, h);

    conf.width  = w;
    conf.height = h;

    glViewport(0, 0, w, h);
    glOrtho(0, w, 0, h, -1, 1);

    set_texture();
}

void init_gfx(GLConfig *conf, int *c, char **v) {
    glutInit(c, v);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(1280, 900);

    conf->gwin = glutCreateWindow("Mandelbrot");
    glutDisplayFunc(render);

    glutKeyboardFunc(keypress);
    glutMouseFunc(mouseclick);
    glutReshapeFunc(resize);
    glGenTextures(1, &conf->texture);
    set_texture();
}

int main(int argc, char **argv) {
    init_gfx(&conf, &argc, argv);
    printf("keys:\n\tr: color rotation\n\tc: monochrome\n\ts: screen dump\n\t"
           "<, >: decrease/increase max iteration\n\tq: quit\n\tmouse buttons "
           "to zoom\n");

    glutMainLoop();
    return 0;
}
