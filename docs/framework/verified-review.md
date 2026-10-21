# Verified framework review

This is the finding ledger established at the 2026-10-20 baseline. It intentionally preserves the original evidence even after a finding is fixed; live resolution status and post-change evidence are recorded in the [implementation plan](implementation-plan.md) and [verification log](verification-log.md). The earlier [engineering review](../review.md) contains broader Garbox product findings.

## Critical

| ID | Finding | Confidence | Evidence and consequence |
|---|---|---|---|
| EVT-01 | Event pool is released while queued event pointers may remain after dispatch-depth exhaustion. | Established by source/API contract | `RuntimeAbs::dispatch()` performs a fixed number of passes and unconditionally calls `releaseDataPool()`. A cascade can leave `mEventQueue` non-empty, making its pointers reusable/dangling. |
| DISP-01 | RGB565 draw-buffer stride is calculated with `sizeof(lv_color_t)`, which is 3 in LVGL 9.4, while only 2 bytes/pixel are allocated. | Established by pinned API contract | `LvglHandler` sets stride to `width * sizeof(lv_color_t)` but allocates `width * height * 2`. LVGL 9.4 defines `lv_color_t` as three bytes and provides `lv_draw_buf_width_to_stride()`. This can cause out-of-bounds rendering and malformed transfers. Target symptoms remain to be observed. |
| SAFE-01 | Fatal handling stops tasks without first forcing hazardous actuators safe. | Established by source contract; electrical result requires target validation | `handleAssertExit()` calls `TaskManager::StopAll()` but never disables `Heatpad`. Deleting the main/control task can leave the last GPIO state asserted. Safe polarity and watchdog behavior require hardware confirmation. |

## High

| ID | Finding | Confidence | Evidence and consequence |
|---|---|---|---|
| CNT-01 | Static storage reports the wrong byte capacity. | Established by source contract | `StaticStorage<T,N>::capacityBytes()` returns `N * sizeof(N)` instead of `N * sizeof(T)`. Byte-bound checks can accept writes beyond the actual array, especially for byte storage. |
| SYNC-01 | `LockGuard` always uses recursive take/give APIs, but `Profiler` and `AnimatedLedGroup` create normal mutexes. | Established by FreeRTOS API/source contract | Mutex creation and locking APIs must match. `Profiler::Reset()` also nests locked functions, so it needs a recursive mutex or non-locking internal helpers. |
| STATE-01 | Generated pending-state getters return published state. | Established by generator source and generated output | `State.cpp.j2` implements `nextX()` with `mCurrent.x`; callers cannot inspect staged values as promised. |
| PWM-01 | Pre-start/first-cycle PWM reconfiguration can retain a stale threshold. | Established by source contract | Setters update current and next values while stopped/first-tick but do not update the threshold. `startNextCycle()` sees current equal next and skips recalculation, so the first cycle can use constructor-era timing. |
| HIST-01 | Four-day interval does not fit 32-bit microseconds. | Established arithmetically/source contract | `4 days / 65 ≈ 5.317e9 µs`, then storage truncates to `uint32_t`. |
| HIST-02 | Long-window average sum and count can overflow. | Established arithmetically/source contract | Power uses fixed-point values and is sampled around 30 Hz. A one-day bucket can exceed signed 32-bit sum; an intended four-day bucket can exceed the 16-bit count. |
| HIST-03 | `iterateSince()` can replay the retained ring multiple times when a consumer is more than capacity behind. | Established by source contract | Read position is clamped to retained samples, but logical `mCurrentWriteSequence` remains the caller’s older sequence and controls loop count. |
| GEN-01 | Generation can leave the working tree partially replaced. | Reproduced structurally; failure injection not yet implemented | `main.py` deletes `src/generated` before all rendering/resource conversion succeeds and updates several other roots/handwritten files in place. |
| BUILD-01 | Native tests do not compile. | Reproduced | Public headers omit their own standard includes. Only one existing test is collected. |
| ASSET-01 | Fresh checkout lacks both source and generated UI images. | Reproduced from Git index/ignore rules | `res/` and `src/bin/` are ignored and contain no tracked files. |

## Medium

| ID | Finding | Confidence | Evidence and consequence |
|---|---|---|---|
| EVT-02 | Event allocation failure yields a null view that normal callers dereference. | Established by source contract | Failure is logged with `TriggerDebug`; with current policy debug becomes fatal, but the API itself does not prevent null dereference or express failure. |
| EVT-03 | Submitted events are not actually immutable. | Established by source contract | `EventView(const EventHeader*)` casts away const and exposes mutable payload/header access. |
| LIFE-01 | Pools/containers accept non-trivial types without complete destruction semantics. | Established by source contract | Pool reset and several release paths do not destroy values, while generated event fields accept arbitrary declared C++ types. |
| LIFE-02 | Heap-backed infrastructure uses fatal destructors instead of ownership. | Established by source contract | Normal host teardown and reuse are prevented; allocated memory/RTOS objects are not released. |
| DISP-02 | Display ownership validates only buffer count, not identity/state. | Established by source contract; target stress test required | A counting semaphore cannot detect duplicate queueing, wrong-buffer completion, or illegal transitions. |
| DISP-03 | DMA failure is discarded and shutdown is incomplete. | Established by source contract | `handleChunkComplete(bool)` ignores the result; `Display::stopTasks()` and `SpiDma::stopTask()` contain in-flight-work TODOs. |
| DISP-04 | Draw-buffer structs are initialized through internal fields. | Established by dependency API comparison | LVGL supplies `lv_draw_buf_init()` with validation/default handlers. Direct initialization is version-fragile and currently enabled the stride defect. |
| TIME-01 | Short modular and long absolute durations are not distinguished in APIs. | Established by source contract | 32-bit microseconds are suitable for short wrap-safe timers but are reused for multi-day history. |
| MODE-01 | Redundant mode/screen requests remain queued. | Established by source contract | `applyQueuedBehaviour/Screen()` returns without clearing a request equal to the active object. |
| ISR-01 | Frequency sensor registers the same GPIO ISR twice. | Established by source contract; target behavior required | One unchecked registration is immediately followed by a second checked registration. |
| UI-01 | Row menu direct selection does not apply its argument. | Established by source contract | `RowMenu::setSelectedRow(index)` never assigns `mActiveRowIndex = index`. |
| GEN-02 | Generator environment and invocation are undocumented/unlocked. | Reproduced | Only an ignored local `.venv` makes generation work. |
| GEN-03 | GUI DSL contains untested dormant mapping/parser defects. | Established by source contract | Comma normalization is discarded; `transform-skew` and `border-post` map to incompatible wrapper calls; duplicate resource validation checks the wrong level. |

## Target-validation findings

| ID | Question | Required evidence |
|---|---|---|
| HW-01 | What are the electrically safe levels for heater enable, fan enable/speed, I2C power, and backlight? | Schematic/board inspection plus unpowered continuity or controlled bench check. |
| HW-02 | Is the physical ESP32-S3 module actually PSRAM-capable? | Module marking and runtime PSRAM report. |
| HW-03 | Does the corrected RGB565 buffer path preserve orientation/colors and improve/remove artifacts? | Panel test with color bars, full frames, narrow dirty regions, and charts. |
| HW-04 | Does buffer identity/order remain correct under maximum render and SPI load? | Instrumented buffer-state assertions and stress benchmark. |
| HW-05 | Are task priorities/cores appropriate? | Runtime latency, frame, stack, and watchdog measurements. |
| HW-06 | Does fatal shutdown force the real heater off under faults from every task context? | Non-heating electrical test first, then coordinated safe-load test. |

## Areas reviewed without a current rewrite recommendation

- The component/controller/behaviour/screen split is useful and should remain.
- Direct generated routing is simpler and more deterministic than a dynamic subscriber registry for this project family.
- Staged state publication and phase dispatch are useful once pending semantics and exhaustion behavior are fixed.
- The partial-render/SPI-DMA pipeline addresses a demonstrated throughput problem and should be stabilized, not replaced speculatively.
- Hardware wrapper breadth is acceptable for the current application, but unused abstractions should not be expanded until a second project demonstrates reuse.
- Fixed-capacity containers remain valuable where capacities represent real bounds; heap-backed versions must not be described as allocation-free.
