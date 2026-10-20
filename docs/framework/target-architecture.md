# Proposed target architecture

## Supported project family

The framework will target small-to-medium ESP32-S3/Arduino-ESP32 appliances and instruments with:

- periodic sensors and actuators;
- one main application task with ordered phases;
- a modest number of typed events and shared states;
- mutually exclusive operating behaviours and screens;
- optional LVGL UI on a small SPI display;
- bounded asynchronous worker tasks for device services.

It will not attempt to become a portable RTOS, general actor framework, dynamic plugin system, or cross-MCU HAL. Supporting a materially different platform should begin with a demonstrated second application, not anticipatory abstractions.

## Retain

### Declarative topology and generated narrow interfaces

Keep YAML declarations for component relationships, typed event/state definitions, phase participation, and direct routing. They eliminate meaningful boilerplate and make application dependencies reviewable.

### Single-owner application runtime

Keep event routing, shared-state publication, mode changes, and semantic UI updates in one application task. This avoids pervasive locking and makes phase latency understandable.

### Staged shared state

Keep pending/current/previous values and coalesced publication. Correct the pending API and document dispatch visibility precisely.

### Bounded event cascades

Keep a configurable bound to catch feedback loops, but exhaustion must be a deterministic fatal contract violation before pool reuse—not a partial continuation with dangling pointers.

### Ordered tick phases

Keep generated phase ordering and dispatch boundaries. Add overrun/deadline instrumentation and tests rather than replacing it with a more general scheduler.

### LVGL compiler and asynchronous display pipeline

Keep the XML-to-typed-C++ flow and render/sender/DMA separation. First correct stride, buffer identity, failure, and shutdown behavior; then benchmark before considering structural changes.

### Fixed-capacity utilities where the bound is meaningful

Keep static vectors/rings/bitsets/pools after correcting storage and lifetime semantics. Prefer standard containers for host-only tooling and unbounded data.

## Change

### Events

- Separate mutable construction from const routed views.
- Enforce payload lifetime constraints or destroy constructed blocks.
- Expose explicit allocation/queue failure policy.
- Assert quiescence at dispatch-limit exhaustion before pool reset.
- Document bypass/immediate routing and prevent unsafe retained views.

### State

- Generate correct pending getters.
- Test publication ordering, coalescing, callback-generated updates, and defaults.
- Enforce main-task mutation in debug target builds.

### Time

- Retain 32-bit modular timestamps for intervals below the documented limit.
- Use 64-bit duration/accumulator types for history and other long horizons.
- Inject/fake the clock in host tests without spreading dependency injection through application code.
- Define missed-period behavior: catch up, skip, or coalesce per utility.

### Containers and resource ownership

- Make public headers self-contained.
- Use normal destructors and correct object lifetimes.
- Add compile-time constraints where a type is intentionally trivial-only.
- Preserve fixed-capacity failure behavior but return/propagate it coherently.
- Remove claims that heap-backed variants perform no allocations.

### Synchronization

- Distinguish normal and recursive mutex guards/types.
- Never use task semaphore APIs from ISR context.
- Document lock ordering for nested subsystems.
- Avoid holding locks across user callbacks unless reentrancy is part of the contract.

### Generator

- Add a minimal locked dependency set.
- Validate all inputs before writes.
- Generate into staging directories, then replace outputs only after success.
- Add `--check` mode and deterministic golden fixtures.
- Track source assets.
- Keep generated/handwritten mixing only where marker replacement is validated and atomic.

### Display

- Construct draw buffers through the public LVGL API.
- Compute RGB565 stride from color format and validate size.
- Track each buffer through named ownership states.
- Propagate transport failure and make shutdown bounded and defined.
- Preserve partial rendering/triple buffering unless benchmarks justify a change.

### Fatal safety

- Add a minimal application-provided emergency-safe-output hook invoked before task shutdown.
- Make it idempotent and allocation-free.
- Keep generic framework fatal handling separate from Garbox-specific LED/piezo reporting.

## Remove or narrow

- Remove fatal destructors as an ownership strategy.
- Remove duplicate or unused configuration constants when one generated source of truth exists.
- Narrow APIs that pretend to support arbitrary non-trivial types without implementing their lifetimes.
- Do not extend unused LVGL wrapper/DSL surface; fix and test the subset needed by current screens.
- Do not extract every hardware wrapper into a standalone package yet. A documented template/example is enough until another real project exercises reuse.

## Intended dependency direction

```text
example or Garbox application
        │
generated application contracts
        │
framework runtime + selected modules
        │
small platform boundary interfaces
        │
Arduino-ESP32 / ESP-IDF / FreeRTOS / LVGL
```

Application policy may depend on generated types. Reusable core code may depend on generated contract headers where generation supplies the application vocabulary, but must not include Garbox-specific controllers, sensors, or IDs beyond those formal generated interfaces.

## Success criterion

The framework is successful if a second similar ESP32 project can start by copying a documented minimal skeleton, declaring its topology, generating code, compiling host contract tests, and building firmware without deciding anew how events, state publication, ticks, screens, or display transport are wired.

It is not successful merely because Garbox builds, nor must it remove every application-specific decision.
