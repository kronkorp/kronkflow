/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Clear a scheduler
*/
#include "../scheduler.h"
#include "dynarray.h"
#include <stddef.h>
#include <stdlib.h>

void kfScheduler_clear(
    kfScheduler *sch
)
{
    if (!sch) {
        return;
    }
    for (size_t i = 0; i < sch->count; ++i) {
        if (sch->tasks[i].clearer) {
            sch->tasks[i].clearer(sch->tasks[i].data);
        }
    }
    if (sch->tasks) {
        free(sch->tasks);
    }
    if (sch->staged) {
        for (size_t i = 0; i < kuDynarray_getLoad(sch->staged); ++i) {
            kuDynarray_free(kuDynarray_at(sch->staged, i));
        }
        kuDynarray_free(sch->staged);
    }
    sch->count = 0;
    sch->size = 0;
    sch->tick = 0;
}
