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
