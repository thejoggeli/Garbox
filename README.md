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
