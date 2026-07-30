/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a task opt from parameters
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "kronkflow/task.h"

PR_API
inline
kfTaskOpt kfTask_opt(
    prHandler handler,
    void *data,
    prClearer clearer
)
{
    return (kfTaskOpt){
        .handler = handler,
        .data = data,
        .clearer = clearer
    };
}
