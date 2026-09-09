/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Definitions of structs and methods for prophecy lib
*/
#ifndef PROPHECY_TASK_IMPL_H
    #define PROPHECY_TASK_IMPL_H
    #include "kronkflow/macros/types.h"
    #include "kronkflow/task.h"
    #include <stdint.h>
    #include <stddef.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_task_s
 *
 * @brief  task definition
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_task_s {

    size_t     id;        //!< The id of the tasks
    kfHandler  handler;   //!< The handler (callback)
    void*      data;      //!< The task data
    kfClearer  clearer;   //!< The data clearer
    kfTick     interval;  //!< The interval (0 if ponctual, > 0 else)
    kfTick     target;    //!< The tick remainings.

    kfStageId  stage;     //!< Stage id
    kfRWMasks  masks;     //!< Masks

} kfTask;
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_TASK_IMPL_H */
