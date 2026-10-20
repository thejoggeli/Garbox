# The custom LVGL stack

## Executive summary

The display subsystem is much larger than a collection of LVGL wrappers. Garbox owns an end-to-end UI toolchain and runtime extending from declarative screen source to pixels on the ST7789 panel:

```text
XML screen DSL + PNG assets
          │
          ▼
Python parser / component expander / attribute compiler / image converter
          │
          ▼
generated, statically typed C++ object trees and image arrays
          │
          ▼
generated screen lifecycle + dirty render-function dispatch
          │
          ▼
handwritten screen behavior, menus, charts, grids, rotated labels
          │
          ▼
C++ LVGL object façade
          │
          ▼
LVGL 9.4 layout, invalidation, and software rendering
          │
          ▼
custom tick/theme/log/display adapter and three partial draw buffers
          │
          ▼
FreeRTOS render/sender pipeline
          │
          ▼
ST7789 command driver → chunked asynchronous SPI DMA → physical panel
```

LVGL itself supplies the widget/layout/rendering engine. Almost every layer needed to make that engine usable as a Garbox product UI is project-owned. This is one of the largest and most sophisticated subsystems in the repository.

## Scope

The stack spans several repository areas rather than one directory:

| Area | Responsibility |
|---|---|
| `codegen/config/gui/` | XML screen definitions and local reusable components. |
| `res/icons/` | Source PNG resources used by those definitions. |
| `codegen/src/loader/gui/` | DSL preprocessing, parsing, component expansion, resource resolution, and attribute compilation. |
| `codegen/src/generate/gen_guis.py` | C++ object-tree and image-resource generation. |
| `codegen/src/lvgl/LVGLImage.py` | PNG-to-LVGL image conversion, including multiple color formats and compression support. |
| `codegen/templates/gui/` | Templates for typed GUI classes, component structs, object construction, initialization, and screen activation. |
| `src/generated/guis/` | Generated C++ GUI trees for `MainScreen` and `SimpleScreen`. |
| `src/bin/images/` | Generated C arrays linked into the firmware as LVGL images. |
| `src/core/lvgl/objects/` | C++ façade over LVGL objects and widget-specific APIs. |
| `src/core/lvgl/helpers/` | Menu, chart, grid, time-series, buffer-rotation, and rotated-label facilities. |
| generated screen bases | Screen lifecycle, typed state/event integration, and dirty render dispatch. |
| handwritten screens | Dynamic presentation and user interaction. |
| `src/modules/parts/display/` | LVGL display adapter, buffer ownership, render/sender tasks, and ST7789 protocol. |
| `src/core/hardware/spi/` | Queued SPI DMA and large-transfer chunking. |
| `include/lv_conf.h` | The selected LVGL allocator, renderer, widgets, fonts, logs, assertions, and image decoder. |

The central GUI-specific source alone is several thousand lines, including the roughly 1,500-line image converter and roughly 1,500-line base object wrapper. That count does not include LVGL itself, generated GUI output, the screen framework, SPI implementation, or application screen code.

## 1. The XML UI language

The UI source is not merely XML-shaped calls to LVGL. It is a small declarative language with its own composition and value rules.

### Object trees

`<root>` contains a nested hierarchy of objects such as:

- `container`, `label`, `image`, `arc`, and `chart`;
- named objects that become stable C++ members;
- unnamed structural objects that receive generated private identifiers;
- parent-child relationships represented directly by C++ construction order.

The current screens use the language for percentage/pixel/content sizing, alignment, flex layouts, padding, margin, borders, opacity, text styling, transforms, arc styling, and image sources.

### Reusable XML components

Each GUI may define local components. `MainScreen.xml`, for example, defines `GraphComponent` and `InfoTile`. `SimpleScreen.xml` defines another `InfoTile` variation.

The shorthand has three important forms:

```xml
<InfoTile $icon $value $color>             <!-- required parameters -->
<Thing $color="#fff">                      <!-- optional parameter -->
<image src=$icon />                        <!-- bind a component value -->
<InfoTile $icon="fan16" $value="0%" />     <!-- pass a value at use site -->
```

The preprocessor rewrites these into XML-safe attribute names. The parser records parameter definitions and instance arguments. The resolver then expands component instances into flat object identities such as `fanInfo.icon` and `fanInfo.value`, while the generated C++ preserves the component as a nested struct.

This produces a useful combination:

- visual structure is authored compactly;
- repeated arrangements do not have to be copied;
- screen code gets direct, typed member access;
- runtime object lookup by string is unnecessary.

### Attribute compiler

XML attributes are compiled into wrapper method calls. Examples include:

```text
width="100%"                 → setWidth(LV_PCT(100))
height="content"             → setHeightContent()
align="top-right -4px 4px"   → setAlign(LV_ALIGN_TOP_RIGHT, -4, 4)
bg-opa="40%"                 → setBgOpa(102)
layout="flex"                → setLayout(LV_LAYOUT_FLEX)
src="fan16"                  → setSource(&MainScreen_image_fan16)
```

Lookup is widget-aware and falls back to base-object attributes. Unknown attributes fail generation instead of being silently ignored. Values have renderers for colors, strings, integers, percentages, pixel units, opacity, angles, zoom, sides, alignment, and multi-value spacing.

### Resource processing

The parser finds images and fonts referenced by the resolved object tree and prefixes their symbols with the GUI name. Used images are declared in the GUI header and converted from PNG into linkable LVGL C arrays.

The bundled image converter is capable of considerably more than the current screens use: multiple LVGL color formats, indexed palettes, alpha formats, stride adjustment, and compression paths are represented. Garbox currently requests `RGB565A8` for its icons and aligns generated image stride to one byte.

Fonts and image resizing are represented in the XML model but explicitly unfinished in the current implementation. Screen code therefore uses built-in LVGL fonts directly, and `resize="16x16"` metadata currently has no effect.

## 2. Generated static GUI model

The GUI generator produces a class such as `MainScreenGui` with two levels of structure:

- nested structs for reusable XML components;
- one `Objects` struct containing the complete root tree.

Constructors create the tree by passing each parent wrapper to its child wrapper. `init()` then applies the compiled attribute calls. `show()` loads the generated root as the active LVGL screen.

This model has several strong embedded properties:

- all application-visible object references have compile-time C++ types;
- no heap-backed application registry or name lookup is needed;
- hierarchy and construction order are deterministic;
- an XML typo in an attribute normally fails at generation or C++ compilation;
- screen logic can write `gui().tempGraph.value.setText(...)` rather than search the LVGL tree;
- XML layout and handwritten dynamic behavior remain separate.

All screen GUI objects are constructed when the generated runtime constructs its screens. They remain resident for the process lifetime; changing screens loads a different root rather than rebuilding the tree. That gives fast, stable screen switching in exchange for retaining every screen’s LVGL objects and buffers in memory.

There is an important startup invariant: `Display::init()` must call `lv_init()` before the runtime constructs any generated GUI. The current startup order satisfies it, but the C++ types do not enforce it.

## 3. C++ object façade

`LvObject` is a broad C++ façade over LVGL’s C object and style API. It covers:

- screen creation and activation;
- object hierarchy and z-order;
- raw and style-derived position and sizing;
- content sizing;
- flex and grid configuration;
- flags, visibility, clicking, and scrolling;
- transform size, pivot, rotation, scale, and skew;
- text and background styling;
- borders, padding, and margins;
- direct style properties for most LVGL visual categories;
- event callback registration and invalidation.

Specialized wrappers provide widget operations:

- `LvLabel`: text and formatted text;
- `LvImage`: image sources;
- `LvArc`: ranges, values, angles, rotation, modes, and arc-part styling;
- `LvChart`: series, point counts, ranges, update modes, values, and refresh;
- `LvBox`: a small structural specialization.

This API is intentionally close to LVGL. Its main value is not hiding how LVGL works; it is giving the generator a stable, uniform C++ target and giving handwritten screens the same vocabulary as generated layouts.

The wrapper is non-owning despite one contradictory header comment. Destruction does not delete the LVGL object; `free()` is explicit, and LVGL parent deletion remains responsible for child deletion. This fits permanent generated screen trees, but the ownership contract should be clarified before the types are reused for more dynamic UIs.

## 4. Higher-level UI facilities

Several parts go well beyond basic wrappers.

### Row menu

`RowMenu` and `RowMenuRow` implement an encoder-oriented menu with:

- a fixed-capacity row collection;
- selection and wraparound navigation;
- a locked edit mode;
- a moving visible-row window;
- separate active-label and active-value styling;
- callback-based value formatting and mutation;
- dirty flags so window and highlight work only runs when needed.

`MainScreenMenu` adapts this generic control to power, target temperature, history, fan mode, and placeholder actions.

### History charts

The chart stack has three layers:

- `TimeSeriesChartAdapter` bridges the project’s sequence-numbered `TimeSeries` ring buffers into LVGL chart series;
- `MultiSeriesChart` owns a fixed number of those adapters for one chart;
- `ChartGridRenderer` hooks LVGL draw events to paint custom grid lines and translucent tick labels over the chart.

The main dashboard uses two charts, three series, custom ranges and line styles, eight selectable history windows, two grid renderers, and dynamic time-axis labels. It can reload a new history series or append only samples written since the last render.

### Rotated text

`RotationRenderer` measures a label, renders it into a temporary RGB565 LVGL canvas, rotates the raw pixels with `BufferRotate`, and returns a persistent canvas/image wrapper containing the rotated result. The dashboard uses this for vertical graph labels.

This is a custom raster utility layered on LVGL’s drawing API, not merely a transform property.

## 5. Generated screen rendering model

GUI tree generation and application component generation meet in each generated `*ScreenAbs` class.

For every configured render function, the generated base provides:

- a virtual handwritten render hook;
- a `RenderFn` index;
- a registered trampoline;
- `markDirty()` and `isMarkedDirty()`;
- a `DirtyDispatcher` entry.

When a screen becomes active, its generated GUI root is loaded and every render function is marked dirty for a complete data refresh. State-change callbacks in handwritten code then mark only the affected regions/functions dirty. At the next display opportunity, the dispatcher invokes just those hooks.

For example, a temperature-state publication marks the measured-temperature and humidity functions, while a history selection marks the axis, attached time series, and menu. `MainScreen::onRender()` also incrementally feeds chart samples when a full series reattachment is not pending.

This is a two-level invalidation scheme:

1. the application dirty dispatcher decides which semantic UI computations must run;
2. LVGL’s own invalidation decides which pixels need repainting after those computations mutate objects.

That split is a particularly good part of the design. State-driven text/chart computation does not have to run on every 30 Hz application tick, and the screen logic remains understandable in terms of application concepts rather than rectangles.

## 6. LVGL configuration and adaptation

`lv_conf.h` selects LVGL 9.4 with:

- 16-bit color and byte-swapped RGB565;
- the software renderer;
- a single draw unit and `LV_OS_NONE`;
- C-library allocation;
- a 33 ms default refresh period;
- LVGL logs and important assertions;
- built-in Montserrat/UNSCII fonts;
- labels, images, arcs, charts, canvas, and other configured widgets;
- LodePNG support;
- native filesystem and platform display/input drivers disabled.

`LvglHandler` is the project-specific port:

- calls `lv_init()`;
- provides the project monotonic clock as LVGL’s tick source;
- maps LVGL logs into Garbox logging/assert behavior;
- creates the `lv_display_t`;
- installs flush and flush-wait callbacks;
- configures partial rendering;
- supplies three custom draw buffers;
- installs a global theme that removes default padding, border, radius, background, margins, and scrolling.

At 320×240×2 bytes with a partial factor of four, each buffer is 38,400 bytes (60 rows). Three buffers consume 115,200 bytes of internal DMA-capable heap. This dynamic cost is not visible in PlatformIO’s static RAM percentage and is worth measuring on hardware after every major UI expansion.

The global stripped theme makes XML layouts predictable: generated objects start nearly style-neutral, and the project explicitly opts into visible styling.

## 7. Render-to-panel pipeline

The runtime path is a carefully staged producer/consumer pipeline.

```text
MainTask / DisplayController
  │ take render-ready token
  │ run active screen's semantic dirty render functions
  │ give render trigger
  ▼
DisplayRenderTask
  │ lv_timer_handler()
  │ LVGL renders dirty areas into one of three 38.4 KiB buffers
  │ flush callback enqueues {rectangle, buffer pointer}
  ▼
three-entry flush queue + free-buffer counting semaphore
  ▼
DisplaySenderTask
  │ set ST7789 column/row window synchronously
  │ start pixel payload asynchronously
  │ wait for completion barrier
  ▼
SpiDmaChunkSender
  │ split payload into 4 KiB transactions
  ▼
SpiDma task / ESP-IDF SPI driver
  │ completion callbacks chain chunks
  ▼
ST7789 completion
  │ release chip select
  │ return draw-buffer token
  │ release sender barrier
  ▼
LVGL may reuse the buffer
```

The free-buffer semaphore provides backpressure between LVGL and transport. Its count, the flush queue capacity, and the number of draw buffers are all three. `flush_wait_cb` waits until at least one DMA transfer returns a token before allowing LVGL to reuse storage.

The controller records skipped frames when the previous `lv_timer_handler()` call has not finished. Rendering is therefore opportunistic rather than allowed to stall the main application tick.

Task priority is explicitly ordered because it affects visual integrity:

| Task | Priority | Role |
|---|---:|---|
| Display render | 1 | Run LVGL and produce partial buffers. |
| SPI DMA | 2 | Process asynchronous SPI transactions. |
| Display sender | 3 | Serialize window commands and pixel transfers. |

All three run on the same configured core. The main task has higher priority and initiates frames.

## 8. Panel and transport layer

`St7789v` is a project driver rather than LVGL’s platform integration. It owns:

- hardware reset timing;
- sleep-out, RGB565 color mode, memory orientation, inversion, and display-on setup;
- column and row address commands;
- synchronous command/small-data writes;
- synchronous or asynchronous pixel transfers;
- chip-select lifetime across an asynchronous transfer;
- gamma-corrected PWM backlight brightness;
- rectangle fill support for direct diagnostics.

Large pixel regions are sent through `SpiDmaChunkSender`, which chains bounded transfers and reports final success. The lower `SpiDma` abstraction owns the ESP-IDF transaction machinery and a dedicated task. This makes the display pipeline independent of a single-transfer size and keeps the framebuffer memory valid until the final chunk completes.

## 9. Concurrency contract

LVGL is configured with `LV_OS_NONE`, so the stack relies on application-level serialization rather than LVGL locks.

The current handshake is coherent:

1. the render task finishes `lv_timer_handler()` and gives `mRenderReady`;
2. the main task takes that token;
3. the active screen mutates LVGL objects through its dirty render functions;
4. the main task triggers the next `lv_timer_handler()` call;
5. no new screen update is allowed until the render task reports ready again.

This prevents the normal screen update path and LVGL rendering from running concurrently. It is, however, an implicit system rule: any future code that calls an LVGL or wrapper function from another task can violate it. The rule should be stated in code documentation and preferably guarded with task-context assertions or one centralized UI command boundary.

DMA transfer continues after `lv_timer_handler()` returns, but the buffer-count protocol prevents LVGL from reusing an in-flight draw buffer.

## 10. What is especially good

### It is vertically coherent

The DSL does not emit arbitrary LVGL calls; it targets the same wrapper API used by handwritten code. Generated screens participate in the same state/event framework as the rest of the application. The runtime buffer protocol matches LVGL partial rendering and the ST7789/SPI constraints. These pieces were designed together.

### It preserves static structure

Typed members, fixed-capacity helpers, direct callbacks, compile-time-generated routing, and permanent object trees are a good fit for an embedded UI. The system avoids runtime reflection and repeated object lookup without making the XML authoring experience primitive.

### It separates semantic and pixel work

State changes dirty semantic render functions. Those functions mutate objects. LVGL then performs pixel invalidation and drawing. SPI DMA operates behind buffer ownership barriers. Each layer has a distinct job.

### It includes genuinely custom UI machinery

The history adapter, multi-series charts, draw-event grid/labels, encoder menu, rotated text renderer, stripped theme, diagnostics, and skipped-frame tracking are real framework features. They are not thin aliases for built-in widgets.

### It already produces a nontrivial UI

The main dashboard demonstrates reusable XML components, images, flex composition, dynamic labels, editable menu rows, custom charts, history switching, and sensor/control status. The simple screen demonstrates layered arcs and state-dependent visual feedback. This validates much of the stack as a system.

## 11. Risks and defects

### UI source and generated assets are absent from a fresh clone

Both `res/` and `src/bin/` are ignored and currently untracked. The generated GUI C++ references image symbols from `src/bin`, while regeneration needs PNGs from `res`. A fresh checkout therefore has neither side of the resource pipeline and cannot reproduce/link the current image-using UI.

This is the most immediate reproducibility problem in the display stack. Source assets should normally be tracked; generated image arrays can then either be tracked or deterministically built.

### The DSL has no automated tests

There are no focused tests for preprocessing, component expansion, attribute rendering, invalid input, resource resolution, or generated C++ snapshots. Several dormant defects are visible:

- `split_attr()` computes a comma-normalized value and then accidentally splits the original string, so documented-looking comma-separated inputs do not work;
- repeated spaces can create empty tokens;
- `transform-skew` maps a two-value result to `setTransformSkewX(...)` rather than `setTransformSkew(...)`;
- `border-post` renders its value as a color even though the wrapper expects a boolean;
- duplicate resource validation checks the wrong dictionary level;
- fonts and resize metadata are parsed but not implemented.

The current XML happens not to exercise these paths. Golden generation tests would catch them cheaply.

### Resource symbols duplicate identical images per GUI

Resource names are prefixed by screen, so the same `power.png`, `fan.png`, and other icons referenced by two GUIs become separate C arrays and separate symbols. Unless the linker can merge their data, identical pixels occupy flash more than once. A content-hash or shared-resource registry would retain collision safety without duplication.

### Wrapper surface and generator mapping can drift

`LvObject` is very broad, while the XML initializer map independently repeats the supported subset and its naming rules. LVGL upgrades can affect both. The manual `lv_draw_buf_t` field initialization in `LvglHandler` also relies on LVGL internals more directly than a public buffer-construction API would.

Pinning LVGL more tightly and adding compile/generation tests are more valuable here than adding more wrapper methods.

### Ownership language is ambiguous

`LvObject` is described as both non-owning and as owning a reference; its destructor is non-owning and `free()` is explicit. Rotated-label canvases also create a persistent `lv_draw_buf_t` whose ownership is not represented in a C++ member. The current process-lifetime screen model makes leaks mostly bounded, but ownership should be documented and made explicit before dynamic creation/deletion is introduced.

### All screens are permanently allocated

Every generated GUI is constructed at runtime creation. This makes screen changes cheap but increases LVGL heap usage and keeps rotated canvases and all inactive widget trees alive. It is a defensible tradeoff at the present scale, but heap high-water measurements should determine when lazy construction becomes worthwhile.

### Error and shutdown paths are incomplete

- `Display::stopTasks()` explicitly leaves in-flight SPI handling and final panel state as TODOs.
- `Display::handleChunkComplete(bool success)` ignores `success`, releases the buffer, and proceeds as if the transport succeeded.
- the send queue allocation is guarded with a debug assertion rather than the fatal assertion used for semaphores;
- task/buffer cleanup is absent because these objects are expected to live forever.

Normal operation can work while these paths remain weak, but fatal handling occurs in a system that may be actively driving DMA. They should be made deterministic.

### Memory assumptions need measurement

The three DMA buffers alone require 115.2 KiB of internal DMA-capable heap. LVGL uses the C allocator for objects, series data, canvases, and layers. All screen trees are resident. The firmware has a heap diagnostics screen, but the repository has no recorded boot/free/minimum/largest-block budget.

The PlatformIO static RAM figure is therefore not a complete display memory budget. Record values after boot, after visiting every screen, and during worst-case chart rendering.

### Task safety is a convention

The render-ready handshake makes the existing path safe, but it is possible for any controller/helper to call the wrappers from the wrong task. Because `LV_OS_NONE` supplies no locks, this should become a documented invariant with debug enforcement.

### A few helper correctness issues remain

- `RowMenu::setSelectedRow(index)` never assigns `index` to `mActiveRowIndex`.
- chart catch-up inherits the `TimeSeries::iterateSince()` overrun problem described in the main review and can replay retained ring data repeatedly when far behind.
- the persistent rotated canvas/draw-buffer lifetime depends on LVGL ownership behavior and lacks a focused lifecycle test.
- manually initialized draw-buffer internals increase upgrade risk across LVGL versions.

## 12. Recommended stabilization plan

The stack does not need replacement. It needs to be recognized as a project of its own and given the same engineering treatment as the control engine.

### First: make it reproducible

1. Track all source icons under `res/`.
2. Add a generator dependency lock or requirements file and document the command.
3. Decide whether `src/bin` is checked in or always generated before PlatformIO builds.
4. Add an explicit build target that runs GUI generation before firmware compilation.
5. Pin the LVGL version precisely enough that wrapper/internal-structure changes are intentional.

### Second: test the compile-time stack

Add host-side tests or golden snapshots for:

- shorthand preprocessing;
- required, optional, and missing component parameters;
- nested object naming and parent construction;
- every supported attribute/value syntax;
- malformed colors, dimensions, angles, and multi-values;
- missing and duplicate resources;
- deterministic generated headers/sources;
- image conversion of the formats actually used by Garbox.

Fix the dormant initializer/resource defects as those tests are introduced.

### Third: test runtime invariants

Add focused tests or hardware instrumentation for:

- dirty functions running once and coalescing repeated marks;
- full dirtying on screen activation;
- no LVGL access while `lv_timer_handler()` is active;
- buffer tokens never underflowing or leaking;
- queue depth and flush ordering;
- failed SPI chunks propagating into a defined display fault;
- clean task shutdown with a transfer in flight;
- heap minimum/largest block after visiting every screen repeatedly.

### Fourth: finish the user-facing gaps

- connect or remove inactive menu rows;
- implement the simple-screen fan status;
- restore display command/brightness handling;
- correct time-axis labels;
- reconcile target ranges;
- finish the debug active-screen field;
- capture reference screenshots for each screen and important system state.

### Finally: resist unnecessary generalization

The stack already covers the current product well. The next valuable work is confidence, not breadth: tests, resource/build reproducibility, explicit ownership/concurrency contracts, memory measurements, and completed screen behavior. New DSL attributes or widgets should be added only when an actual Garbox screen needs them.

## Bottom line

The LVGL work is arguably a second major project inside Garbox. It is a custom UI compiler plus a static C++ view model plus a state-aware render scheduler plus a real-time display transport. Calling it “LVGL wrappers” misses its architecture and its value.

It is also one of the clearest examples of the project’s productive overengineering: the stack enables polished, data-rich embedded screens with little repetitive application code. Its main weakness is the same as the rest of the framework—its sophistication is not yet matched by tests, dependency packaging, and explicit invariants.
