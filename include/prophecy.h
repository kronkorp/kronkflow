/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Definitions of structs and methods for prophecy lib
*/
#ifndef PROPHECY_H
    #define PROPHECY_H
    #include <stdint.h>
    #include <stddef.h>
    #include <stdbool.h>

typedef bool (*prHandler)(void *, void *);
typedef void (*prClearer)(void *);
typedef uint64_t tick;

/*
** This task scheduler will implemente a min - heap binary tree,
** in order to optimize ressources and checking.
** https://www.geeksforgeeks.org/c/c-program-to-implement-min-heap/
*/

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


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_scheduler_s
 *
 * @brief  scheduler definition
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_scheduler_s {

    prTask  *tasks;  //!< The raw array of tasks
    size_t   size;   //!< The size of tasks raw array
    size_t   count;  //!< The number of tasks pushed
    tick     tick;   //!< The current tick (please tick scheduler at each loop)

} prScheduler;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documention
prScheduler *prScheduler_create(size_t size);
void prScheduler_destroy(prScheduler *sch);
int prScheduler_init(prScheduler *sch, size_t size);
void prScheduler_clear(prScheduler *sch);

size_t prScheduler_addTask(prScheduler *sch, prTask task);
prTask prTask_create(prHandler handler, prTaskData data, tick delay, tick interval);

size_t prScheduler_tick(prScheduler *sch, void *context);

tick prScheduler_currentTick(const prScheduler *sch);

#endif /* PROPHECY_H */
