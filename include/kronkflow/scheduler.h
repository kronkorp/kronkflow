/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Definitions of structs and methods for prophecy lib
*/
#ifndef PROPHECY_SCHEDULER_H
    #define PROPHECY_SCHEDULER_H
    #include "kronkflow/macros/types.h"
    #include "kronkflow/macros/optimization.h"
    #include "kronkflow/task.h"
    #include <stdint.h>
    #include <stddef.h>
    #include <stdbool.h>
    /*
    ** This task scheduler will implemente a min - heap binary tree,
    ** in order to optimize ressources and checking.
    ** https://www.geeksforgeeks.org/c/c-program-to-implement-min-heap/
    */

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Forward declaration of kfScheduler type
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_scheduler_s kfScheduler;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Create a task scheduler
 *
 * @param size  Starting size of tasks scheduler
 * @return      Returns newly allocated kfScheduler, or NULL on error
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfScheduler *kfScheduler_create(size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Destroy the task scheduler
 *
 * @param sch  The scheduler to destroy
 */
///////////////////////////////////////////////////////////////////////////////
KF_API void kfScheduler_destroy(kfScheduler *sch);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Add a task to the scheduler
 *
 * @param sch       The scheduler to add a task
 * @param opt       The task options (handler, data, clearer)
 * @param delay     In how many ticks the task is gonna be executed
 * @param interval  If >0, the task will be reprogramming every <interval> ticks
 * @return          The id of the task added (>= 1), 0 if failed
 *
 * @note   Ids are per scheduler, in registration order. A periodic task keeps
 *         the same id every time it is reprogrammed. Tasks of a same stage due
 *         on the same tick run in the order of their ids.
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfTaskID kfScheduler_addTask(kfScheduler *sch, kfTaskOpt opt, kfTick delay, kfTick interval);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Remove a task that has not run yet (or a periodic one) from the scheduler
 *
 * The clearer of the task is called, exactly once, like when a task ends by
 * itself. It can be called from a handler, including by a task on its own id:
 * it then finishes its current run and is not reprogrammed.
 *
 * @param sch  The scheduler
 * @param id   The id given by kfScheduler_addTask
 * @return     kfTrue if the task was found and removed, kfFalse otherwise
 *             (unknown id, or one-shot task that already ran)
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfBool kfScheduler_removeTask(kfScheduler *sch, kfTaskID id);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Tick the scheduler
 *
 * @param sch      The scheduler to tick
 * @param context  The context to give to the task that are ready to exectute
 * @return         Returns the number of tasks executed this tick
 */
///////////////////////////////////////////////////////////////////////////////
KF_API size_t kfScheduler_tick(kfScheduler *sch, void *context);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Get the current tick of the scheduler
 *
 * @param sch  The scheduler to get tick of
 * @return     The current tick
 */
///////////////////////////////////////////////////////////////////////////////
KF_API kfTick kfScheduler_currentTick(const kfScheduler *sch);
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_SCHEDULER_H */
