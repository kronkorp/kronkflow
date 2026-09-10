/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Init the scheduler
*/
#include "dynarray.h"
#include "kronkflow/scheduler.h"
#include "../../task/task.h"
#include "../scheduler.h"
#include "kronkflow/task.h"
#include <stdlib.h>

int kfScheduler_init(
    kfScheduler *sch,
    size_t size
)
{
    if (!size) {
        return -1;
    }
    sch->tasks = calloc(size, sizeof(kfTask));
    if (!sch->tasks) {
        return -1;
    }
    sch->size = size;
    sch->count = 0;
    sch->tick = 0;
    kuDynarray_init(&sch->staged, 2, kfTask *);
    return 0;
}
