/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Clear a scheduler
*/
#include "../scheduler.h"
#include <stddef.h>
#include <stdlib.h>

void prScheduler_clear(
    prScheduler *sch
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
    sch->count = 0;
    sch->size = 0;
    sch->tick = 0;
}
