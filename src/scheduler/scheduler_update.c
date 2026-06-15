/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Update the scheduler
*/
#include "prophecy.h"
#include <stddef.h>
#include "../minheap/minheap.h"

size_t prScheduler_tick(prScheduler *sch, void *context)
{
    size_t done = 0;
    prTask ctask;
    bool r;

    if (!sch) {
        return 0;
    }
    sch->tick++;
    while (sch->count > 0 && sch->tasks[0].target <= sch->tick) {
        ctask = sch->tasks[0];
        prMinHeap_remove(sch, 0);
        r = ctask.handler(context, ctask.data.data);
        done++;
        if (r && ctask.interval > 0) {
            ctask.target = ctask.interval;
            prScheduler_addTask(sch, ctask);
        } else if (ctask.data.clearer) {
            ctask.data.clearer(ctask.data.data);
        }
    }
    return done;
}
