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
