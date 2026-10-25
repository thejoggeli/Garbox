# Garbox

Garbox is ESP32-S3 firmware for a temperature-controlled fermentation box. See
[the project overview](docs/project-overview.md) for the architecture and
[the desktop simulation plan](docs/framework/desktop-simulation-plan.md) for
the verification boundary.

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

# Model Field Specification (Quick Reference)

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
