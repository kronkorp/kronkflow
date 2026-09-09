/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy scheduler implementation
*/
#ifndef PROPHECY_TASKS_H
    #define PROPHECY_TASKS_H
    #include "kronkflow/macros/optimization.h"
    #include "kronkflow/macros/types.h"
    #include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Forward declaration of kfTask
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_task_s kfTask;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_read_write_masks_s
 *
 * @brief  Read mask and write mask for multithreading
 *
 * @note   We're gonna compare tasks in the same stage to find out if we can
 *         run them in the same threadpool session.
 *         If read mask & read mask: yes. If read mask & write mask or write mask
 *         & write mask: no. 
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronkflow_read_write_masks_s {

    uint64_t rmask;  //!< The read mask
    uint64_t wmask;  //!< The write mask

} kfRWMasks;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_task_opt_s
 *
 * @brief  Give the user the possibility to create its own task with parameters
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_task_opt_s {

    kfHandler handler;   //!< The task handler (function ptr)
    void*     data;      //!< The data to give to the handler
    kfClearer clearer;   //!< The clearer of the data if allocated
    kfStageId stage;        //!< The stage ID
    kfRWMasks masks;     //!< The RW Masks

} kfTaskOpt;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Create task opts
 *
 * @param handler  The function ptr handler kfBool (*)(void *, void *)
 * @param data     The data to give to the handler
 * @param clearer  The clearer to clear data if allocated
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfTaskOpt kfTask_opt(kfHandler handler, void *data, kfClearer clearer);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Create a task
 *
 * @param data      All task options
 * @param delay     The delay
 * @param interval  The interval
 * @return          Returns the new task
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfTask kfTask_create(kfTaskOpt *data, kfTick delay, kfTick interval);
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_TASKS_H */
