# Verification log

This is the append-only evidence log for the stabilization program. A successful host check proves only the exercised software contract; it does not substitute for ESP32 timing, display, sensor, or actuator validation.

## 2026-10-20 — Baseline

- Revision: `6c6b5f08661b590b82384002eaae69200b307261` (`main`) plus the protected pre-existing working-tree changes listed in the [baseline](baseline-2026-10-20.md).
- Generator: a temporary-copy generation run completed and reproduced `src/generated`, `src/shared`, `src/bin`, and `src/app` byte-for-byte.
- ESP32-S3 firmware: successful build, 30,232 bytes RAM (9.2%) and 859,089 bytes flash (13.7%).
- Native environment: failed to compile because reusable headers depended on transitive declarations of `size_t`, `std::move`, and `std::forward`.
- Hardware measurements: not run; no target hardware is available.

## 2026-10-20 — BUILD-01, CNT-01, TEST-01

Changes under test:

- public container/helper headers include the declarations they use;
- standalone translation units compile representative headers in isolation;
- `StaticStorage<T, N>` reports `N * sizeof(T)` bytes rather than `N * sizeof(N)`;
- fixed-capacity tests cover one-, two-, and four-byte element types and exact data-pool exhaustion;
- the native test environment uses AddressSanitizer and UndefinedBehaviorSanitizer;
- a deterministic fake `Time` backend covers snapshots, delay advancement, and 32-bit microsecond wrap.

Verification:

```text
/home/michael/.platformio/penv/bin/platformio test -e native
23 test cases: 23 succeeded
native:* PASSED in 2.718 s

/home/michael/.platformio/penv/bin/platformio run -e esp32-s3-devkitc-1
SUCCESS in 111.981 s
RAM:   30,232 / 327,680 bytes (9.2%)
Flash: 859,089 / 6,291,456 bytes (13.7%)
```

No sanitizer diagnostics were emitted. The firmware footprint is unchanged from baseline. These tasks are therefore marked **SOFTWARE VERIFIED**. No hardware claim is made.

## 2026-10-20 — STATE-01

- Corrected the state generator template so `nextX()` reads the pending `mNext` snapshot.
- Regenerated all state implementations; all twelve generated state types changed only at their pending getters.
- Added a generated-state regression that distinguishes pending, current, and last values both before and across two publications.
- Native ASan/UBSan suite: 24 of 24 tests passed in 6.748 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 117.534 seconds at the unchanged 30,232-byte RAM and 859,089-byte flash footprint.
- Generator completed successfully with the current configuration. Its pre-existing generated hardware differences remain part of the protected user work, not this task.

## 2026-10-20 — EVT-01

- Runtime dispatch now releases the event bump pool only after both the dirty-state and event queues are empty.
- If the configured dispatch-pass limit is reached with work remaining, queued event pointers and their pool storage survive until a later dispatch.
- Regression coverage exercises a cascade ending exactly at a two-pass limit, a cascade extending one event beyond it, allocations between dispatches that would deterministically overwrite the queued event under the old behavior, ordered completion, and pool reuse after stability.
- Expanding host coverage exposed an unqualified-name lookup failure in `DataPoolHeap` under the native compiler; explicitly qualifying its storage base fixed the public header without changing target behavior.
- Native ASan/UBSan suite: 26 of 26 tests passed in 8.464 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 124.888 seconds: RAM remains 30,232 bytes; flash is 859,105 bytes, a 16-byte increase from baseline.

## 2026-10-21 — TIME-01, PWM-01

- Replaced `SoftwareTimer`'s period-by-period catch-up loop with constant-work modular arithmetic while retaining the latest phase boundary.
- Documented the 32-bit timer observation limit; long history horizons remain a separate 64-bit task.
- Removed the ambiguous PWM “first tick” window. Configuration made while stopped now defines the first cycle and recalculates its threshold.
- Running changes with `finishCurrent=true` remain staged for the next natural cycle. Changes with `finishCurrent=false` now start a new cycle immediately rather than retaining the previous cycle's time origin.
- Fake-clock coverage includes zero-duration/reset behavior, `UINT32_MAX` crossing, missed periods, a 4,000,000,321-microsecond stall, both PWM phase modes, 0%/100%, pre-start configuration, deferred/immediate duty and period changes, and stop/restart.
- Native ASan/UBSan suite: 37 of 37 tests passed in 2.312 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 95.700 seconds: 30,224 bytes RAM (8 fewer than baseline) and 859,105 bytes flash (16 more than baseline).

## 2026-10-21 — EVT-01 contract refinement

- Aligned the implementation with the target bounded-cascade contract: reaching the configured pass limit with state/event work still queued now raises `TriggerExit` before retaining the pool.
- The host assert handler deliberately returns, allowing the regression to prove that the queued event remains intact even after the fatal signal. Exact-limit stable completion raises no assertion and resets the pool normally.
- Native ASan/UBSan suite: 37 of 37 tests passed in 2.380 seconds. ESP32-S3 build passed in 34.354 seconds at 30,224 bytes RAM and 859,137 bytes flash.

## 2026-10-21 — DISP-01

- Replaced direct writes to LVGL's internal `lv_draw_buf_t` fields with the public `lv_draw_buf_init()` API.
- RGB565 allocation geometry is explicit and overflow-safe: the configured 320-pixel width has a 640-byte stride, and each 60-row partial buffer occupies 38,400 bytes.
- At initialization, the independently calculated stride is checked against `lv_draw_buf_width_to_stride(..., LV_COLOR_FORMAT_RGB565)` from the pinned LVGL version before any buffer is registered.
- Preserved the existing three-buffer, render-task, queue, and chunked-DMA architecture; this milestone changes buffer construction, not transport scheduling.
- Native ASan/UBSan suite: 38 of 38 tests passed in 1.345 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 34.009 seconds: 30,232 bytes RAM (equal to baseline) and 859,481 bytes flash (392 more than baseline).
- Physical color order, orientation, rendering, and sustained DMA behavior remain unverified until target hardware is available and are tracked by DISP-04.

## 2026-10-21 — DISP-02

- Added a fixed-capacity ownership tracker for the three exact LVGL draw-buffer addresses and integrated it with the real flush queue, sender task, and final chunk-completion path.
- The tracker mirrors FIFO order and rejects unknown or duplicate buffers, duplicate queueing, out-of-order DMA starts, multiple DMA owners, wrong/duplicate completion, and invalid reuse.
- FreeRTOS semaphores remain the blocking/backpressure mechanism. A dedicated recursive mutex serializes identity/FIFO state transitions across render, sender, and SPI completion tasks.
- A failed FreeRTOS queue insertion now rolls back both tracker ownership and the free-buffer token. Queue, render-ready, and tracker synchronization objects are all checked with fatal initialization assertions.
- Host coverage exercises repeated partial flushes, all three buffers in flight, ordered completion, duplicate and wrong transitions, queue rollback, and post-completion reuse.
- Native ASan/UBSan suite: 42 of 42 tests passed in 1.311 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 34.472 seconds: 30,288 bytes RAM (56 more than baseline) and 860,709 bytes flash (1,620 more than baseline).
- Physical scheduling, panel output, and sustained-load behavior remain hardware-gated under DISP-04; transport failure and shutdown semantics remain DISP-03.

## 2026-10-21 — SYNC-01

- Replaced the ambiguous `LockGuard`, which always used recursive FreeRTOS APIs, with explicit `MutexGuard` and `RecursiveMutexGuard` types. Null handles and failed take/give operations are fatal contract violations.
- Normal creation/take/give is now used by Profiler, AnimatedLedGroup, StaticLed, the display ownership tracker, and SpiDma. AnimatedLed and PiezoPlayer retain recursive mutexes because their public operations intentionally call locked helpers.
- Removed Profiler's nested locked calls during reset, moved enabled checks under its mutex, and changed record accessors to return value snapshots rather than references/pointers that outlived the lock.
- SpiDma now frees a completed transaction slot and releases its normal mutex before invoking a copied callback. Chunk completion may safely queue the next asynchronous transfer without recursive lock re-entry or a lock held across user code.
- Assertion handlers no longer construct no-op guards around never-created mutexes. Handler registration is a single-threaded startup operation, and an atomic latch selects the first fatal caller; invocation remains explicitly task-context only.
- Added a host FreeRTOS semaphore model that distinguishes normal and recursive handles/APIs. Tests prove matching calls, two-level recursive acquisition, mismatch detection, single-lock Profiler reset, and snapshot behavior.
- Native ASan/UBSan suite: 45 of 45 tests passed in 7.003 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 37.828 seconds: 30,288 bytes RAM (56 more than baseline) and 860,793 bytes flash (1,704 more than baseline).
- ISR registration and target interrupt behavior remain separately tracked by ISR-01; no mutex guard is permitted in ISR context.

## 2026-10-21 — DISP-03 transport-failure slice

- Extended `DisplayBufferTracker` with explicit `Running`, `Faulted`, and `Stopped` lifecycle states plus a terminal `Invalid` state for every registered allocation.
- A failed final DMA chunk now verifies the active buffer identity, invalidates queued/in-flight ownership, deselects the panel, releases the sender barrier, and then raises the fatal signal. It deliberately does not return an LVGL reuse token.
- Terminal display states reject new render, flush, queue, and completion work. Stop is idempotent and preserves an earlier fault state.
- Screen changes remain safe under the existing render-ready handshake: activation precedes the next render, while generated screen trees and draw-buffer allocations have process lifetime. An older transfer therefore cannot reference freed screen or buffer memory.
- Native ASan/UBSan suite: 47 of 47 tests passed in 3.179 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware build passed in 34.723 seconds: 30,296 bytes RAM (64 more than baseline) and 861,329 bytes flash (2,240 more than baseline).
- This is intentionally not marked software-verified yet. Generic `Task::stop()` still directly deletes tasks and can interrupt the sender while it owns the SPI path; TASK-01 must close that deadlock risk. Physical transport-failure injection remains hardware-gated.

## 2026-10-21 — TASK-01 and DISP-03 closure

- Replaced external `vTaskDelete()` with an atomic cooperative stop token passed to every managed task handler. Only a worker whose handler has returned may delete itself.
- `Task::stop()` is idempotent and waits for at most 1.5 seconds. A current-task call is request-only; an external timeout is reported without forced deletion.
- Task owners close new-work admission and bound or wake their specific blockers: display semaphores/queue, a 100 ms piezo notification poll, one-second SPI completion polling, periodic LED delay, and main tick-phase boundaries. SPI admission is an atomic gate, so stop never waits for the transfer mutex merely to reject new work.
- Display stop now wakes render, free-buffer, queue, and DMA waits before joining. SPI stop rejects new synchronous/asynchronous transfers before joining, so shutdown cannot delete the sender while it owns the SPI mutex or driver stack.
- The generated runtime and its generator templates both propagate the stop token through `RuntimeAbs` into `TickRunner`, preventing regeneration from reverting the contract.
- Added a deterministic native FreeRTOS task model. Regressions prove external stop never deletes the worker, current-task stop is request-only, simulated in-flight ownership is released by handler unwinding, self-deletion happens once, and repeated stop succeeds.
- Final native ASan/UBSan suite: 50 of 50 tests passed in 1.846 seconds with no sanitizer diagnostics.
- Final incremental ESP32-S3 firmware build passed in 38.160 seconds: 30,296 bytes RAM (64 more than baseline) and 862,289 bytes flash (3,200 more than baseline).
- TASK-01 and DISP-03 are therefore **SOFTWARE VERIFIED**. An ESP-IDF transaction already accepted at shutdown may finish against process-lifetime storage; physical bus/panel behavior and transport-failure injection remain hardware-gated under DISP-04. Hazardous actuator shutdown ordering remains SAFE-01.

## 2026-10-21 — ISR-01

- Removed the duplicate frequency-sensor `gpio_isr_handler_add()` call. The per-instance handler is now registered exactly once and the result is fatal-checked.
- Kept GPIO driver calls and failure reporting outside port critical sections.
- The frequency ISR now protects its edge timestamp/flag update with `portENTER_CRITICAL_ISR`; task code uses the matching task-context critical section for coherent snapshot/reset operations.
- Startup remains the sole owner of `gpio_install_isr_service()`, executed before parts attach handlers.
- ESP32-S3 firmware build passed in 34.263 seconds: 30,296 bytes RAM (64 more than baseline) and 862,421 bytes flash (3,332 more than baseline).
- Native tests do not emulate ESP32 interrupt dispatch; target tachometer edge/timeout behavior remains hardware-gated.

## 2026-10-21 — SAFE-01 heater-off slice

- Added a fixed-capacity safe-output registry using plain function/context pairs, with no recurring allocation or `std::function` dependency. Atomic first-apply semantics make it idempotent and reject late registration.
- Fatal handling applies safe outputs before any cooperative task stop/wait. `MainTask` is registered as the first stop handler so ordinary application output commands are stopped before auxiliary joins.
- The heatpad now has a one-way emergency latch: future enable/duty/period commands and ticks are ignored, software PWM resets without invoking callbacks, and the logical enable output is driven low.
- Native tests cover registration order, exactly-once application, null/capacity/late-registration rejection, and callback-free PWM reset. ASan/UBSan suite: 53 of 53 tests passed in 8.470 seconds.
- ESP32-S3 firmware build passed in 107.886 seconds: 30,344 bytes RAM (112 more than baseline) and 862,961 bytes flash (3,872 more than baseline).
- SAFE-01 remains **IN PROGRESS** pending the product decision for fatal fan behavior (full-speed cooldown versus power-off). GPIO polarity and physical heater-off behavior remain hardware-gated.

## 2026-10-22 — EVT-02

- Event-pool allocation exhaustion is now an explicit fatal contract rather than a debug-only factory result. The returned view is invalid and retains a null header, while payload access uses discard storage so a recovery-capable host handler cannot expose existing call sites to a null dereference.
- Invalid views expose `valid()` and an explicit boolean conversion. Component submission drops their null headers after the originating fatal error; direct null submission to the runtime is itself fatal and is neither counted nor routed.
- A full event queue rejects the new pointer, preserves the existing FIFO, and increments the runtime event count only for accepted events. The dispatch/pool-retention guarantees from EVT-01 remain unchanged.
- Native ASan/UBSan coverage uses a zero-byte pool and a one-entry queue, and also tests direct null submission. The full suite passed 56 of 56 tests in 3.354 seconds with no sanitizer diagnostics.
- ESP32-S3 firmware built successfully in 47.493 seconds: 30,408 bytes RAM and 862,945 bytes flash.
- EVT-02 is therefore **SOFTWARE VERIFIED**. As before, fatal-handler continuation exists only to make failure aftermath testable; the target fatal handler does not return.

## 2026-10-27 — GEN-01 through GEN-04, ASSET-01, and FLOW-01

- Added pinned generator dependencies and made the entry point independent of the caller's working directory.
- YAML/config loading is deterministically ordered and rejects duplicate keys/definitions. Pre-generation validation covers identifiers, duplicate fields/components, event/state/tick references, initial modes, LEDC timer bindings, GUI component parameters, and image paths.
- Generation now renders into a same-filesystem staging tree. The checkout is updated only after configuration parsing, validation, template rendering, generated-section replacement, and PNG conversion all succeed; application includes rollback if an apply operation fails.
- Added non-mutating `--check` mode with readable file/diff output. It passes against the current checkout and detects deliberately stale output without repairing it.
- Added nine generator/GUI compiler regressions, including injected malformed-template failure that leaves generated, shared, binary-resource, and handwritten overlay files byte-identical.
- Fixed comma/repeated-space tokenization, `transform-skew`, `border-post`, duplicate GUI resource/component/normalized-object detection, unknown/missing component parameters, and leaked PNG file handles.
- Source PNGs and the deterministic generated C arrays are now tracked. The README documents setup/generation, and `tools/verify_project.py` provides one software gate.
- Verification: generator tests pass with `ResourceWarning` promoted to an error; generated-output check is clean; 64/64 native ASan/UBSan tests pass; the full desktop UI flow passes; ESP32-S3 firmware builds at 30,440 bytes RAM and 889,697 bytes flash.

## 2026-10-27 — EVT-03, STATE-02, and MODE-01

- Event submission now seals both event metadata and payload. Mutable access after submission and duplicate submission are fatal contract violations; recovery-capable host handlers receive discard storage instead of being able to alter queued data.
- The raw event header is exposed as const. Explicit pre-submit setters retain the supported metadata configuration path without allowing callers to bypass the lifetime guard.
- The two application call sites that cached command payloads after submission now copy before transferring ownership.
- Re-requesting the active behaviour or screen clears any older pending switch immediately. Defensive apply paths also consume redundant requests.
- Runtime tests prove submitted payload immutability, duplicate-submit rejection, cancellation of pending mode switches, coalescing of multiple state writes, and callback-generated publication in a later dispatch pass with correct pending/current/last values.
- Verification: generated-output check is clean; 67/67 native ASan/UBSan tests pass; ESP32-S3 firmware builds at 30,432 bytes RAM and 890,765 bytes flash.

## 2026-10-27 — CNT-02 and LIFE-01

- Heap storage now owns and frees aligned raw allocations. Size multiplication is overflow-checked, allocation failure leaves an empty safe object, and copy/move operations cannot alias ownership.
- Arrays, vectors, ring buffers, and masked arrays destroy every live object during normal teardown. Raw-storage containers are non-copyable/non-movable so object lifetimes cannot be duplicated with a bytewise copy.
- Non-destructing vector/ring release operations and bump-pool construction are restricted at compile time to trivially destructible values. Vector `assign()` now copy-constructs rather than assigning into storage before object lifetime begins.
- Pool alignment is checked for nonzero power-of-two fundamental alignment, and its offset/size arithmetic cannot wrap. Static zero-capacity storage retains a legal aligned backing byte without claiming capacity.
- Lifetime probes cover arrays, masked arrays, partially populated vectors, and wrapped ring ownership. Tests also cover alignment, invalid pool alignment, and non-copyable ownership contracts.
- Verification: 71/71 native ASan/UBSan tests pass; ESP32-S3 firmware builds at 30,432 bytes RAM and 891,317 bytes flash.

## 2026-10-27 — HIST-01 and HIST-02

- Added a coherent 64-bit monotonic microsecond snapshot alongside the existing 32-bit modular clock. Multi-day history no longer depends on a counter that wraps every 71.6 minutes.
- History sample intervals, last-boundary timestamps, average sums, and counts now use widths suitable for every configured window. Invalid zero-sized configurations and accumulator overflow are explicit contract failures with safe post-failure state.
- Late ticks advance directly to the most recent sample boundary instead of creating a prolonged synthetic catch-up sequence.
- Since-sequence iteration tracks a bounded readable count rather than comparing wrapped sequence numbers. An overrun returns the retained ring exactly once, and `UINT32_MAX` sequence wrap remains valid.
- Tests cover the exact four-day/65-point interval, a 70,001-value high-rate average bucket, partial/full/overrun reads, and write-sequence wrap.
- Verification: generated-output check is clean; 75/75 native ASan/UBSan tests pass; ESP32-S3 firmware builds at 30,448 bytes RAM and 891,977 bytes flash.

## 2026-10-27 — UI-01

- Row-menu invalid capacity/name/index inputs now stop safely after reporting the contract failure. Empty menus cannot underflow selection, invalid reads return a safe value for recovery-capable host handlers, and null callbacks are replaced by inert fallbacks after assertion.
- Added real-LVGL simulator contract checks for direct selection, centered/clamped window movement, wraparound navigation, edit locking, value changes, and selection clearing.
- Chart catch-up inherits the bounded overrun and wrap-correct iterator proven under HIST-02; the complete flow exercises attachment, periodic updates, and history-window changes through the production adapter.
- Rebuilt and exercised all home, trend/menu, diagnostics-scroll, event-log, and state-log paths. Nineteen fresh 320x240 frames were reviewed together; no clipping, unintended scrollbar, padding, border, or chart-grid regression was found.
- Verification: desktop LVGL build and complete UI flow pass; ESP32-S3 firmware builds at 30,448 bytes RAM and 892,317 bytes flash.

## 2026-10-27 — APP-02 and input fail-safe cleanup

- Temperature samples now carry a generated 64-bit acquisition timestamp written by the physical controller. Regulation requires power, running/first-sample status, age at most three seconds, finite values, temperature from -20 to 80 °C, and humidity from 0 to 100%.
- The simulator can independently hold sensor transport status active while withholding new samples. Regression scenarios prove stale, NaN, and implausible readings force `InvalidInput`, heater off, and full safety fan.
- Target requests reject NaN/infinity and clamp to the supported 15–50 °C range. Non-finite manual fan speed becomes zero rather than entering actuator state.
- PID steps reject non-finite/non-positive timing without corrupting state. Conditional integration now discards only changes that drive farther into the active limit, so opposite-sign error can unwind a saturated controller.
- Removed the deliberate fatal assertion reached by a long encoder-button hold. Audible repeat feedback now saturates safely at 3 kHz, including counter arithmetic overflow protection.
- Verification: all nine generator regressions pass with resource warnings fatal; generated-output check is clean; 80/80 native ASan/UBSan tests pass; complete desktop UI flow passes; ESP32-S3 firmware builds at 30,480 bytes RAM and 893,273 bytes flash.

## 2026-10-27 — APP-03 independent safety supervisor

- Added a separate final heater-permission layer after the normal fermentation PID/FSM. Transient sensor availability/freshness/plausibility faults veto heat; an absolute 60 °C reading and persistent electrical output while commanded off latch until process restart.
- The command/measurement check compares the previous final heat command with measured heater voltage/current. Provisional thresholds are 1.0 V or 0.1 A continuously for two seconds, allowing filtered measurements to decay after a legitimate off transition.
- Any supervisor fault forces heater duty to zero and requests full fan cooling, including an electrical mismatch detected while regulation is already off. Invalid/non-finite/out-of-range heater commands are also vetoed.
- Generated behavior bindings now expose `HeatpadSample` to the supervisor. Generated `FermentationStatus` publishes a typed `SafetyFault`, and the scrollable diagnostics climate card displays it without dropping existing fields.
- Direct tests cover transient recovery, absolute-limit latching, timed mismatch latching, and invalid-command veto. End-to-end simulation proves both latch classes control the real command route and persist across otherwise-valid inputs.
- Verification: generated-output check is clean; 86/86 native ASan/UBSan tests pass; complete desktop LVGL flow passes and refreshed diagnostics captures were visually reviewed; ESP32-S3 firmware builds at 30,512 bytes RAM and 894,157 bytes flash.
- Electrical thresholds, ADC behavior, actual off polarity, cooling policy, and the independent physical cutoff remain hardware-gated under APP-04/SAFE-01.

## 2026-10-27 — generator input-boundary hardening

- Configuration-controlled component subdirectories are now constrained to normalized relative paths inside the staging tree, closing a path-escape hole in the otherwise transactional workflow.
- Event, state, component, hardware, tick, field, and GUI identifiers reject suffix collisions and reserved C++ keywords before rendering. Reference lists reject duplicates, invalid sentinel use, and malformed values.
- Literal rendering rejects non-finite floats and unsafe enum expressions, while C++ strings are escaped without changing valid UTF-8 UI text. Empty event/state declarations now receive complete parsed metadata.
- XML parsing disables networking, DTD loading, and entity resolution and rejects document types explicitly. XML-derived object, component, GUI, and resource names must be valid generated C++ identifiers.
- Explicit tick orders can safely coexist with implicit orders, hardware sections are shape-checked, renderer and include values are validated, and absent LEDC configuration no longer crashes preprocessing.
- Verification: 16 generator regressions pass, including path traversal, XML entity, invalid literal/name, sentinel, and mixed tick-order cases; generated-output check is clean.

## 2026-10-27 — reusable function and lightweight-wrapper hardening

- Moving-average, composite, lookup, and sampled functions now release owned storage normally and cannot be copied or moved into aliased ownership. `ColorMap` remains explicitly non-owning and no longer raises a fatal error at ordinary teardown.
- Moving-average accumulation uses 64 bits; fixed-point conversion, rescaling, sample domains, interpolation data, calibration points, thresholds, and filter inputs reject non-finite or unrepresentable values without continuing into undefined arithmetic.
- Fixed dormant math defects: constant functions now retain their constructor value, square root no longer calls cube root, and 90/180/270-degree sine factories use the advertised phase offsets.
- Removed destructor failures whose only rationale was that `std::function` may allocate. Component bases now have a virtual normal destructor; FSM buttons, FSM storage, and the ST7789 callback wrapper tear down normally.
- RGB LED wrappers now own and release their NeoPixel allocations, reject use before successful initialization, retain the current single-pixel color correctly, avoid filling an entire strip once per pixel, and reject non-finite brightness conversion.
- Verification: 94/94 native ASan/UBSan tests pass; the desktop LVGL simulator and complete 19-step UI flow pass; the ESP32-S3 firmware builds successfully at 30,608 bytes RAM and 895,217 bytes flash. Task/DMA/display objects with asynchronous hardware ownership remain process-lifetime and require coordinated target teardown work rather than speculative destructors.

## 2026-10-27 — scheduler, dirty-dispatch, and PWM boundaries

- Dirty-dispatch registration and indexing now reject null, exhausted, and out-of-range requests without continuing into storage. Dispatch processes a bounded snapshot, so callback-generated dirtiness is deferred to the next pass instead of being lost or forming an unbounded self-requeue loop.
- Empty time-slot schedulers are inert after reporting their invalid configuration; diagnostic indexing has a safe recovery result, slot indexes no longer truncate at 255, timing maxima are unsigned, and reported frequency is normalized by the actual elapsed measurement window.
- Software PWM rejects NaN/infinite duty values and places invalid constructor values into a deterministic heater-off fallback instead of converting NaN to an integer threshold.
- LED and piezo task startup now returns after invalid initialization/frequency contracts, and tick delays have a one-tick floor so frequencies above the RTOS tick rate cannot create a zero-delay busy loop. Stop-handler registration similarly rejects null/capacity failures without writing past its fixed array.
- Verification: 98/98 native ASan/UBSan tests pass. The ESP32-S3 firmware builds successfully at 30,600 bytes RAM and 895,581 bytes flash. Physical task cadence remains hardware-gated.
