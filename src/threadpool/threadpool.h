/*
** FREE PROJECT, 2026
** Kronkflow
** File description:
** threadpool
*/
#ifndef KRONKFLOW_THREADPOOL_PRIVATE_H
    #define KRONKFLOW_THREADPOOL_PRIVATE_H
    #include <stdatomic.h>
    #include <pthread.h>
    #include <stddef.h>
    #include <stdbool.h>
    #include "kronkflow/macros/types.h"
    #include "queue/queue.h"

typedef void *(*kfThreadPoolHandler)(void *);

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronkflow_threadpool_s
 *
 * @brief  This struct is a dedicated threadpool for kronkflow multithreading features
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronkflow_threadpool_s {

    atomic_size_t   pendings;  //!< The number of task pendings
    atomic_size_t   runnings;  //!< The number of thread up and runnings
    atomic_size_t   workers;   //!< The number of threads
    pthread_t*      threads;   //!< The threads (array)
    pthread_cond_t  cond;      //!< The conditionnal variable
    pthread_mutex_t mutex;     //!< Mutex
    kfQueue         queue;     //!< Queue for tasks
    kfBool          stop;      //!< Does the pool should stop
    void*           ctx;       //!< The ctx to give...

} kfThreadPool;
///////////////////////////////////////////////////////////////////////////////

typedef struct kronkflow_thread_task_s {

    kfThreadPoolHandler handler;
    void*               data;

} kfThreadTask;

// TODO: Documentation
kfThreadPool *kfThreadPool_create(ssize_t nthreads, void *ctx);
int kfThreadPool_init(kfThreadPool *pool, size_t nthreads, void *ctx);
void kfThreadPool_destroy(kfThreadPool *pool);
void kfThreadPool_clear(kfThreadPool *pool);
void kfThreadPool_stop(kfThreadPool *pool);
size_t kfThreadPool_running(const kfThreadPool *pool);
size_t kfThreadPool_remaining(const kfThreadPool *pool);
int kfThreadPool_pushTask(kfThreadPool *pool, kfThreadPoolHandler task, void *data);

#endif /* KRONKFLOW_THREADPOOL_PRIVATE_H */
