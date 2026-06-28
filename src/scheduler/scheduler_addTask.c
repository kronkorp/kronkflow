/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** add a task to the scheduler
*/
#include "scheduler.h"
#include <stddef.h>
#include <stdlib.h>
#include "../minheap/minheap.h"
#include "prophecy/task.h"

static int __prScheduler_ensureCapacity(
    prScheduler *sch
)
{
    prTask *old = sch->tasks;
    size_t newSize = (sch->size == 0) ? 16 : sch->size * 2;

    sch->tasks = reallocarray(sch->tasks, newSize, sizeof(prTask));
    if (!sch->tasks) {
        sch->tasks = old;
        return -1;
    }
    sch->size = newSize;
    return 0;
}

size_t prScheduler_addTask(
    prScheduler *sch,
    prTaskOpt task
)
{
    static size_t _id = 1;

    if (!sch) {
        return 0;
    }
    if (sch->count >= sch->size) {
        if (__prScheduler_ensureCapacity(sch) == -1) {
            return 0;
        }
    }
    sch->tasks[sch->count] = task; 
    sch->tasks[sch->count].id = _id;
    sch->tasks[sch->count].target = sch->tick + task.target;
    prMinHeap_add(sch, sch->count);
    ++sch->count;
    return _id++;
}
