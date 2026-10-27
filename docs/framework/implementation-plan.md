# Implementation plan

Status values: `TODO`, `IN PROGRESS`, `SOFTWARE VERIFIED`, `HARDWARE BLOCKED`, `DONE`, `DEFERRED`.

The order below is dependency-driven. Necessary correctness/reproducibility work precedes optional cleanup.

## Milestone 0 — Baseline and boundaries

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| M0-01 | DONE | — | Record revision, protected dirty files, tools, builds, dependencies, size, generation, and unavailable measurements. | Baseline document contains commands/results and separates target gaps. |
| M0-02 | DONE | — | Map lifecycle, task/data ownership, event/state lifetime, allocation, display buffers, and fatal shutdown. | Ownership document states current and target contracts. |
| M0-03 | DONE | M0-01, M0-02 | Classify findings by severity and confidence. | Ledger uses reproduced/source-contract/target-validation labels. |
| M0-04 | DONE | M0-02, M0-03 | Decide target scope and retained/changed/removed mechanisms. | Target architecture explicitly limits supported project family. |

## Milestone 1 — Host build and container foundation

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| BUILD-01 | SOFTWARE VERIFIED | M0 | Make public headers self-contained and add a header-compilation check. | Native environment compiles; representative public headers compile alone. |
| CNT-01 | SOFTWARE VERIFIED | BUILD-01 | Correct static byte capacity and test element/byte capacities across element sizes. | Tests fail on old expression and pass with `N*sizeof(T)`; sanitizer run has no OOB. |
| CNT-02 | SOFTWARE VERIFIED | BUILD-01 | Audit vector/array/ring/pool construction, release, copying, alignment, and destruction. | Boundary and non-trivial probe tests define supported behavior; unsupported types get `static_assert`. |
| LIFE-01 | SOFTWARE VERIFIED | CNT-02 | Replace fatal heap-storage/container destruction with RAII in reusable core. | Host teardown succeeds under ASan/UBSan with no leak attributable to tested types. |
| TEST-01 | SOFTWARE VERIFIED | BUILD-01 | Add fake-clock and assertion test support usable by time/runtime tests. | Tests control time deterministically and can observe expected contract failures. |

## Milestone 2 — State and event contracts

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| STATE-01 | SOFTWARE VERIFIED | BUILD-01 | Fix pending getters in the state template and regenerate. | Generated fixture proves pending/current/last before and after publication; generator output clean. |
| EVT-01 | SOFTWARE VERIFIED | CNT-01, TEST-01 | Prevent pool release with queued events; define dispatch-limit behavior. | Cascade tests cover exact limit, one beyond limit, stable completion, and no dangling queue pointers. |
| EVT-02 | SOFTWARE VERIFIED | EVT-01 | Define allocation/queue exhaustion and invalid-view behavior. | Tiny pool/queue tests produce one documented deterministic outcome without null dereference. |
| EVT-03 | SOFTWARE VERIFIED | EVT-01 | Enforce post-submit immutability and payload lifetime restrictions. | Routed handlers receive const payloads; compile-time or runtime tests cover prohibited retention/type. |
| STATE-02 | SOFTWARE VERIFIED | STATE-01, TEST-01 | Test coalescing, callback-generated state changes, initialization, and visibility boundaries. | Contract tests exercise pending/current/last and multi-pass publication. |
| MODE-01 | SOFTWARE VERIFIED | TEST-01 | Clear/reject redundant screen and behaviour requests. | Repeated same-mode request is consumed once and does not log/apply forever. |

## Milestone 3 — Time, PWM, and history

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| TIME-01 | SOFTWARE VERIFIED | TEST-01 | Specify/test modular timer wrap, zero duration, missed periods, and long stall behavior. | Boundary tests cross `UINT32_MAX`; no unbounded catch-up loop for supported inputs. |
| PWM-01 | SOFTWARE VERIFIED | TIME-01 | Fix pre-start/first-cycle and immediate/deferred PWM reconfiguration. | Fake-clock tests cover 0%, 100%, both phase modes, pre-start changes, mid-cycle changes, and restart. |
| HIST-01 | SOFTWARE VERIFIED | TIME-01, CNT-02 | Use safe interval, sum, and count widths for every advertised window. | Four-day simulated test at maximum configured input avoids overflow and samples at expected boundaries. |
| HIST-02 | SOFTWARE VERIFIED | HIST-01 | Fix since-sequence overrun and sequence wrap semantics. | Tests cover empty/partial/full ring, consumer overrun, ring wrap, and sequence wrap. |

## Milestone 4 — Synchronization and shutdown

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| SYNC-01 | SOFTWARE VERIFIED | BUILD-01 | Split normal/recursive mutex guards or normalize creation; audit nested calls. | Creation/take/give APIs match; recursive call paths are tested or structurally eliminated. |
| ISR-01 | SOFTWARE VERIFIED | BUILD-01 | Remove duplicate ISR registration and document ISR-safe contract. | One checked registration; firmware builds; target tachometer smoke test remains gated. |
| SAFE-01 | IN PROGRESS | SYNC-01 | Add idempotent emergency-safe-output hook before task shutdown. | Software test verifies ordering and idempotence; electrical levels remain hardware-gated. |
| TASK-01 | SOFTWARE VERIFIED | SYNC-01 | Define task stop ownership and eliminate unsafe task-wrapper destruction/forced deletion. | Stop is idempotent; current-task and in-flight-worker cases are documented/tested where possible. |

## Milestone 5 — Generator and reproducible workflow

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| GEN-01 | SOFTWARE VERIFIED | BUILD-01 | Add minimal generator dependency manifest and root-independent command. | Clean virtual environment can install requirements and generate. |
| ASSET-01 | SOFTWARE VERIFIED | GEN-01 | Track source GUI assets and define generated-image policy. | Fresh checkout has everything needed to generate and link images. |
| GEN-02 | SOFTWARE VERIFIED | GEN-01 | Validate then generate into staging; atomically apply outputs. | Injected template/resource failure leaves tracked outputs byte-identical. |
| GEN-03 | SOFTWARE VERIFIED | GEN-02 | Add `--check`/diff mode and deterministic fixtures. | CI-style command exits zero when clean, nonzero with readable diff when stale. |
| GEN-04 | SOFTWARE VERIFIED | GEN-03 | Test/fix GUI preprocessing, mappings, resources, and malformed config. | Golden/negative tests cover current DSL subset and known dormant defects. |
| FLOW-01 | SOFTWARE VERIFIED | ASSET-01, GEN-03 | Add one documented checkout → generate/check → host test → firmware build command sequence. | Sequence succeeds without relying on undocumented local state. |

## Milestone 6 — Display correctness before restructuring

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| DISP-01 | SOFTWARE VERIFIED | BUILD-01 | Replace manual RGB565 stride/buffer initialization with public LVGL API and validated size calculations. | Compile-time/unit checks prove 320px stride is 640 bytes and 60-row buffer is 38,400 bytes; firmware builds. |
| DISP-02 | SOFTWARE VERIFIED | DISP-01, TEST-01 | Model buffer identity/state transitions and validate queue/DMA ownership. | Host model covers partial flushes, three in flight, ordered completion, duplicate/wrong completion, and reuse. |
| DISP-03 | SOFTWARE VERIFIED | DISP-02, SYNC-01, TASK-01 | Propagate transport failure; define screen-change and shutdown behavior. | Failure injection reaches defined fault state, releases/invalidates ownership safely, and does not deadlock. |
| DISP-04 | HARDWARE BLOCKED | DISP-03 | Benchmark unchanged architecture and corrected implementation. | Full/small/chart/load metrics plus CPU, skipped frames, heap, stack, and main latency are recorded comparably. |
| UI-01 | TODO | BUILD-01 | Fix/test row-menu selection and chart adapter edge cases. | Host tests cover selection/window/lock and history catch-up. |

## Milestone 7 — Garbox safety/correctness retention

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| APP-01 | SOFTWARE VERIFIED | STATE-01, PWM-01 | Add host tests for fermentation state/output transitions and startup-disabled policy. | Deterministic tests cover invalid input, overtemperature, cooldown, enable/disable, target changes, and a closed-loop plant. |
| APP-02 | IN PROGRESS | APP-01 | Correct safety-fan precedence, sensor freshness/validity, and PID anti-windup. | Safety-fan precedence is fixed and covered. Sensor freshness and PID recovery remain. |
| APP-03 | TODO | SAFE-01, APP-02 | Add independent software safety supervisor contract. | Tests cover absolute limit, stale/implausible sensor, and command/measurement mismatch; physical protection remains documented. |
| APP-04 | HARDWARE BLOCKED | APP-03 | Validate real sensor, fan tachometer, heater-off path, electrical limits, and failure behavior without uncontrolled heating. | Coordinated hardware checklist completed. |

## Milestone 8 — Reuse proof and delivery

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| EXAMPLE-01 | TODO | FLOW-01, EVT-03, STATE-02 | Add a minimal second-project example or starting procedure. | Example declares one controller/behaviour/screen, event, state, and phases; generation and host compile succeed. |
| CLEAN-01 | TODO | EXAMPLE-01 | Audit framework dependencies on Garbox-specific code and remove only demonstrated blockers. | Dependency report has no unexplained app-layer imports in reusable code. |
| DOC-01 | TODO | all software milestones | Finalize contract, workflow, retained/removed abstraction, limitation, and evidence docs. | A new checkout can follow docs; all commands/results and hardware gates are current. |
| FINAL-01 | HARDWARE BLOCKED | DOC-01, DISP-04, APP-04 | Final assessment with target evidence. | Separates software verification, hardware verification, and assumptions. |

## Desktop simulation track

The detailed design and acceptance criteria live in [desktop-simulation-plan.md](desktop-simulation-plan.md). The simulator substitutes semantic components rather than emulating ESP-IDF peripherals.

| ID | Status | Depends on | Work | Acceptance and verification |
|---|---|---|---|---|
| SIM-00 | DONE | M0-02, M0-04 | Define simulation boundary and evidence limits. | Architecture separates application simulation from target validation. |
| SIM-01 | SOFTWARE VERIFIED | TEST-01, STATE-01, EVT-02 | Deterministic headless runtime with real framework and fermentation behaviour. | Native scenarios boot, step virtual time, publish sensor state, and record typed commands. |
| SIM-02 | SOFTWARE VERIFIED | SIM-01, APP-01 | Thermal/fan plant, traces, and safety scenarios. | Deterministic startup/regulation/fault/cooldown scenarios and invariants pass. |
| SIM-03 | SOFTWARE VERIFIED | SIM-01 | Script semantic user and mode requests. | Direct typed requests and UI-generated requests exercise real routing and publish expected state. |
| SIM-04 | SOFTWARE VERIFIED | SIM-01, UI-01 | Run generated UI and custom LVGL stack against a host framebuffer. | All five real screens construct, render, and navigate using the shared embedded theme and generated state/event contracts. |
| SIM-05 | SOFTWARE VERIFIED | SIM-04 | Interactive desktop frontend. | A dependency-free local browser frontend streams the live framebuffer and maps encoder rotation plus short/long button presses to real input events. |
| SIM-06 | SOFTWARE VERIFIED | SIM-02, SIM-04 | Reproducible simulator workflow. | Commands and evidence are documented; generator check/CI integration remains tracked by GEN-03. |

## Optional refactoring, explicitly deferred

These are not required unless measurements or a real second application justify them:

- replacing generated direct routing with a dynamic bus;
- changing the three-task display pipeline;
- lazy construction of screens;
- extracting all core/modules into a separately versioned package;
- replacing the XML UI language;
- generalized multi-core application-state mutation;
- cross-MCU hardware abstraction.

## Immediate execution sequence

The first implementation slice is:

```text
BUILD-01 → CNT-01 → TEST-01
```

This slice is software-verified and remains hardware-neutral. It unlocks reliable host tests without changing application behavior or touching the protected hardware bring-up files. The next slice is `STATE-01` followed by the event cascade tests/fix.
