/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a task from parameters
*/
#include "prophecy/macros/optimization.h"
#include "prophecy/macros/types.h"
#include "prophecy/task.h"
#include "task.h"

// DEBUG: Make sure opt is not NULL
PR_API
inline
prTask prTask_create(
    prTaskOpt *opt,
    prTick delay,
    prTick interval
)
{
    return (prTask){
        .id = 0,
        .handler = opt->handler,
        .data = opt->data,
        .clearer = opt->clearer,
        .target = delay,
        .interval = interval,
    };
}
