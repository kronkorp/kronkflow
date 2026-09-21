/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Remove a task from the scheduler
*/
#include "dynarray.h"
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "scheduler.h"
#include "../minheap/minheap.h"
#include <stddef.h>

static kfBool remove_from_heap(
    kfScheduler *sch,
    kfTaskID id
)
{
    for (size_t i = 0; i < sch->count; ++i) {
        if (sch->tasks[i].id != id) {
            continue;
        }
        if (sch->tasks[i].clearer) {
            sch->tasks[i].clearer(sch->tasks[i].data);
        }
        prMinHeap_remove(sch, i);
        return kfTrue;
    }
    return kfFalse;
}

// NOTE: A staged task can't be erased while the tick iterates its bucket:
// it is flagged, and the tick skips it and calls its clearer.
static kfBool cancel_staged(
    kfScheduler *sch,
    kfTaskID id
)
{
    size_t first;

    for (size_t i = sch->curStage; i < kuDynarray_getLoad(sch->staged); ++i) {
        if (sch->staged[i] == NULL) {
            continue;
        }
        first = (i == sch->curStage) ? sch->curIdx : 0;
        for (size_t j = first; j < kuDynarray_getLoad(sch->staged[i]); ++j) {
            if (sch->staged[i][j].id == id && !sch->staged[i][j].cancelled) {
                sch->staged[i][j].cancelled = true;
                return kfTrue;
            }
        }
    }
    return kfFalse;
}

KF_API
kfBool kfScheduler_removeTask(
    kfScheduler *sch,
    kfTaskID id
)
{
    if (!sch || id == 0) {
        return kfFalse;
    }
    if (remove_from_heap(sch, id)) {
        return kfTrue;
    }
    if (sch->ticking) {
        return cancel_staged(sch, id);
    }
    return kfFalse;
}
