/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Get scheduler currentt tick
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/scheduler.h"
#include "scheduler.h"

PR_API
prTick kfScheduler_currentTick(
    const kfScheduler *sch
)
{
    if (!sch) {
        return 0;
    }
    return sch->tick;
}
