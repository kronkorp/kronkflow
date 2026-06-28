/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Get scheduler currentt tick
*/
#include "prophecy/macros/optimization.h"
#include "prophecy/scheduler.h"
#include "scheduler.h"

PR_API
prTick prScheduler_currentTick(
    const prScheduler *sch
)
{
    if (!sch) {
        return 0;
    }
    return sch->tick;
}
