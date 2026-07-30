/*
** FREE PROJECT, 2026
** Kronkflow
** File description:
** threadpool
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "queue/queue.h"
#include "threadpool.h"
#include "kronkflow/threadpool.h"
#include <bits/pthreadtypes.h>
#include <prophecy/macros/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

kfThreadPool *kfThreadPool_create(
    ssize_t nthreads,
    void *ctx
)
{
    kfThreadPool *pool = NULL;

    if (nthreads == 0) {
        return NULL;
    } else if (nthreads == -1) {
        nthreads = sysconf(_SC_NPROCESSORS_ONLN);
        if (nthreads <= 0) {
            return NULL;
        }
    }
    pool = calloc(1, sizeof(kfThreadPool));
    if (!pool) {
        return NULL;
    }
    // NOTE: Nthread is already good, so init won't check...
    if (kfThreadPool_init(pool, nthreads, ctx) == -1) {
        free(pool);
        return NULL;
    }
    return pool;
}

static void *__routine(
    void *arg
)
{
    kfThreadPool *pool = (kfThreadPool *)arg;

    if (!pool) {
        return NULL;
    }
    while (true) {
        kfHandler handler;
        void *d;
        pthread_mutex_lock(&pool->mutex);
        while (!pool->stop && queue_empty(&pool->queue)) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        // FIXME: Queue empty really necessary ??
        if (pool->stop && queue_empty(&pool->queue)) {
            return NULL;
        }
        d = queue_front(&pool->queue);
        handler = (void *)((long int)d >> 8);
        queue_pop(&pool->queue);
        pool->pendings--;
        pthread_mutex_unlock(&pool->mutex);
        pool->runnings++;
        // NOTE: Should call handler... with ctx
        handler(pool->ctx, (void *)((long int)d & 0xff));
        pool->runnings--;
    }
}

int kfThreadPool_init(
    kfThreadPool *pool,
    size_t nthreads,
    void *ctx
)
{
    if (!pool) {
        return -1;
    }
    if (pthread_mutex_init(&pool->mutex, NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&pool->cond, NULL) != 0) {
        return -1;
    }
    pool->pendings = 0;
    pool->runnings = 0;
    pool->ctx = ctx;
    pool->workers = nthreads;
    pool->stop = kfFalse;
    pool->threads = calloc(nthreads, sizeof(pthread_t));
    if (!pool->threads) {
        return -1;
    }
    for (size_t i = 0; i < nthreads; ++i) {
        pthread_create(&pool->threads[i], NULL, &__routine, pool);
    }
    queue_init(&pool->queue);
    return 0;
}
