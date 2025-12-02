# Architecture Overview — Phase 1

This document describes the architecture implemented in **Phase 1** of the
*Smart Power‑Managed Sensor Hub* project for the STM32 Nucleo‑F446RE.

---

## 1. Layered Architecture

The firmware is organized into clear layers to keep responsibilities separated
and make future phases (sensors, power management, RTOS) easier to integrate.

### 1.1 Hardware / HAL Layer

- STM32CubeMX‑generated startup code and HAL drivers.
- Provides access to:
  - GPIO (LED)
  - UART2 (logging)
  - SysTick (millisecond tick via `HAL_GetTick()`)

All HAL‑specific details live in the `Core/` directory and are *not* directly
exposed to higher layers.

### 1.2 Common Utilities Layer

**Module:** `common/log.c` / `common/log.h`

Responsibilities:

- Initialize logging with a configured `UART_HandleTypeDef`.
- Provide formatted log output with:
  - Timestamp in milliseconds (`HAL_GetTick()`).
  - Log level tag (DBG / INF / WRN / ERR).
  - Source file name.
  - Line number.
  - Function name.

API highlights:

- `void Log_Init(UART_HandleTypeDef *huart);`
- `void Log_Print(LogLevel_t level, const char *file, uint32_t line,
                  const char *func, const char *fmt, ...);`
- Macros for convenience:
  - `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`.

This makes it easy to instrument any future module (sensors, power manager,
communication stack) without changing the logging internals.

### 1.3 Application Layer

The application layer currently consists of a **Task Manager** and the
application entry logic.

#### 1.3.1 Task Manager (`app_task_manager.c/.h`)

The Task Manager implements a simple cooperative scheduler.

Each task is described by:

```c
typedef void (*AppTaskFunction_t)(void);

typedef struct
{
    const char        *name;
    AppTaskFunction_t  function;
    uint32_t           period_ms;
    uint32_t           lastRun_ms;
} AppTaskDescriptor_t;
```

Key behavior:

- On initialization, the manager clears its task list and logs that it is ready.
- Tasks are registered via `AppTaskManager_RegisterTask()`.
- The main loop calls `AppTaskManager_RunOnce()` frequently.
- For each registered task, the scheduler checks:
  - `elapsed = now_ms - lastRun_ms`
  - If `elapsed >= period_ms`, the task is executed and `lastRun_ms` is updated.
- A debug log entry is printed whenever a task is run.

This pattern is deliberately similar to periodic tasks in an RTOS and will make
later migration to FreeRTOS straightforward.

#### 1.3.2 Application Main (`app_main.c/.h`)

Responsible for:

- High‑level application initialization.
- Registering all tasks with the Task Manager.
- Defining actual task functions.

In Phase 1, only a **Heartbeat** task is defined:

```c
static void App_TaskHeartbeat(void);
```

It is registered like so:

```c
static AppTaskDescriptor_t s_heartbeatTask =
{
    .name       = "Heartbeat",
    .function   = App_TaskHeartbeat,
    .period_ms  = 500U,
    .lastRun_ms = 0U
};
```

On each run:

- Toggles the on‑board LED (PA5).
- Logs an INFO‑level message indicating that it ran.

---

## 2. main.c Integration

`main.c` remains Cube‑generated but integrates your modules in the
`USER CODE` sections:

1. Includes:

```c
#include "log.h"
#include "app_main.h"
```

2. After HAL and peripherals are initialized:

```c
Log_Init(&huart2);
LOG_INFO("=== Smart Sensor Hub - Phase 1 startup ===");
App_MainInit();
```

3. In the main loop:

```c
while (1)
{
    App_MainLoop();
}
```

This keeps `main.c` as a thin shell that forwards control to your
application framework.

---

## 3. Extensibility for Later Phases

Phase 1’s design is intentionally minimal, but it already anticipates:

- **Sensor modules**  
  New tasks can be added for sensor sampling, with their own modules under
  a future `sensors/` or `drivers/` directory.

- **Power management**  
  A dedicated Power Manager task can control when the MCU enters low‑power
  modes based on system state, sampling rates, or external events.

- **Communication protocols**  
  A communication task can periodically package data and send it via UART,
  USB‑CDC, or a wireless module.

- **RTOS migration**  
  The current `AppTaskDescriptor_t` and periodic thinking can be mapped
  directly to FreeRTOS tasks and software timers in a later phase.

This layered architecture ensures that new features can be added without
introducing tight coupling between unrelated parts of the system.

---

## 4. Sensor Abstraction Layer (Phase 2)

Phase 2 introduces a **generic sensor interface** so that the application can
interact with sensors in a hardware-agnostic way.

### 4.1 Data & API Types

Defined in `sensors/sensor_if.h`:

- `SensorData_t`
  - `float value` — scalar measurement (e.g., temperature in °C).
  - `uint32_t timestamp` — time in ms (from `HAL_GetTick()`).

- `SensorIF_t`
  - `bool (*init)(void)` — initialize the sensor implementation.
  - `bool (*read)(SensorData_t *outData)` — perform a measurement.

- `const SensorIF_t *Sensor_GetInterface(void)`
  - Returns a pointer to the currently active sensor implementation.

### 4.2 Simulated Sensor Implementation

The first implementation is a **simulated temperature sensor** located in
`sensors/sensor_sim_temp.c`:

- Uses `HAL_GetTick()` to compute elapsed time since startup.
- Generates a synthetic temperature using a sine wave:
  - `T(t) = 25.0°C + 3.0°C * sin(t / 2000 ms)`.
- Fills `SensorData_t` with:
  - `value` — simulated temperature.
  - `timestamp` — current tick count in ms.

`Sensor_GetInterface()` currently returns this simulated interface, but in later
phases the same function can route to real I2C/SPI sensor drivers without
changing any application code.

### 4.3 Sensor Sampling Task

The application layer defines a **Sensor Sampling Task** in `app/app_main.c`:

- Registered with the Task Manager to run every 1000 ms.
- On each execution:
  1. Retrieves the active `SensorIF_t` via `Sensor_GetInterface()`.
  2. Calls `sensorIF->read(&data)`.
  3. Logs the reading using the logging subsystem.

This task forms the core of the data path for future phases:
- Real hardware sensors.
- Filtering or averaging.
- Anomaly detection or TinyML.
- Uplink over UART/USB-CDC/BLE.
