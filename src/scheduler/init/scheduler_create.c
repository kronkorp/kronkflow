/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a scheduler
*/
#include "../scheduler.h"
#include "kronkflow/macros/optimization.h"
#include <stdlib.h>
#include <stddef.h>

PR_API
kfScheduler *kfScheduler_create(
    size_t size
)
{
    kfScheduler *sch = calloc(1, sizeof(kfScheduler));

    if (!sch) {
        return NULL;
    }
    if (kfScheduler_init(sch, size) == -1) {
        free(sch);
        return NULL;
    }
    return sch;
}
