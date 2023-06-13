#include "types.h"
#include <stdlib.h>

WorkQueue *workQueue_new() {
    WorkQueue *wq = malloc(sizeof(WorkQueue));
    wq->next      = NULL;
    wq->c         = NULL;
    return wq;
}

void workQueue_push(WorkQueue *wq, void *restrict val) {
    WorkQueue *w = wq;
    while (w->c != NULL)
        w = w->next;
    w->next = workQueue_new();
    w->c    = (void *)val;
}

void *restrict workQueue_pop(WorkQueue **wq) {
    void *c            = (*wq)->c;
    WorkQueue *next_wq = (*wq)->next;
    free(*wq);
    *wq = next_wq;
    return c;
}
