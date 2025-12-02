/**
 * @file log.c
 * @brief Logging implementation for Smart Sensor Hub.
 */

#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief UART handle used for log output.
 *
 * This handle is set in Log_Init() and used internally by
 * Log_Print() to send data over serial.
 */
static UART_HandleTypeDef *s_logUart = NULL;

/**
 * @brief Maximum length of a single log line.
 */
#define LOG_MAX_MESSAGE_LENGTH   (256U)

/**
 * @brief Converts a LogLevel_t to a short string tag.
 *
 * @param level Logging level.
 * @return const char* Pointer to a static string.
 */
static const char *Log_LevelToString(LogLevel_t level);

/* ------------------------------------------------------------------------- */

void Log_Init(UART_HandleTypeDef *huart)
{
    s_logUart = huart;
}

void Log_Print(LogLevel_t level,
               const char *file,
               uint32_t line,
               const char *func,
               const char *fmt,
               ...)
{
    if (s_logUart == NULL)
    {
        /* Logging is not initialized yet, nothing to do. */
        return;
    }

    char buffer[LOG_MAX_MESSAGE_LENGTH];
    char logPrefix[96];

    /* Obtain system tick count for basic timestamping. */
    uint32_t timestamp_ms = HAL_GetTick();

    /* Compose a short prefix with timestamp, level, file, line, and function. */
    (void)snprintf(logPrefix,
                   sizeof(logPrefix),
                   "[%08lu ms][%s][%s:%lu][%s] ",
                   (unsigned long)timestamp_ms,
                   Log_LevelToString(level),
                   file,
                   (unsigned long)line,
                   func);

    /* Format the main log message using the variable arguments. */
    va_list args;
    va_start(args, fmt);
    int msgLen = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (msgLen < 0)
    {
        return;
    }

    /* Transmit prefix, then message, then newline. */
    (void)HAL_UART_Transmit(s_logUart,
                            (uint8_t *)logPrefix,
                            (uint16_t)strlen(logPrefix),
                            HAL_MAX_DELAY);

    (void)HAL_UART_Transmit(s_logUart,
                            (uint8_t *)buffer,
                            (uint16_t)strlen(buffer),
                            HAL_MAX_DELAY);

    const char newline[] = "\r\n";
    (void)HAL_UART_Transmit(s_logUart,
                            (uint8_t *)newline,
                            (uint16_t)strlen(newline),
                            HAL_MAX_DELAY);
}

static const char *Log_LevelToString(LogLevel_t level)
{
    switch (level)
    {
        case LOG_LEVEL_DEBUG: return "DBG";
        case LOG_LEVEL_INFO:  return "INF";
        case LOG_LEVEL_WARN:  return "WRN";
        case LOG_LEVEL_ERROR: return "ERR";
        default:              return "UNK";
    }
}
