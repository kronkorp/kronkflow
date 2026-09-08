/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Destroy the scheduler
*/
#include "../scheduler.h"
#include "kronkflow/macros/optimization.h"
#include <stdlib.h>

KF_API
void kfScheduler_destroy(
    kfScheduler *sch
)
{
    if (!sch) {
        return;
    }
    kfScheduler_clear(sch);
    free(sch);
    return;
}
