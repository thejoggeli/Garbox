# Current architecture and ownership map

## System boundary

Garbox currently combines four layers:

```text
Garbox policy and screens
        │
generated application topology and typed contracts
        │
reusable runtime, utilities, UI stack, and device modules
        │
Arduino-ESP32 / ESP-IDF / FreeRTOS / LVGL / physical hardware
```

The reusable target is not a board-independent library. It is a starting architecture for ESP32/FreeRTOS devices that have periodic sensing and actuation, a small event/state-driven application, and optionally a local LVGL display.

## Startup and construction ownership

`setup()` is the composition sequence:

1. initialize logging and assertion callbacks;
2. install the GPIO ISR service;
3. initialize generated peripheral singletons;
4. construct/initialize part singletons through `PartsProvider`;
5. initialize LVGL/display dimensions and start auxiliary tasks;
6. run startup feedback;
7. construct and initialize the generated runtime, all components, all states, and all GUI trees;
8. create `MainTask`, which starts and runs the application forever.

Most objects have static process lifetime. Several heap-using types deliberately fail in their destructors rather than implement cleanup. That is current behavior, not the target ownership model.

## Task ownership

| Context | Owns/mutates | Synchronization contract |
|---|---|---|
| Arduino task | Startup/composition only after boot; idle `loop()` afterward | Must not enter the application runtime after `MainTask` starts. |
| `MainTask` | Runtime context, component lifecycle, active mode/screen, event creation/queueing/routing, shared-state mutation/publication, screen semantic updates | Intended sole owner; currently documented only implicitly. |
| LED task | Animated LED progression | Normal group mutex; per-LED recursive mutex for composed animation operations. |
| Piezo task | Tone queue playback and PWM updates | Recursive mutex because public operations call locked helpers. |
| Display render task | `lv_timer_handler()` and LVGL pixel production | Serialized against main-task LVGL mutation by render-ready/trigger handshake. |
| Display sender task | ST7789 window commands and one asynchronous pixel stream at a time | Queue plus DMA-completion barrier. |
| SPI DMA task | ESP-IDF completion polling/routing and transaction-slot release | Normal SPI mutex and transaction queue; callbacks run after unlock. |
| GPIO ISR | Frequency-sensor edge timestamps/flags | Port critical section and volatile fields; only ISR-safe operations allowed. |

The intended framework contract is: application events, states, lifecycle callbacks, mode changes, and semantic screen updates belong to `MainTask`. No component callback from an auxiliary task or ISR may mutate them directly. Cross-task inputs must be transferred through an explicitly synchronized mailbox/queue and consumed by `MainTask`.

## Synchronization and lock order

`MutexGuard` is only for handles created by `xSemaphoreCreateMutex()`; `RecursiveMutexGuard` is only for handles created by `xSemaphoreCreateRecursiveMutex()`. Both are task-context APIs and fail fatally on null handles or failed take/give operations. ISR code must instead use an ISR-safe critical section or the appropriate FreeRTOS `...FromISR` primitive.

The audited mutex map is:

| Owner | Kind | Reason |
|---|---|---|
| Profiler | Normal | All operations take one lock; reset no longer calls other locked public methods. Records are returned as value snapshots. |
| Animated LED group | Normal | Group operations do not reacquire the group mutex. |
| Static LED | Normal | Each operation has a single critical section. |
| Display buffer tracker | Normal | Each identity transition is atomic and the lock is released before ST7789/SPI calls. |
| SPI DMA | Normal | Slot allocation/release occurs under one lock; completion callbacks are copied and invoked after unlock, so chunk chaining may re-enter `transferAsync()`. |
| Animated LED | Recursive | Public animation operations intentionally compose locked helpers such as `animationClear()`, `animationStart()`, `stop()`, and `enterState()`. |
| Piezo player | Recursive | Queue/playback operations intentionally compose `playNextInQueue()`, `clearQueue()`, and `stop()`. |

Observed nesting is group → profiler → animated LED in the LED tick, and piezo → profiler in playback. Neither path has a reverse acquisition. SPI completion invokes callbacks without its mutex, and display code never holds the tracker mutex while entering SPI. Assertion handlers are registered during single-threaded startup, are task-context only, and use an atomic first-fatal latch rather than an uninitialized mutex.

## Component lifecycle

Controllers are always enabled. Exactly one behaviour and one screen are active. Runtime construction creates all component instances. `init()` injects hosts and calls `onInit`; `start()` calls `onStart`; enable/disable hooks follow mode changes.

Current lifecycle callbacks can emit events or change state because the host is already available during component initialization and activation. The target contract is:

- constructors: no events, states, hardware effects, or task starts;
- `onInit`: configure internal dependencies; state writes are allowed but must be dispatched before normal operation; queued events require an explicit initialization-dispatch policy;
- `onStart`: normal event/state emission is allowed on `MainTask`;
- enable/disable: may emit events/state on `MainTask`; changes become visible at the next dispatch boundary;
- ticks and routed callbacks: may emit events/state, subject to bounded dispatch rules;
- destructors/stop: must not emit application work.

This policy is not yet fully enforced.

## Event ownership and lifetime

An `EventBlock<E>` contains a header followed by the generated payload. `EventFactory` constructs blocks in a bump-allocated heap pool. Callers receive a non-owning `EventView<E>` and the runtime queues header pointers.

Current intended lifetime:

```text
make event → caller initializes payload → receiveEvent queues pointer
           → dispatch routes pointer → dispatch cascade reaches quiescence
           → event pool releaseAll invalidates every event/view
```

Current behavior and remaining ambiguities:

- stable completion resets the pool; exhausting the configured pass bound with queued work raises a fatal contract violation first and retains the pool, so recovery-capable test handlers cannot observe dangling pointers;
- allocation failure returns a view containing null pointers, but callers generally dereference it immediately;
- a `const EventHeader*` can be converted to a mutable view with `const_cast`, so post-send immutability is not enforced;
- immediate/bypass routing can create nested routing and shares the same pool lifetime;
- pool reset does not destroy non-trivial payloads.

Remaining target contract: event construction is mutable only before submission; routing receives const views; event payload types must satisfy an enforced lifetime rule; allocation/queue overflow must fail deterministically without leaving live dangling pointers.

## Shared-state ownership and visibility

Each generated state has `mNext`, `mCurrent`, and `mLast`:

```text
writer setter → mNext → mark dirty once
dispatchStates → mLast = mCurrent; mCurrent = mNext; clear dirty
              → route state-changed callbacks
```

Normal getters read published `mCurrent`, pending getters read staged `mNext`, and previous getters read `mLast`. The pending getter contract is covered by a generated-state host regression.

The configuration assigns one writer and zero or more readers. Generated component state bundles enforce that topology at compile time. State mutation/publication belongs to `MainTask`; auxiliary tasks must not write generated state directly.

Changes made while routing state callbacks enter the dirty queue for a later cascade pass. Multiple writes before publication coalesce into one notification.

## Tick and time model

The generated tick runner executes ordered phases on a nominal 33 ms period and dispatches state/events after each phase. The phase boundary is the application visibility boundary.

`Time::GetMicros()` truncates the ESP timer to `uint32_t`, wrapping every 2^32 microseconds (about 71.6 minutes). `SoftwareTimer` uses unsigned subtraction and is valid across a wrap provided it is observed before a complete counter lap becomes indistinguishable. Restart skips missed periods in constant work while retaining the most recent period boundary. Millisecond and second snapshots wrap on much longer periods.

`SoftwarePwm` applies configuration made while stopped to its first cycle. While running, `finishCurrent=true` stages duty/period changes for the next natural boundary; `finishCurrent=false` starts a new cycle at the call time. Phase modes and the 0%/100% constant-output cases follow the same timer contract.

Current time contracts are inconsistent: most timers use 32-bit microseconds, while advertised history windows exceed what 32-bit microseconds can represent. The target is to keep 32-bit modular time for short real-time intervals and use 64-bit duration/accumulation types for multi-hour/day data.

## Container and allocation ownership

The custom storage/container family separates storage from algorithms:

- `StaticStorage<T,N>` embeds aligned bytes;
- `HeapStorage<T>` allocates raw bytes;
- arrays construct all elements;
- vectors/ring buffers construct elements on insertion;
- data pools perform aligned bump allocation.

Current contracts are incomplete:

- `StaticStorage::capacityBytes()` is wrong;
- multiple headers are not self-contained;
- destruction/lifetime behavior is unsafe or implicit for non-trivial types;
- `HeapStorage` deliberately fails instead of freeing;
- claims of fixed capacity do not imply no heap allocation, because heap-backed variants and `std::function` are widely used.

The target is conventional RAII plus explicit fixed-capacity semantics. Types that intentionally only support trivial elements must state and enforce that with compile-time constraints.

## Display buffer ownership

The enforced ownership states for each LVGL partial buffer are:

```text
LVGL-owned (Reusable/Rendering) → Queued → DMA-owned → LVGL-owned
```

- **Reusable:** LVGL may select the buffer for a draw operation.
- **Rendering:** LVGL software renderer is writing the dirty region.
- **Queued:** `{rect, pointer}` is in the three-entry sender queue.
- **DMA-owned:** the sender has set the ST7789 window and the chunk sender is still reading the buffer.
- **Reusable:** final DMA completion has released chip-select and returned ownership.

`DisplayBufferTracker` registers the three exact allocation addresses and enforces the observable transitions. Its fixed-capacity FIFO must agree with the FreeRTOS send queue, only its head may become DMA-owned, only one DMA owner may exist, and completion must name that same buffer. Unknown, duplicate, out-of-order, and repeated transitions are fatal invariant violations. A normal mutex protects the tracker across the render, sender, and SPI completion tasks; the counting semaphore remains responsible for blocking/backpressure. The tracker deliberately treats LVGL's internal reusable/rendering distinction as one `LvglOwned` state because there is no driver callback at the start of software rendering.

Application-side LVGL object mutation occurs in `MainTask` only after taking the render-ready token. It finishes before triggering the display render task. `lv_timer_handler()` then runs alone. Because LVGL is configured with `LV_OS_NONE`, this handshake is the entire thread-safety contract.

The RGB565 buffer contract is `stride = lv_draw_buf_width_to_stride(width, LV_COLOR_FORMAT_RGB565)` and `data_size >= stride * height`. `LvglHandler` now computes the two-byte RGB565 geometry independently, verifies it against LVGL's public stride helper, and initializes each buffer through `lv_draw_buf_init()`. For the configured 320 by 60 partial buffers this is a 640-byte stride and 38,400-byte allocation. Physical color, orientation, and sustained-transfer validation remains hardware-gated.

## Failure and shutdown ownership

At baseline, fatal handling stops registered tasks and produces LED/piezo feedback, but it does not synchronously force the heater and fan GPIO/PWM outputs to safe states before task shutdown. Display and SPI shutdown also leave in-flight work unresolved.

Target ordering for fatal shutdown is:

1. atomically latch fatal state and prevent new application/output commands;
2. force hazardous actuators to their hardware-defined safe outputs using a minimal non-allocating path;
3. stop/abort auxiliary activity with bounded waits where safe;
4. report the failure using only known-safe indicators;
5. watchdog reset or remain latched according to the product policy.

The exact electrical safe levels and watchdog/reset policy require target confirmation.
