/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy scheduler private implementation
*/
#ifndef PROPHECY_SCHEDULER_IMPL_H
    #define PROPHECY_SCHEDULER_IMPL_H
    #include "../task/task.h"
    #include "kronkflow/task.h"
    #include <stddef.h>
    #include ""

typedef struct prophecy_stage_data_s {

    kfTask* tasks;
    bool passed;

} kfStageData;

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_scheduler_s
 *
 * @brief  scheduler definition
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_scheduler_s {

    kfTask  *tasks;  //!< The raw array of tasks
    size_t   size;   //!< The size of tasks raw array
    size_t   count;  //!< The number of tasks pushed
    kfTick   tick;   //!< The current tick (please tick scheduler at each loop)

    // TODO: Can move some tasks to arena. Maybe do it with a dynamic array ?
    

} kfScheduler;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Init a scheduler
 *
 * @param sch   The scheduler to init
 * @param size  The starting size of the scheduler
 */
///////////////////////////////////////////////////////////////////////////////
int kfScheduler_init(kfScheduler *sch, size_t size);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  Clear a scheduler
 *
 * @param sch  The scheduler to clear
 */
///////////////////////////////////////////////////////////////////////////////
void kfScheduler_clear(kfScheduler *sch);
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_SCHEDULER_IMPL_H */
