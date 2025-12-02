# Release Notes — Smart Power‑Managed Sensor Hub

This file tracks changes across phases of the project.

---

## v0.1.0 — Phase 1: Skeleton, Logging, Task Manager

### ✅ Added

- Initial **STM32CubeIDE project** targeting the Nucleo‑F446RE board.
- **Logging subsystem** (`common/log.c`, `common/log.h`):
  - UART2‑based logging with timestamps.
  - Severity levels: DEBUG / INFO / WARN / ERROR.
  - Automatic inclusion of file name, line number, and function name.
- **Cooperative Task Manager** (`app/app_task_manager.c`, `app/app_task_manager.h`):
  - Registration of periodic tasks via `AppTaskManager_RegisterTask()`.
  - Execution of due tasks based on `HAL_GetTick()` timing.
- **Application entry logic** (`app/app_main.c`, `app/app_main.h`):
  - Initialization sequence with log messages.
  - Registration of a Heartbeat task.
- **Heartbeat task**:
  - Toggles the on‑board LED (PA5).
  - Logs a message each time it runs, demonstrating scheduler + logging.
- **Documentation**:
  - `docs/README.md` – project overview and usage.
  - `docs/ARCHITECTURE.md` – layered design explanation.
  - `docs/RELEASE_NOTES.md` – this file.

### ⚠️ Known Limitations

- No sensor interfaces have been implemented yet.
- No explicit power management (sleep/stop modes) is in use yet.
- No communication protocol beyond debug logging over UART2.
- No RTOS; only a cooperative scheduler is available.

These limitations are expected at Phase 1 and will be addressed in:

- **Phase 2** — simulated sensor and sampling task.
- **Phase 3** — power management building blocks.
- **Phase 4+** — communication stack and RTOS migration.

---

## v0.2.0 — Phase 2: Simulated Sensor & Sampling Task

### ✅ Added

- **Generic Sensor Interface** (`sensors/sensor_if.h`):
  - `SensorData_t` structure for scalar measurements and timestamps.
  - `SensorIF_t` function pointer-based interface (`init`, `read`).
  - `Sensor_GetInterface()` to obtain the active sensor implementation.

- **Simulated Temperature Sensor** (`sensors/sensor_sim_temp.c`, `sensors/sensor_sim_temp.h`):
  - Time-varying temperature signal based on a sine wave:
    - `T(t) = 25°C + 3°C * sin(t / 2000 ms)`
  - Uses `HAL_GetTick()` for timing.
  - Implements the generic `SensorIF_t` interface.

- **Sensor Sampling Task** (`app/app_main.c`):
  - New periodic task `App_TaskSensorSample()` running every 1000 ms.
  - Calls `sensorIF->read()` and logs structured data:
    - Sensor value in °C.
    - Timestamp in milliseconds.

- **Logging refinement** (`common/log.c`):
  - Fixed newline string declaration to remove GCC warning:
    - `const char newline[] = "\r\n";`

### ⚠️ Remaining Limitations

- Still using a cooperative scheduler (no RTOS yet).
- Only a single simulated temperature sensor is available.
- No explicit power management or low-power modes yet.
- No communication protocol beyond UART logging.

These will be addressed in:
- Phase 3 — power management scaffolding.
- Phase 4 — communication protocol and PC-side tools.
- Phase 5 — FreeRTOS migration and task monitoring.

