/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Destroy the scheduler
*/
#include "prophecy.h"
#include <stdlib.h>

void prScheduler_destroy(prScheduler *sch)
{
    if (!sch) {
        return;
    }
    prScheduler_clear(sch);
    free(sch);
    return;
}
