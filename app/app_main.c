/**
 * @file app_main.c
 * @brief Application main logic for Smart Sensor Hub.
 */

#include "app_main.h"
#include "app_task_manager.h"
#include "log.h"
#include "stm32f4xx_hal.h"

/* Forward declaration of the heartbeat task function. */
static void App_TaskHeartbeat(void);

/**
 * @brief Descriptor for the heartbeat task.
 *
 * This task toggles the on-board LED and logs a message at
 * a fixed period, demonstrating that the scheduler and logging
 * are functioning correctly.
 */
static AppTaskDescriptor_t s_heartbeatTask =
{
    .name       = "Heartbeat",
    .function   = App_TaskHeartbeat,
    .period_ms  = 500U,
    .lastRun_ms = 0U
};

void App_MainInit(void)
{
    LOG_INFO("Application initialization started");

    /* Initialize task manager and register tasks. */
    AppTaskManager_Init();
    (void)AppTaskManager_RegisterTask(&s_heartbeatTask);

    LOG_INFO("Application initialization completed");
}

void App_MainLoop(void)
{
    /* Run scheduler once to process any ready tasks. */
    AppTaskManager_RunOnce();
}

/* ------------------------------------------------------------------------- */

static void App_TaskHeartbeat(void)
{
    /* This assumes the on-board LED is connected to GPIOA Pin 5 (Nucleo-64). */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    LOG_INFO("Heartbeat task toggled LED");
}
