/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Init the scheduler
*/
#include "prophecy.h"
#include <stdlib.h>

int prScheduler_init(prScheduler *sch, size_t size)
{
    if (!size) {
        return -1;
    }
    sch->tasks = calloc(size, sizeof(prTask));
    if (!sch->tasks) {
        return -1;
    }
    sch->size = size;
    sch->count = 0;
    sch->tick = 0;
    return 0;
}
