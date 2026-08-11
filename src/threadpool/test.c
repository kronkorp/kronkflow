#include "kronkflow/macros/types.h"
#include "kronkflow/utils/threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "threadpool.h"

static void *__test(
    void *arg
)
{
    printf("%ld -> %d\n", (long int)arg, rand());
    return NULL;
}

[[gnu::constructor]]
static void seedrand(void)
{
    srand(time(NULL));
}

KF_API
void kfThreadPool_test(void)
{
    kfThreadPool *pool = kfThreadPool_create(8, NULL);

    kfThreadPool_pushTask(pool, __test, (void *)10);
    // kfThreadPool_join(pool);
    printf("Test threadpool\n");
    kfThreadPool_destroy(pool);
}
