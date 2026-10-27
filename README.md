# Garbox

Garbox is ESP32-S3 firmware for a temperature-controlled fermentation box. It
includes a typed application framework/code generator, a custom LVGL UI stack,
host-side control and thermal simulation, and the actual appliance firmware.

Start with the [project overview](docs/project-overview.md),
[engineering review](docs/review.md), and [custom LVGL stack](docs/lvgl-stack.md).

## Setup

PlatformIO is required for C++ builds. The generator uses a small pinned Python
environment:

```bash
python3 -m venv .venv
.venv/bin/python -m pip install -r codegen/requirements.txt
```

Source PNGs in `res/`, generated image arrays in `src/bin/`, and generated C++
are tracked deliberately. A clean checkout therefore builds without first
running the generator, while `--check` proves that all generated artifacts still
match their declarative inputs.

## Code generation

The command is independent of the current working directory:

```bash
.venv/bin/python codegen/src/main.py
.venv/bin/python codegen/src/main.py --check --quiet
```

Generation is transactional: configuration, handwritten section markers,
templates, and image conversion are completed in a staging tree before any
project output is replaced. `--check` never writes project files and exits
nonzero with a diff when output is stale.

Configuration lives in `codegen/config`, GUI XML in `codegen/config/gui`, and
templates in `codegen/templates`. Do not hand-edit `src/generated` or `src/bin`.

## Build and verification

Run the complete software gate with:

```bash
.venv/bin/python tools/verify_project.py
```

Use `--skip-target` when the ESP32 toolchain is unavailable and `--skip-ui` for
a faster non-visual pass. Individual commands are:

```bash
~/.platformio/penv/bin/pio test -e native
~/.platformio/penv/bin/pio run -e ui-simulator
.venv/bin/python tools/verify_ui_flow.py
~/.platformio/penv/bin/pio run -e esp32-s3-devkitc-1
```

## Interactive desktop UI

Run the real generated LVGL UI, application runtime, fermentation behaviour,
and thermal plant in a local browser:

```bash
python3 tools/ui_simulator.py
```

The launcher finds PlatformIO in the shell or in its standard user install,
builds `ui-simulator`, and opens `http://127.0.0.1:8765/`. Rotate the simulated
encoder with the on-screen arrows, mouse wheel, or left/right keys. Click the
encoder for a short press, or hold it for 0.7 seconds for a long press. Use
`--no-build` to reuse the current binary and `--no-browser` to suppress opening
the browser.

## Model field specification (legacy quick reference)

Each entry in `model.fields` describes one model value. Valid keys:

- **name** (required)  
  Unique field name stored in the model.

- **type** (required if no `bind`)  
  C++ type for manual fields.

- **bind** (required if no `type`)  
  Format: `EventType->field`.  
  Automatically infers `type`, optional `default`, and marks the field as event-driven.

- **group** (optional)  
  Logical update group. Defaults to `UpperFirst(name)`.

- **dirty** (optional)  
  Additional groups to mark dirty when this field changes.

- **epsilon** (optional, float only)  
  Minimum delta required to trigger a dirty update.

- **default** (optional)  
  C++ literal default. For bound fields, inherited from event definition if missing.

- **debounce** (optional)  
  Debounce time in microseconds (`uint32_t`).
