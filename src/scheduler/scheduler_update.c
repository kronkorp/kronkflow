/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Update the scheduler
*/
#include "dynarray.h"
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "kronkflow/task.h"
#include "scheduler.h"
#include <stddef.h>
#include <string.h>
#include "../task/task.h"
#include "../minheap/minheap.h"
#include "kronkflow/scheduler.h"

static void push_to_buckets(
    kfScheduler *sch,
    kfTask *task
)
{
    kfStageId stage = task->stage;
    kuDynarrayHeader *header;
    size_t oldLoad;

    if (stage >= kuDynarray_getSize(sch->staged)) {
        kuDynarray_resize(sch->staged, stage + 1);
    }
    header = kuDynarray_getHeader(sch->staged);
    if (stage >= header->load) {
        oldLoad = header->load;
        memset(sch->staged + oldLoad, 0,
            (stage + 1 - oldLoad) * sizeof(*sch->staged));
        header->load = stage + 1;
    }
    if (sch->staged[stage] == NULL) {
        kuDynarray_init(&sch->staged[stage], 2, kfTask);
    }
    kuDynarray_pushBack(sch->staged[stage], *task);
}

KF_API
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
    ++sch->tick;
    while (sch->count > 0 && sch->tasks[0].target <= sch->tick) {
        ctask = sch->tasks[0];
        prMinHeap_remove(sch, 0);

        push_to_buckets(sch, &ctask);
    }

    for (size_t i = 0; i < kuDynarray_getLoad(sch->staged); ++i) {
        for (size_t j = 0; j < kuDynarray_getLoad(sch->staged[i]); ++j) {
            ctask = sch->staged[i][j];
            r = ctask.handler(context, ctask.data);
            ++done;
            if (r && ctask.interval > 0) {
                ctask.target = ctask.interval;
                kfScheduler_addTask(sch, (kfTaskOpt){
                    ctask.handler,
                    ctask.data,
                    ctask.clearer,
                    ctask.stage,
                    ctask.masks
                }, ctask.interval, ctask.interval);
            } else if (ctask.clearer) {
                ctask.clearer(ctask.data);
            }
        }
        kuDynarray_clear(sch->staged[i]);
    }
    return done;
}
