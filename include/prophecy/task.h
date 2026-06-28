/*
** FREE PROJECT, 2026
** PROPHECY
** File description:
** Prophecy scheduler implementation
*/
#ifndef PROPHECY_TASKS_H
    #define PROPHECY_TASKS_H
    #include "prophecy/macros/types.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct prophecy_task_opt_s
 *
 * @brief  Give the user the possibility to create its own task with parameters
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct prophecy_task_opt_s {

    prHandler handler;   //!< The task handler (function ptr)
    void*     data;      //!< The data to give to the handler
    prClearer clearer;   //!< The clearer of the data if allocated

} prTaskOpt;
///////////////////////////////////////////////////////////////////////////////

#endif /* PROPHECY_TASKS_H */
