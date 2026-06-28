/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a task opt from parameters
*/
#include "prophecy/macros/optimization.h"
#include "prophecy/macros/types.h"
#include "prophecy/task.h"

PR_API
inline
prTaskOpt prTask_opt(
    prHandler handler,
    void *data,
    prClearer clearer
)
{
    return (prTaskOpt){
        .handler = handler,
        .data = data,
        .clearer = clearer
    };
}
