# Release Notes — Smart Power‑Managed Sensor Hub

This file tracks changes across phases of the project.

---

## v0.1.0 — Phase 1: Skeleton, Logging, Task Manager

**Date:** _Fill in with actual commit date (YYYY‑MM‑DD)_

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
