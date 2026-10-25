# Desktop simulation plan

## Purpose and boundary

The desktop simulator exists to execute Garbox application semantics deterministically without target hardware. It is not an ESP32 emulator and must not be used as evidence for electrical levels, peripheral timing, interrupt latency, DMA behavior, FreeRTOS scheduling, stack size, core affinity, or watchdog behavior.

The simulated boundary is the generated application component contract:

```text
scripted environment / thermal plant
                |
        simulated state producers
                |
real RuntimeAbs + events + states + FermentationBehaviour
                |
       recorded semantic commands
                |
   optional real LVGL screens and desktop display backend
```

The embedded build continues to use the concrete sensor, actuator, input, display, FreeRTOS, and ESP-IDF implementations. The simulator does not provide fake GPIO, I2C, LEDC, PCNT, SPI, or DMA APIs merely to make target drivers compile.

## Design decisions

### Deterministic virtual time

Simulation advances the existing fake `Time` backend explicitly. One call to `step()` advances one configured application period and performs a complete input-publication, logic, event-routing, and plant-update cycle. No host sleeps or worker threads are part of the headless correctness path.

### Semantic inputs and outputs

Inputs are temperature, humidity, validity/freshness, fan RPM, fan-stall state, and user requests. Outputs are the application's fan, heatpad, and display commands plus published status. Tests assert these semantic values, not mock call counts on low-level drivers.

### Real application policy

The simulator uses the real generated event/state types, `RuntimeAbs`, `FermentationBehaviour`, `FermentationEngine`, PID, FSM, timers, and dispatch rules. Only physical controllers are replaced. This exercises the application policy once while allowing target controllers to remain hardware-oriented.

### Optional plant model

The plant is deliberately simple and parameterized: ambient leakage, heater gain, fan cooling gain, thermal mass, humidity, and fan response. It is useful for long scenario and invariant testing, not for predicting enclosure performance. Recorded/replayed measurements can later replace it without changing the runtime harness.

### Real LVGL above the backend

The UI phase will compile the pinned LVGL core, generated GUIs, screens, and custom LVGL wrappers on the host. A memory/framebuffer backend is the reproducible baseline. An SDL window is optional when SDL development headers are available. Embedded `Display`, ST7789, SPI, DMA, and FreeRTOS buffer scheduling are not used by the desktop backend; their ownership model remains covered by target-neutral contract tests and later hardware tests.

The host and embedded backends install the same `LvglTheme` implementation.
This is part of the layout contract: generated GUIs assume that LVGL's default
padding, margin, border, radius, background, and scrolling styles have been
cleared before their explicit XML styles are applied. Display dimensions and
pixel size also come from the shared `AppConfig` constants.

## Work packages

| ID | Status | Depends on | Work | Acceptance |
|---|---|---|---|---|
| SIM-00 | DONE | existing architecture review | Define the simulation boundary, evidence limits, directory/build layout, and staged implementation. | This document distinguishes application evidence from hardware evidence. |
| SIM-01 | SOFTWARE VERIFIED | TEST-01, STATE-01, EVT-02 | Add deterministic application stepping and a headless simulation runtime using the real behaviour and framework contracts. | Native tests boot, publish inputs, run logic, and record typed fan/heatpad commands without sleeping or ESP headers. |
| SIM-02 | SOFTWARE VERIFIED | SIM-01, APP-01 | Add a parameterized thermal/fan plant, scenario runner, trace records, and safety invariants. | Startup, regulation, overtemperature, invalid sensor, cooldown, recovery, and a five-minute closed loop run deterministically. |
| SIM-03 | SOFTWARE VERIFIED | SIM-01 | Add scripted requests and UI input at the semantic event boundary. | Direct typed requests and real `Button`/`EncoderStep` routing produce expected power, target, and fan-policy changes. |
| SIM-04 | IN PROGRESS | SIM-01, UI-01 | Compile the real generated GUI/wrapper stack against a host framebuffer backend and export inspectable frames. | `SimpleScreen` constructs and renders at 320x240; input changes application state and the frame checksum. Main/debug/log screens and sanitizer-clean teardown remain. |
| SIM-05 | DEFERRED | SIM-04 | Add an optional interactive SDL frontend without making it a required test dependency. | Deferred until interactive use justifies adding SDL development packages; the headless path is sufficient for automated checks. |
| SIM-06 | SOFTWARE VERIFIED | SIM-02, SIM-04 | Add reproducible build/run commands. | Documented commands run unit/scenario/UI checks; the embedded build remains separately green. CI integration waits for the generator check work. |

## Directory and build layout

```text
src/simulation/              host-neutral simulation model and runtime
test/TestSimulation.cpp      deterministic contracts and scenarios
tools/                       optional scenario/UI launch helpers
docs/framework/              design, usage, evidence, limitations
platformio.ini               native tests plus a separate simulator environment
```

Simulation code must not add target firmware objects or dependencies. Source files under `src/simulation` are compiled only when `GARBOX_SIMULATION` is defined, and the embedded build remains the authoritative compilation check for target code.

## Scenario model

Each trace row records virtual time, inputs, active fermentation state, and semantic outputs. The initial required scenarios are:

1. disabled startup always commands heater and fan off;
2. valid cold input enters regulation and permits bounded heat;
3. overtemperature forces heat off and fan full;
4. invalid or stale input forces heat off and fan full;
5. disabling while hot enters cooldown and keeps heat off;
6. cooldown exits only below its threshold;
7. fan-stall input cannot leave the heater enabled;
8. every step maintains finite values, duty in `[0,1]`, and the heater-off safety implications.

The separate product decision for fatal-shutdown fan policy remains unresolved. Simulation makes either choice testable, but does not choose it implicitly.

## Running it

From the repository root:

```bash
# Sanitized native contract and application scenarios
pio test -e native

# Standalone closed-loop trace (CSV on stdout)
pio run -e simulator
.pio/build/simulator/program --minutes 10 --target 32 --initial 20

# Real LVGL + generated SimpleScreen into a deterministic framebuffer
pio run -e ui-simulator
.pio/build/ui-simulator/program --output /tmp/garbox-simple.ppm

# Target compilation remains a separate evidence class
pio run -e esp32-s3-devkitc-1
```

The UI executable first renders the disabled state, then injects encoder and
button events through `SimpleScreen`. It fails unless the target becomes
32.5 °C, fermentation becomes enabled, LVGL flushes a non-empty frame, and the
disabled/enabled checksums differ. The optional PPM is an inspectable artifact,
not a checked-in golden image.

## Current verification record (2026-10-24)

- Native ASan/UBSan suite: 63/63 tests passed.
- Standalone simulator: builds and runs a one-minute deterministic trace with
  all step invariants satisfied.
- Headless LVGL 9.4.0: 320x240 RGB565 render with the shared embedded theme
  succeeded; disabled checksum `62a1bc832670339f`, enabled checksum
  `e6a84a9fcba3a912`.
- ESP32-S3 firmware: builds successfully; 30,408 bytes RAM and 863,089 bytes
  flash. `src/simulation` is excluded from this build.
- No hardware evidence was produced. Sensor behavior, actuator polarity,
  thermal response, display transport/DMA, and timing remain target-gated.

## Evidence classification

- Passing headless scenarios is software evidence for application state transitions and emitted semantic commands.
- Passing LVGL host rendering is software evidence for object construction, layout execution, and state-to-UI adaptation with the pinned library.
- It is not evidence for actual sensor accuracy, actuator polarity, thermal limits, fan airflow, display color/order, SPI throughput, DMA completion, ISR behavior, or shutdown electrical safety.
