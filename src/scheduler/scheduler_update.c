/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Update the scheduler
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/task.h"
#include "scheduler.h"
#include <stddef.h>
#include "../task/task.h"
#include "../minheap/minheap.h"
#include "kronkflow/scheduler.h"

PR_API
size_t kfScheduler_tick(
    kfScheduler *sch,
    void *context
)
{
    size_t done = 0;
    kfTask ctask;
    bool r;

    if (!sch) {
        return 0;
    }
    sch->tick++;
    while (sch->count > 0 && sch->tasks[0].target <= sch->tick) {
        ctask = sch->tasks[0];
        prMinHeap_remove(sch, 0);
        r = ctask.handler(context, ctask.data);
        done++;
        if (r && ctask.interval > 0) {
            ctask.target = ctask.interval;
            kfScheduler_addTask(sch, (kfTaskOpt){ctask.handler, ctask.data, ctask.clearer}, ctask.interval, ctask.interval);
        } else if (ctask.clearer) {
            ctask.clearer(ctask.data);
        }
    }
    return done;
}
