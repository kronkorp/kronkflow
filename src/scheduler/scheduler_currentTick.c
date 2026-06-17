/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Get scheduler currentt tick
*/
#include "prophecy.h"

tick prScheduler_currentTick(const prScheduler *sch)
{
    if (!sch) {
        return 0;
    }
    return sch->tick;
}
