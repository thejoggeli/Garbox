# Framework stabilization program

This folder tracks the work to turn the reusable parts of Garbox into a dependable starting point for similar ESP32 projects while keeping Garbox working.

The controlling documents are:

1. [Baseline](baseline-2026-10-20.md) — exact starting revision, dirty-tree boundary, tools, builds, generation, sizes, and unavailable measurements.
2. [Architecture and ownership](architecture-ownership.md) — current execution, data, lifetime, synchronization, and display-buffer contracts.
3. [Verified review](verified-review.md) — severity-ranked findings with evidence confidence.
4. [Target architecture](target-architecture.md) — what stays, changes, or is deliberately not generalized.
5. [Implementation plan](implementation-plan.md) — task IDs, dependencies, acceptance criteria, verification, progress, and hardware gates.
6. [Verification log](verification-log.md) — dated commands, results, resource usage, and remaining validation boundaries.

The broader application and subsystem descriptions remain in the parent documentation:

- [Project overview](../project-overview.md)
- [Architecture and code generation](../architecture.md)
- [Custom LVGL stack](../lvgl-stack.md)
- [Engineering review](../review.md)
- [Fermentation-box finishing plan](../finishing-plan.md)

## Evidence labels

Findings use three confidence labels:

- **Reproduced** — observed by a build, test, deterministic comparison, or other executable check.
- **Established by source/API contract** — follows directly from the checked-in source and the pinned dependency API. It still may need a target regression test.
- **Target validation required** — behavior, timing, electrical polarity, memory headroom, or performance cannot be established without the physical ESP32/display/appliance.

## Change policy

- Preserve pre-existing user work and keep framework changes isolated.
- Fix generator templates rather than generated files alone.
- Add a failing regression test before or with each behavioral fix where host testing is practical.
- Preserve the display architecture until its correctness and baseline performance are known.
- Do not energize actuators as part of generic framework validation.
- Record software verification separately from target verification.
