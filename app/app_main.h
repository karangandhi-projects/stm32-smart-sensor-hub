/**
 * @file app_main.h
 * @brief Application entry points for Smart Sensor Hub.
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the application.
 *
 * This function sets up application-specific modules such as the
 * task manager and individual tasks.
 */
void App_MainInit(void);

/**
 * @brief Executes one iteration of the application main loop.
 *
 * This function should be called repeatedly from the main() loop.
 * It typically delegates to the task manager to run any tasks
 * that are due.
 */
void App_MainLoop(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_MAIN_H */
