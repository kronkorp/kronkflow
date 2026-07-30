/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a task from parameters
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "kronkflow/task.h"
#include "task.h"

// DEBUG: Make sure opt is not NULL
PR_API
inline
kfTask kfTask_create(
    kfTaskOpt *opt,
    prTick delay,
    prTick interval
)
{
    return (kfTask){
        .id = 0,
        .handler = opt->handler,
        .data = opt->data,
        .clearer = opt->clearer,
        .target = delay,
        .interval = interval,
    };
}
