/*
** FREE PROJECT, 2026
** Kronkflow
** File description:
** threadpool
*/
#include "queue/queue.h"
#include "threadpool.h"
#include <pthread.h>
#include <stdlib.h>

int kfThreadPool_pushTask(
    kfThreadPool *pool,
    kfHandler task,
    void *data
)
{
    kfThreadTask *p = NULL;

    if (!pool) {
        return -1;
    }
    p = calloc(1, sizeof(kfThreadTask));
    if (!p) {
        return -1;
    }
    p->data = data;
    p->handler = task;
    pthread_mutex_lock(&pool->mutex);
    queue_push(&pool->queue, p);
    ++pool->pendings;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}
