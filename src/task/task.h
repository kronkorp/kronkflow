/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Definitions of structs and methods for prophecy lib
*/
#ifndef PROPHECY_TASK_IMPL_H
    #define PROPHECY_TASK_IMPL_H
    #include "prophecy/macros/types.h"
    #include "prophecy/task.h"
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
    prHandler  handler;   //!< The handler (callback)
    void*      data;      //!< The task data
    prClearer  clearer;   //!< The data clearer
    prTick     interval;  //!< The interval (0 if ponctual, > 0 else)
    prTick     target;    //!< The tick remainings.

} prTask;
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_TASK_IMPL_H */
