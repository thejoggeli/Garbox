# Project overview

## What Garbox is

Garbox is firmware for a temperature-controlled fermentation enclosure built around an ESP32-S3. It measures temperature and relative humidity, drives a heater with slow software PWM, circulates air with a controllable tachometer-equipped fan, and presents controls and telemetry on a 320×240 ST7789 display.

The project also became a general embedded application platform. Device behavior, screens, hardware instances, events, states, and tick phases are declared in configuration. A Python generator turns those declarations into typed C++ wiring, while handwritten C++ supplies the device-specific logic.

The current generated application contains:

- 15 components: 2 behaviours, 8 controllers, and 5 screens;
- 12 shared states: 10 application states and 2 runtime states;
- 12 event types: 10 application events and 2 runtime events;
- 7 ordered tick phases on a 33 ms period (about 30.3 Hz);
- one active behaviour and one active screen at a time.

## Repository map

| Path | Role |
|---|---|
| `platformio.ini` | ESP32-S3 firmware and native-test build definitions. |
| `partitions.csv` | 8 MiB flash layout: NVS, OTA metadata, one 6 MiB app slot, and LittleFS. |
| `codegen/config/` | Declarative source of truth for application topology, hardware, state, events, components, and XML GUIs. |
| `codegen/src/` | Python loader, validators/transforms, GUI parser, and generation pipeline. |
| `codegen/templates/` | Jinja templates for runtime, hardware, shared types, states, components, and GUIs. |
| `codegen/output/` | Ignored generator diagnostics: normalized JSON, preprocessed GUI XML, and stubs. |
| `src/generated/` | Generated C++ runtime, state classes, component bases/bindings, hardware instances, config, and GUI object trees. |
| `src/shared/` | Generated IDs, event types/payloads/aliases, state types, tick phases, plus shared core config. |
| `src/app/` | Handwritten Garbox application: control engine, component implementations, screens, providers, services, startup, and task orchestration. |
| `src/core/` | Reusable framework and platform infrastructure. |
| `src/modules/` | Higher-level reusable physical parts and sensors built on `src/core/hardware`. |
| `src/bin/` | Generated compiled image resources used by LVGL. |
| `res/` | Ignored source images consumed by the GUI generator. |
| `data/` | PlatformIO filesystem payload; currently contains two fan images and a test XML file. |
| `utility/thermal_simulation/` | Python/Jupyter thermal network models, simulator, and experimental controllers. |
| `test/` | Native Unity test suite; currently only exercises `ColorMap`. |
| `include/lv_conf.h` | LVGL feature, allocator, color, font, widget, logging, and driver configuration. |

The project is roughly 30,000 lines of C/C++ and 38,000 lines including generator/config sources. `src/core` is the largest area, followed by generated code, the app, and hardware modules.

## Actual fermentation-box behavior

### Startup

`setup()` performs the following sequence:

1. Initializes logging and installs debug/fatal assertion handlers.
2. Installs the ESP32 GPIO ISR service.
3. Initializes generated GPIO, ADC, LEDC, I2C, SPI-DMA, and timer instances.
4. Constructs and initializes all physical parts through `PartsProvider`.
5. Initializes the LVGL provider and starts LED, piezo, display-render, display-sender, and SPI-DMA tasks.
6. Plays an audible and visible sweep animation.
7. Constructs and initializes the generated application runtime.
8. Starts the main application task, which runs the tick loop forever.

Fatal assertions stop registered tasks, illuminate all status LEDs, beep/blink three times, and then remain in an error loop that periodically logs the failure.

### Temperature and humidity

An SHT31 on I2C address `0x44` runs in 1 Hz periodic mode. `I2cPartsController` manages a recovery state machine:

- power-off reset phase;
- power-on stabilization phase;
- normal sampling;
- automatic reset after repeated fetch failure.

It publishes temperature/humidity samples and driver/power/reset/running status.

### Heater

The heat pad has:

- an enable GPIO;
- voltage and current ADC channels;
- calibrated and filtered voltage/current measurements;
- a slow software-PWM driver, currently commanded with a five-second period;
- published current/next PWM settings and cycle progress;
- an RGB indicator whose color follows sensed heater voltage.

The measurements are currently telemetry only; they are not yet part of the heater safety logic.

### Fan

The fan has:

- an enable GPIO;
- a 25 kHz LEDC speed output;
- tachometer frequency measurement using a hardware timer and GPIO interrupt;
- signal conditioning and RPM conversion;
- idle/spinning/stalled monitoring with debounced transitions;
- repeated audible stall alerts;
- an optional RPM PID mode in `FanController`.

The fermentation behavior currently sends duty-style speed commands and does not enable the RPM PID mode.

### Fermentation control engine

`FermentationEngine` is isolated from most hardware concerns and models six states:

| State | Heater | Fan requested | Meaning |
|---|---:|---:|---|
| `Reset` | Off | Off | Regulation disabled and no cooldown required. |
| `Ready` | Off | Off | Regulation enabled, waiting for valid inputs. |
| `Regulating` | PID output | 40% | Normal temperature regulation. |
| `InvalidInput` | Off | 100% | Sensor invalid or fan reported stalled. |
| `OverTemperature` | Off | 100% | Measured temperature is more than 2 °C above target. |
| `Cooldown` | Off | 100% | Regulation was disabled above 30 °C. |

The heater PID constants are currently `Kp=0.05`, `Ki=0.02`, `Kd=0`, with output clamped to 0–100%. The engine delays exit from over-temperature for ten seconds unless the target is changed so the reading is no longer over target.

`FermentationBehaviour` translates shared states into engine inputs and translates engine outputs into fan/heater command events. It also accepts UI requests to toggle power, change the target, and change automatic fan mode.

### Input and feedback

The device uses a rotary encoder plus push button:

- button transitions and held-button repeats become typed events;
- encoder movement becomes signed step events;
- button actions play tones and animate a status LED;
- a heartbeat controller animates a separate LED and sends a heartbeat event every two seconds;
- startup, fan-stall, button, and fatal-error conditions use the piezo.

### Screens

The initial screen is `SimpleScreen`:

- target temperature in large type;
- measured temperature and humidity;
- heater output;
- circular target/measured temperature arcs with color interpolation;
- short press toggles fermentation power;
- encoder adjusts the target in 0.1 °C steps between 15 and 50 °C;
- long press opens the main dashboard.

`MainScreen` is a dashboard and menu:

- measured and target temperature chart;
- heater-power chart;
- fan, heater power, humidity, and system-state tiles;
- selectable history windows from one minute through four days;
- editable power, target temperature, history window, and fan-auto values;
- menu placeholders for calibration, about, and debug;
- long press opens the debug screen.

`DebugScreen` shows live runtime, heap, display, sensor, fan, heater, and fermentation diagnostics.

`EventLogScreen` records every routed event. `StateLogScreen` records every published state change. Button presses cycle `Debug → EventLog → StateLog → Simple`.

### History and charts

`GarboxHistory` maintains temperature, target, and heater-power series for eight windows:

- 1, 4, and 16 minutes;
- 1, 4, and 12 hours;
- 1 and 4 days.

Each series contains 65 averaged samples. The LVGL chart adapter can attach to a series, load existing samples, then append newly written samples.

History is RAM-only and resets on boot.

## Framework capabilities

### Application model

The framework distinguishes three component kinds:

- **Controllers** are always present and own ongoing I/O or services.
- **Behaviours** implement mutually exclusive high-level policies; only the active behaviour receives its configured ticks/events/state changes.
- **Screens** implement mutually exclusive UIs; only the active screen receives its configured ticks/events/state changes and is rendered.

Components communicate through generated typed events and shared states. A component only receives generated accessors and handlers for what its configuration declares.

### Event system

Events are value-initialized in a fixed-size bump-allocated pool and queued as pointers in a fixed-capacity ring buffer. Every event carries:

- a monotonically increasing ID;
- a generated event type;
- sender component type and ID;
- flags for inactive-component delivery and immediate routing;
- a generated, event-specific payload.

Generated runtime code routes each event directly to the known receivers without runtime registration or string lookup.

### State system

Each generated state stores current, next, and last data. Setters update next data and mark the state dirty. During dispatch, the runtime publishes all dirty states and calls generated reader handlers. This gives components a staged write/publish/read model and coalesces multiple writes before publication.

### Tick scheduler

The generated application loop runs seven phases:

| Order | Phase | Main work |
|---:|---|---|
| 1 | Heartbeat | Heartbeat event/LED. |
| 2 | SensorRead | Fan, heater, input, and SHT31 sampling. |
| 3 | UserInput | Active main-screen input tick. |
| 4 | Logic | Active behavior, including fermentation control. |
| 5 | ActorWrite | Fan/heater output and history sampling. |
| 6 | Logging | Periodic profiler output. |
| 7 | Render | Backlight and screen-render request, delayed 20 ms within the frame. |

State/event dispatch runs after every phase, so later phases normally see changes produced by earlier phases in the same 33 ms application tick.

### Reusable core utilities

`src/core` provides:

- assertion routing and logging;
- FreeRTOS task and lock wrappers;
- monotonic time snapshots, timers, software PWM, and time literals;
- fixed-capacity heap/static arrays, vectors, bitsets, masked arrays, data pools, and ring buffers;
- finite-state machines and dirty-handler dispatch;
- PID control, filters, ADC/tachometer conditioning, easing/waveform/math functions, lookup/sampled functions, and CRC;
- RGB/HSL/Lab/RGB565 conversions and interpolated color maps;
- time-series and multi-resolution history;
- profiler and time-slot scheduling;
- GPIO, ADC, I2C, LEDC, timer, pulse-counter, and SPI-DMA abstractions;
- an extensive LVGL-facing C++ layer with object/widget wrappers, menus, charts, grids, time-series adapters, rotated text, buffer helpers, and filesystem glue.

### Custom LVGL stack

The UI subsystem is a full vertical stack rather than a thin wrapper around LVGL. XML screen descriptions and PNG resources are compiled into statically typed C++ object trees and linkable image arrays. Generated screen bases connect application state changes to semantic dirty render functions. Handwritten screens use custom menu, chart, grid, history, and rotated-label helpers through a broad C++ LVGL façade.

At runtime, a custom LVGL port renders into three internal DMA-capable partial buffers. FreeRTOS tasks and semaphores hand dirty rectangles to a project-owned ST7789 driver, which sends commands synchronously and pixel data through chunked asynchronous SPI DMA. See [The custom LVGL stack](lvgl-stack.md) for the complete authoring-to-panel architecture and its review.

### Reusable modules

`src/modules` assembles core primitives into device-oriented abstractions:

- debounced polling and interrupt buttons;
- quadrature rotary encoder;
- fan, tachometer, conditioning, and stall monitor;
- heater sensing and software PWM;
- SHT31 periodic sensor driver;
- ST7789 display, LVGL handler, triple buffering, async SPI-DMA transport;
- static, dimming, animated, grouped, and RGB LEDs;
- queued tone/sequence piezo playback;
- generic frequency sensor.

## Thermal simulation tools

The Python tools implement a general lumped thermal RC network:

- arbitrary thermal nodes with heat capacities;
- bidirectional resistive heat links;
- fixed ambient node;
- injected heater power;
- numerical time stepping and Plotly visualization.

Four notebooks explore two-, three-, five-, and many-node models. Alternative PID, hysteresis, two-level hysteresis, and PID+hysteresis controllers can be compared before transferring a strategy to firmware.

