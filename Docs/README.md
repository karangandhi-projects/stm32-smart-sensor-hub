# Smart Power-Managed Sensor Hub (STM32F446RE)

This repository contains the **Phase 1** implementation of a modular embedded firmware project targeting the STM32 Nucleo-F446RE board.

The long‑term goal is to build a **Smart Power‑Managed Sensor Hub** that:
- Integrates multiple sensors over I2C/SPI/UART/etc.
- Uses structured power management (sleep/stop modes, wake sources).
- Exposes data over a communication interface (UART/USB‑CDC/BLE).
- Demonstrates production‑style embedded architecture (layers, drivers, logging, RTOS).

Phase 1 is focused purely on **infrastructure**:
- A clean project structure.
- A cooperative **Task Manager** (mini scheduler).
- A robust **logging system** over UART2.
- A simple **heartbeat task** to prove the flow.

---

## 🚀 Phase 1 – Features

Phase 1 implements:

- **UART2 logging**  
  - Timestamped messages using `HAL_GetTick()`.  
  - Log levels: DEBUG / INFO / WARN / ERROR.  
  - Automatic metadata: file name, line number, function name.

- **Cooperative Task Manager**  
  - Simple periodic scheduling using `HAL_GetTick()`.  
  - Each task has a name, function pointer, period, and last-run timestamp.  
  - Easy to extend with more tasks in later phases.

- **Heartbeat Task**  
  - Toggles the Nucleo on‑board LED (PA5).  
  - Prints a log line every time it runs.  
  - Serves as a sanity check that scheduler + logging work.

- **Docs & Release Notes**  
  - This README.  
  - High‑level architecture description.  
  - Versioned release notes starting at `v0.1.0`.

---

## 🧱 Repository Structure (Phase 1)

```text
Core/                 # STM32CubeIDE-generated startup + HAL code
  Inc/
  Src/
app/
  app_main.c          # Application entry points (init + loop)
  app_main.h
  app_task_manager.c  # Cooperative task scheduler
  app_task_manager.h
common/
  log.c               # Logging implementation (UART2-based)
  log.h
docs/
  README.md           # Project overview & usage
  ARCHITECTURE.md     # Layered architecture and design notes
  RELEASE_NOTES.md    # Version history for each phase
```

This structure keeps Cube‑generated code in `Core/` and your custom application logic in `app/` and `common/`.

---

## 🛠️ Build & Run

1. Open the project in **STM32CubeIDE**.
2. Check your `.ioc` configuration:
   - **USART2** enabled in Asynchronous mode at **115200 baud** (ST‑Link VCP).
   - **PA5** configured as a GPIO output (on‑board LED on Nucleo‑F446RE).
3. Generate code if you changed the `.ioc`.
4. Build the project.
5. Flash the firmware to the Nucleo board.
6. Open a serial terminal on the ST‑Link VCP port:
   - Baud rate: `115200`
   - Data bits: `8`
   - Parity: `None`
   - Stop bits: `1`
7. You should see:
   - The LED blinking every ~500 ms.
   - Log messages similar to:
     ```text
     [00001234 ms][INF][app_main.c:42][App_MainInit] Application initialization started
     [00001235 ms][INF][app_task_manager.c:52][AppTaskManager_Init] Task Manager initialized (max tasks = 8)
     [00001736 ms][DBG][app_task_manager.c:103][AppTaskManager_RunOnce] Running task 'Heartbeat' (elapsed: 500 ms)
     [00001736 ms][INF][app_main.c:74][App_TaskHeartbeat] Heartbeat task toggled LED
     ```

If you see these logs and the LED blinks, Phase 1 is working correctly.

---

## 📌 Design Intent

Phase 1 is intentionally simple but sets up patterns you will reuse:

- **Logging everywhere**  
  Clear insight into code flow, useful when more tasks and drivers appear.

- **Task‑based thinking**  
  Even without a full RTOS yet, you’re already structuring work as tasks,
  which will map very cleanly to FreeRTOS tasks in a later phase.

- **Separation of concerns**  
  HAL and startup code stay in `Core/`.  
  Your application framework stays in `app/` and `common/`.  

---

## 🔭 Next Steps (Phase 2 and beyond)

Planned roadmap:

- **Phase 2**  
  - Introduce a **simulated sensor** module.  
  - Add a **Sensor Sampling task** that logs structured measurement data.  
  - Start defining a generic sensor interface.

- **Phase 3**  
  - Add basic **power management** (sleep/stop mode hooks).  
  - Create a **Power Manager task**.

- **Phase 4**  
  - Add a simple **UART/USB‑CDC protocol** and a PC‑side script.

- **Phase 5 (RTOS)**  
  - Migrate cooperative scheduler to **FreeRTOS**.  
  - Add a **Task Manager CLI** showing task stats.

Each phase will have its own release note entry and updated docs.
