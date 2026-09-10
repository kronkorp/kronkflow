/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Create a task opt from parameters
*/
#include "kronkflow/macros/optimization.h"
#include "kronkflow/macros/types.h"
#include "kronkflow/task.h"

KF_API
inline
kfTaskOpt kfTask_opt(
    kfHandler handler,
    void *data,
    kfClearer clearer,
    kfStageId stageId,
    kfRWMasks rwmask
)
{
    return (kfTaskOpt){
        .handler = handler,
        .data = data,
        .clearer = clearer,
        .stage = stageId,
        .masks = rwmask
    };
}
