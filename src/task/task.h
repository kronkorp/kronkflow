#include "prophecy/macros/types.h"
#include <stddef.h>

typedef struct prophecy_task_data_s {

    void      *data;
    prClearer  clearer;

} prTaskData;

// TODO: Add a cleanup function ptr.
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
    prTaskData data;      //!< The task data
    tick       interval;  //!< The interval (0 if ponctual, > 0 else)
    tick       target;     //!< The tick remainings.

} prTask;
///////////////////////////////////////////////////////////////////////////////

prTask prTask_create(prHandler handler, prTaskData data, tick delay, tick interval);
prTaskData prTask_data(void *data, prClearer clearer);
