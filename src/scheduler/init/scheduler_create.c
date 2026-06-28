/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a scheduler
*/
#include "../scheduler.h"
#include "prophecy/macros/optimization.h"
#include <stdlib.h>
#include <stddef.h>

PR_API
prScheduler *prScheduler_create(
    size_t size
)
{
    prScheduler *sch = calloc(1, sizeof(prScheduler));

    if (!sch) {
        return NULL;
    }
    if (prScheduler_init(sch, size) == -1) {
        free(sch);
        return NULL;
    }
    return sch;
}
