# Engineering review

## Review scope and evidence

This review covered the tracked repository plus the pre-existing uncommitted working-tree changes. It included source/config inspection, a native test attempt, a full ESP32 build, and an isolated code-generator reproduction check.

Results on 2026-10-19:

- ESP32 firmware build: **successful**.
- Linked size: **30,232 bytes RAM (9.2%)**, **859,089 bytes flash (13.7% of the 6 MiB app partition)**.
- Generator run: **successful and byte-reproducible** for generated, shared, binary-resource, and generated-header-section outputs.
- Native tests: **do not compile** because several custom-container headers use `size_t` and `std::forward` without directly including `<cstddef>` and `<utility>`.
- Existing tests: one Unity file focused on `ColorMap`; no application, runtime, control, history, driver, or generator tests.

## What is strong

### The architecture has real boundaries

Hardware drivers, physical parts, controllers, policy, and screens are distinct. `FermentationEngine` is not entangled with LVGL or GPIO. Controllers own hardware. Screens make requests rather than driving actuators. This is a very good base for host-side testing.

### The generator removes the right repetition

Generated state/event types, direct routing switches, ID enums, component state bundles, phase handlers, hardware singletons, and GUI construction would be tedious and error-prone by hand. The generator gives each handwritten component a narrow interface based on declared capabilities.

The generator also has a sensible preservation strategy: it refreshes marked header sections and refuses to overwrite existing component `.cpp` files.

### Embedded resource use is deliberate

The main event path uses fixed-capacity pools and queues. State updates coalesce. The application tick is ordered and measured. Display rendering and SPI transport are offloaded. Diagnostic counters and profiling are present. The resulting firmware has ample flash and static RAM headroom.

### Hardware recovery and observability are better than average

The SHT31 is power-cycled after fetch failure. Fan stall is modeled explicitly. Fatal errors are visible and audible. The debug, event-log, and state-log screens are unusually useful for bringing up an appliance without a debugger attached.

### UI generation is ambitious but functional

The XML system supports reusable components, parameter substitution, images, styles, and typed C++ wrappers. Both generated GUIs reproduce exactly. The dashboard/history architecture is substantive rather than decorative. More broadly, the project owns nearly the full application-to-panel path around LVGL: UI compilation, generated static views, dirty rendering, higher-level controls, triple buffering, task coordination, the ST7789 protocol, and chunked SPI DMA. [The custom LVGL stack](lvgl-stack.md) reviews this subsystem separately.

### The simulations are directly relevant

The thermal network and multiple control variants provide a good place to tune behavior before heating physical hardware. They should become part of the control-validation loop rather than remain exploratory notebooks.

## Blockers before unattended heater operation

These should be treated as safety/correctness work, not polish.

### 1. Regulation starts enabled by default

`FermentationStatus.powerOn` defaults to true, and `FermentationBehaviour::onBecomeEnabled()` explicitly sets it true. Once the sensor becomes valid, the box can begin heating without a deliberate operator action after boot.

Recommendation: boot with heater regulation off, preserve a clearly defined safe output state through initialization, and require an explicit enable action. If automatic resume is ever desired, persist a run plan plus a clean-shutdown marker and make resume an explicit product decision.

### 2. “Fan Off” overrides safety cooling

The engine requests 100% fan in `InvalidInput`, `OverTemperature`, and `Cooldown`, but the behavior sends `output.fanEnabled && autoFan`. When the menu’s fan setting is “Off,” it disables the fan even in the states where the engine is trying to cool the box.

Recommendation: define override precedence. Safety cooling must dominate user preference. A user-selectable “circulation off during normal regulation” setting can exist, but it must not suppress over-temperature or cooldown output.

### 3. Temperature validity is weaker than the UI’s validity check

The fermentation behavior considers the temperature valid when I2C power and driver are enabled and resetting is false. It does not require `hasFirstSample` or `isRunning`, while the main screen does. That creates a path where default/stale sample data can be considered suitable for regulation.

Recommendation: use one shared validity definition requiring a recent first sample, running state, finite values, plausible range, and freshness timeout. The control engine should receive sample age or an explicit validity decision.

### 4. There is no independent hard safety envelope

The only over-temperature rule is target +2 °C. There is no target-independent maximum, maximum heater-on duration, sensor-stuck detection, redundant cutoff, watchdog policy, or hardware thermal fuse represented in the repository. Heater voltage/current are displayed but not used to detect a stuck-on switch, open heater, or unexpected current.

Recommendation: add an independent safety supervisor outside the PID policy, plus physical protection appropriate to the heater and enclosure. At minimum it should enforce absolute temperature limits, sample freshness/plausibility, actuator-command consistency, and latched faults.

### 5. PID anti-windup is incorrect for recovery from saturation

`PidControl` restores the old integral whenever output is at either limit, regardless of whether the current error would drive farther into saturation or back out of it. In particular, a negative error while the output remains saturated high cannot unwind the integral. This can prolong full heater output after overshoot.

Recommendation: only reject an integration step when it drives further into the active limit, or use conditional integration/back-calculation. Add deterministic step-response tests before tuning gains. The Python PID implementation contains the same logic and should be corrected in parallel.

### 6. The fan tachometer ISR is registered twice

`FrequencySensor::init()` calls `addInterruptHandler()` once without checking the result, then calls it again inside a critical section and treats failure as fatal. This is clearly unintended and may either fail initialization or obscure which registration is active, depending on ESP-IDF behavior.

Recommendation: register once, check once, and keep interrupt-disable sequencing explicit. Add a hardware smoke test that proves fan RPM transitions from zero to plausible values.

### 7. A long button hold deliberately enters fatal error handling

`InputController::handleButtonHold()` calls `TriggerExit("Testing")` once repeat-generated tone frequency exceeds 3000 Hz. With the current 1.2 s initial delay and 300 ms repeat, a sufficiently long hold will intentionally crash the device.

Recommendation: remove the diagnostic trap; clamp, stop, or change the feedback pattern instead.

## Framework/runtime correctness findings

### Dispatch can release storage while queued event pointers remain

`RuntimeAbs::dispatch()` performs at most three state/event cascade passes and then always resets the event data pool. If the configured recursion depth is exhausted while events remain queued, those queued pointers refer to memory the factory now considers free. A later allocation can overwrite them before they are routed.

Recommendation: after the last pass, assert that both queues are empty; alternatively defer pool reset until all pointer consumers are gone. If bounded cascades are required, fail safely instead of retaining dangling queued events.

### Generated `next...()` state getters return current data

The state template implements every `nextField()` getter as `return mCurrent.field` rather than `mNext.field`. The name and comments promise staged state. `FanController` already uses `nextMeasuredRpm()`, so this is observable behavior, not merely a defective unused API.

Recommendation: fix the template, regenerate, and add a generator/runtime unit test for current/next/last semantics.

### Same-component switch requests remain queued forever

If the requested behavior or screen is already active, `applyQueuedBehaviour()` / `applyQueuedScreen()` logs and returns without clearing the queued pointer. Repeated application attempts and logs follow.

Recommendation: clear the redundant request or reject it when queued.

### Pool/container contracts are narrower than their APIs imply

The bump pool does not invoke destructors on reset and is documented as intended for POD-like values, but the event generator accepts arbitrary C++ field types. Several custom containers also expose operations that are only safe for trivial types, while heap-backed storage destructors deliberately trigger fatal assertions.

Recommendation: encode restrictions with `static_assert(std::is_trivially_destructible_v<T>)` where appropriate, or implement correct lifetime handling. Make reusable core types obey normal RAII for host tests unless there is a measured embedded reason not to.

### Headers rely on transitive includes

Native compilation fails in `RingBufferBase`, `VectorBase`, and `DirtyDispatcher` because required standard definitions are not included locally. `DataPoolBase` also uses placement new and forwarding without including every defining header itself.

Recommendation: make every header self-contained and add a “compile each public header alone” check.

### Callback guard in `FanMonitor` uses the wrong boolean operation

On entry to the stalled state, the code invokes the callback when `period > 0 || callback`. If a period is configured without a callback, it dereferences a null `std::function` target. Current app setup installs the callback, but the generic class contract is unsafe.

Recommendation: require `callback` before invoking it; treat the period independently.

## History and UI correctness findings

### Four-day sample timing overflows 32-bit microseconds

`TimeSeries` computes a 64-bit interval but stores it in `uint32_t`. A four-day window divided into 65 samples is about 5.317 billion microseconds per sample, beyond the 32-bit limit. That history window therefore samples on the wrong interval.

### Long-window averaging has almost no numeric headroom

The accumulated value and sample count are too narrow for high-rate, fixed-point power sampling over the long windows. At the current 33 ms tick, a 100%-power value would accumulate to about 4.12 billion during a one-day-series interval, but `mAccumulatedValue` is signed 32-bit and therefore overflows roughly halfway through. The intended four-day interval would also exceed the 16-bit sample counter; its separate interval overflow currently masks that path by sampling too early.

Recommendation: use 64-bit elapsed/accumulator arithmetic or downsample hierarchically, and test every configured window at maximum representable input.

### Stale chart adapters can replay beyond retained history

`IteratorSinceSequence` limits its starting index to available samples but keeps its logical iteration count at the full unseen sequence count. If a consumer falls more than one ring capacity behind, it can loop through the buffer repeatedly instead of yielding only retained samples.

Recommendation: set the iterator’s logical start to `targetSequence - readableCount` and test wrap, overrun, and sequence wraparound.

### Several user-facing features are placeholders

- Simple-screen fan rendering is empty.
- Display brightness commands are ignored because the handler body is commented out.
- Main-menu Calibration, About, and Debug rows display “Open” but have no action.
- The calibration behaviour is not reachable from the current UI and contains no actual calibration procedure.
- Active-screen display on the debug screen is noted but not implemented.
- Time-axis labels contain `12n` and `16n` typos.
- Simple and main screens enforce different target ranges (15–50 °C versus 20–40 °C).

These are normal prototype gaps, but the UI should not advertise inactive actions.

### The LVGL stack has its own stabilization backlog

The UI compiler and runtime deserve dedicated tests. The XML renderer contains dormant parsing/mapping defects, including comma splitting that discards its normalized value and incorrect mappings for `transform-skew` and `border-post`. `RowMenu::setSelectedRow()` does not assign the requested index. Fonts and image resizing are modeled but unimplemented. The manual LVGL draw-buffer initialization and implicit single-thread access contract increase upgrade/concurrency risk.

The display error path is also incomplete: shutdown does not resolve in-flight DMA, queue creation is only debug-asserted, and DMA failure is discarded by `Display::handleChunkComplete`. These issues do not invalidate the normal pipeline, but they should be fixed and stress-tested before the UI layer is treated as stable infrastructure. See [The custom LVGL stack](lvgl-stack.md) for the full analysis.

## Build, dependency, and repository findings

### What works

- Current ESP32 code compiles and links.
- Generated files match the generator.
- PlatformIO pins LVGL to compatible major/minor `^9.4.0` and declares NeoPixel.
- Ignored outputs keep the tracked tree focused.

### What needs tightening

- There is no generator dependency manifest or documented invocation. The ignored `.venv` currently supplies PyYAML, Jinja2, SortedContainers, Pillow, NumPy, Plotly, and notebook tooling, but another machine cannot reproduce that environment from the repo alone.
- PlatformIO’s board report says “No PSRAM” while `board_build.psram = enabled`; this should be reconciled with the exact physical ESP32-S3 module/board variant.
- `AppConfig` duplicates event-pool/queue constants that are also generated from application YAML, inviting drift.
- Source GUI resources in `res/` and generated `src/bin/` are both ignored. A fresh clone has neither the source icons nor their generated C arrays, so it cannot reproduce or link the current image-using GUIs without obtaining those files elsewhere.
- `data/` includes what look like legacy/test image/XML artifacts rather than a documented LittleFS payload.
- The README currently contains only a model-field quick reference for a feature not present in the reviewed state parser. It does not explain setup, generation, build, wiring, or operation.
- Generated output is spread across `src/generated`, `src/shared`, `src/bin`, and marked app-header sections. The boundaries are consistent but not obvious without documentation.

## Code quality assessment

| Area | Assessment |
|---|---|
| Overall design | Strong, coherent, and unusually modular for a personal embedded project. |
| Generated architecture | High value; direct typed routing is a genuine strength. |
| Hardware abstraction | Broad and useful, but more custom surface area than is currently tested. |
| Control safety | Prototype grade; not ready for unattended heating. |
| UI/diagnostics | Feature-rich and thoughtfully instrumented; several unfinished paths. |
| Concurrency | Purposeful display pipeline, but shutdown/failure behavior needs stress testing. |
| Tests | Far below the complexity and risk of the codebase. |
| Documentation | Previously minimal; this folder establishes a baseline. |
| Reproducibility | Generated output is reproducible on the existing environment; dependency/bootstrap story is missing. |
| Maintainability | Good boundaries offset by a large custom framework and several implicit contracts. |

## Was it overengineered?

Yes—but in a productive way up to a point.

The typed generated topology, separation of control from hardware, simulator, diagnostics, and screen tooling all create leverage. They make sophisticated changes possible without turning the firmware into a monolithic loop.

The overengineering becomes costly where generic infrastructure lacks tests or where the framework’s abstraction surface is larger than the application needs: custom lifetime-sensitive containers, multiple scheduling mechanisms, a GUI DSL, and broad wrapper layers all require maintenance.

The right response is not a rewrite. The project has a solid internal model and already builds. The right response is to stop adding generality, lock the architecture down with tests, and spend the next development cycle on appliance behavior, safety, and validation.

