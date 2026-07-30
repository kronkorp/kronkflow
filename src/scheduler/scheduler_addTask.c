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

PR_API
size_t kfScheduler_addTask(
    kfScheduler *sch,
    kfTaskOpt taskOptions,
    prTick target,
    prTick interval
)
{
    static size_t _id = 1;

    if (!sch) {
        return 0;
    }
    if (sch->count >= sch->size) {
        if (__kfScheduler_ensureCapacity(sch) == -1) {
            return 0;
        }
    }
    sch->tasks[sch->count] = kfTask_create(&taskOptions, target, interval); 
    sch->tasks[sch->count].id = _id;
    sch->tasks[sch->count].target = sch->tick + target;
    prMinHeap_add(sch, sch->count);
    ++sch->count;
    return _id++;
}
