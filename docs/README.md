# Garbox project documentation

This documentation is a snapshot of the repository as reviewed on 2026-10-19. It describes the current working tree, including the uncommitted hardware, encoder, display, and SHT31 changes that existed when the review began.

## Reading order

1. [Project overview](project-overview.md) — what the device and every major repository area can do.
2. [Architecture and code generation](architecture.md) — how control, state, events, generated routing, hardware, tasks, and UI fit together.
3. [Custom LVGL stack](lvgl-stack.md) — the XML UI compiler, typed C++ view layer, dirty rendering, widgets, buffering, tasks, panel driver, and SPI-DMA pipeline.
4. [Engineering review](review.md) — strengths, risks, concrete defects, and maintainability assessment.
5. [Finishing plan](finishing-plan.md) — a staged path from the current prototype to a dependable fermentation box.
6. [Framework stabilization program](framework/README.md) — verified baseline, ownership contracts, target architecture, and executable task plan for reuse in future ESP32 projects.

## Executive summary

Garbox is already a substantial ESP32-S3 firmware, not merely a half-finished appliance sketch. The repository contains:

- a declarative YAML/XML-to-C++ generator;
- a typed event/state/component application framework;
- reusable ESP32 hardware abstractions and device drivers;
- fixed-capacity containers, control, timing, color, filtering, history, and diagnostics utilities;
- a full custom LVGL-facing stack: XML UI DSL/compiler, generated typed object trees and assets, C++ object façade, semantic dirty rendering, custom menus/charts/rotation helpers, triple-buffered partial rendering, ST7789 driver, and chunked SPI-DMA transport;
- a real fermentation application with temperature sensing, heater PWM, fan control and stall detection, rotary input, multiple screens, charts, status LEDs, piezo feedback, and diagnostics;
- Python thermal simulations used to explore control strategies.

The current ESP32 firmware builds successfully. A fresh generator run reproduces the current generated output exactly. The largest remaining problem is not architecture: it is turning an ambitious framework prototype into a safety-reviewed, tested appliance. Several control and runtime defects should be fixed before unattended heater operation; the most important are summarized in [Engineering review](review.md).

The main recommendation is to freeze framework expansion temporarily. Preserve the architecture, fix the correctness and safety issues, add tests around the fermentation engine and runtime, validate the hardware failure modes, and only then complete the user experience.

