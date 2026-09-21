/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** add a task to the scheduler
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "scheduler.h"
#include <stddef.h>
#include <stdlib.h>
#include "../minheap/minheap.h"
#include "kronkflow/task.h"

static int __kfScheduler_ensureCapacity(
    kfScheduler *sch
)
{
    kfTask *old = sch->tasks;
    size_t newSize = (sch->size == 0) ? 16 : sch->size * 2;

    sch->tasks = reallocarray(sch->tasks, newSize, sizeof(kfTask));
    if (!sch->tasks) {
        sch->tasks = old;
        return -1;
    }
    sch->size = newSize;
    return 0;
}

int kfScheduler_insertTask(
    kfScheduler *sch,
    const kfTask *task
)
{
    if (sch->count >= sch->size) {
        if (__kfScheduler_ensureCapacity(sch) == -1) {
            return -1;
        }
    }
    sch->tasks[sch->count] = *task;
    prMinHeap_add(sch, sch->count);
    ++sch->count;
    return 0;
}

KF_API
size_t kfScheduler_addTask(
    kfScheduler *sch,
    kfTaskOpt taskOptions,
    kfTick delay,
    kfTick interval
)
{
    kfTask task;

    if (!sch) {
        return 0;
    }
    task = kfTask_create(&taskOptions, delay, interval);
    task.id = sch->nextId;
    task.target = sch->tick + delay;
    if (kfScheduler_insertTask(sch, &task) == -1) {
        return 0;
    }
    return sch->nextId++;
}
