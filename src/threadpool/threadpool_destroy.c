/*
** FREE PROJECT, 2026
** Kronkflow
** File description:
** threadpool destroy
*/
#include "queue/queue.h"
#include "threadpool.h"
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>

void kfThreadPool_destroy(
    kfThreadPool *pool
)
{
    if (!pool)
        return;
    kfThreadPool_clear(pool);
    free(pool);
}

void kfThreadPool_clear(
    kfThreadPool *pool
)
{
    // pthread_mutex_lock(&pool->mutex);
    pool->stop = true;
    // pthread_mutex_unlock(&pool->mutex);
    for (size_t i = 0; i < pool->workers; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    queue_destroy(&pool->queue, NULL);
    free(pool->threads);
}
