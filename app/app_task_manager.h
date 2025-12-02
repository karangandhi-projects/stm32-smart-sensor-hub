/**
 * @file app_task_manager.h
 * @brief Cooperative task manager for Smart Sensor Hub.
 *
 * This module implements a simple tick-based scheduler that
 * periodically calls registered tasks based on their configured
 * execution period. It is intended as a stepping stone toward
 * a full RTOS-based design in later phases.
 */

#ifndef APP_TASK_MANAGER_H
#define APP_TASK_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * @brief Function pointer type for a scheduled task.
 */
typedef void (*AppTaskFunction_t)(void);

/**
 * @brief Descriptor for a single scheduled task.
 */
typedef struct
{
    const char        *name;        /**< Human-readable task name.          */
    AppTaskFunction_t  function;    /**< Pointer to the task function.      */
    uint32_t           period_ms;   /**< Period of execution in milliseconds. */
    uint32_t           lastRun_ms;  /**< Last time the task was executed.   */
} AppTaskDescriptor_t;

/**
 * @brief Initializes the task manager.
 *
 * This function must be called once at startup, before the scheduler
 * is used. It resets all internal tracking variables.
 */
void AppTaskManager_Init(void);

/**
 * @brief Registers a task with the scheduler.
 *
 * Tasks must be registered before they will be executed. This
 * function should typically be called once for each task during
 * application initialization.
 *
 * @param task Pointer to a task descriptor. The descriptor must remain
 *             valid (in static or global storage) for the lifetime
 *             of the application.
 *
 * @return 0 on success, non-zero if the task list is full.
 */
int AppTaskManager_RegisterTask(AppTaskDescriptor_t *task);

/**
 * @brief Executes any tasks that are due to run.
 *
 * This function should be called frequently from the main loop. It
 * checks each registered task and executes it if its period has
 * elapsed since the last run.
 */
void AppTaskManager_RunOnce(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_MANAGER_H */
