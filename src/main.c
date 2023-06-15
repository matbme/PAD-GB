#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "calculations.h"
#include "controls.h"
#include "types.h"

void render();
void set_texture();
void draw_texture(struct result *result);
void mouseclick(int button, int state, int x, int y);
void keypress(unsigned char key, int x, int y);
void special_keypress(int key, int x, int y);

int THREADS;

pthread_mutex_t mutex;
pthread_mutex_t result_mutex;
pthread_cond_t result_done;

int old_width, old_height;

GLConfig conf = {.scale        = 1. / 256,
                 .cx           = -2.9,
                 .cy           = -1.5,
                 .color_rotate = 0,
                 .saturation   = 1,
                 .invert       = 0,
                 .max_iter     = 256};

void render() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, conf.texture);

    glBegin(GL_QUADS);

    glTexCoord2d(0, 0);
    glVertex2i(conf.x_start, conf.y_start);
    glTexCoord2d(1, 0);
    glVertex2i(conf.x_end, conf.y_start);
    glTexCoord2d(1, 1);
    glVertex2i(conf.x_end, conf.y_end);
    glTexCoord2d(0, 1);
    glVertex2i(conf.x_start, conf.y_end);

    glEnd();

    glFlush();
    glFinish();
}

void draw_texture(struct result *result) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, conf.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result->xmax - result->xmin + 1,
                 result->ymax - result->ymin + 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 result->tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_TEXTURE_2D);

    conf.x_start = result->xmin;
    conf.x_end   = result->xmax + 1;
    conf.y_start = result->ymin;
    conf.y_end   = result->ymax + 1;
    render();
}

void set_texture() {
    if (conf.width == 0 || conf.height == 0)
        return;

    int block_width  = floor(conf.width / (float)THREADS);
    int block_height = floor(conf.height / (float)THREADS);

    struct task *tasks = malloc(THREADS * THREADS * sizeof(struct task));
    for (int i = 0; i < THREADS; i++) {
        for (int j = 0; j < THREADS; j++) {
            tasks[(THREADS * i) + j].xmin = block_width * i;
            tasks[(THREADS * i) + j].xmax = block_width * (i + 1) - 1;
            tasks[(THREADS * i) + j].ymin = block_height * j;
            tasks[(THREADS * i) + j].ymax = block_height * (j + 1) - 1;
        }
    }

    pthread_t *thrids       = malloc(THREADS * sizeof(pthread_t *));
    WorkQueue *wq           = workQueue_new();
    struct thread_args args = {.conf         = &conf,
                               .mutex        = &mutex,
                               .result_mutex = &result_mutex,
                               .tasks        = tasks,
                               .done         = &result_done,
                               .wq           = &wq,
                               .total_tasks  = THREADS * THREADS};
    for (int t = 0; t < THREADS; t++)
        pthread_create(&thrids[t], NULL, (void *)calc_mandel, &args);

    int total_tasks    = THREADS * THREADS;
    int complete_tasks = 0;
    struct result *result;
    while (1) {
        pthread_mutex_lock(&result_mutex);
        while (wq->c == NULL)
            pthread_cond_wait(&result_done, &result_mutex);

        result = workQueue_pop(&wq);
        draw_texture(result);
        pthread_mutex_unlock(&result_mutex);
        free(result->tex);
        free(result);

        if (++complete_tasks == total_tasks)
            break;
    }

    free(wq);
    free(thrids);
    free(tasks);
}

void keypress(unsigned char key, int x, int y) {
    keypress_handler(&conf, key, x, y, set_texture);
}

void special_keypress(int key, int x, int y) {
    keypress_handler(&conf, (unsigned char)key, x, y, set_texture);
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
    glutSpecialFunc(special_keypress);
    glutMouseFunc(mouseclick);
    glutReshapeFunc(resize);
    glGenTextures(1, &conf->texture);
    set_texture();
}

int main(int argc, char **argv) {
    THREADS = atoi(argv[1]);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&result_done, NULL);
    pthread_mutex_init(&result_mutex, NULL);

    init_gfx(&conf, &argc, argv);

    printf("Using %d threads.\n", THREADS);
    printf("keys:\n\tr: color rotation\n\tc: monochrome\n\t"
           "<, >: decrease/increase max iteration\n\tq: quit\n\tmouse buttons "
           "to zoom\n\tarrow keys: move around ");

    glutMainLoop();
    return 0;
}
