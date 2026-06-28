/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Destroy the scheduler
*/
#include "../scheduler.h"
#include "prophecy/macros/optimization.h"
#include <stdlib.h>

PR_API
void prScheduler_destroy(
    prScheduler *sch
)
{
    if (!sch) {
        return;
    }
    prScheduler_clear(sch);
    free(sch);
    return;
}
