import re
from pathlib import Path

from common.str_filters import ensure_suffix
from common.util import ensure_list


_CPP_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


def _require_mapping(config, key):
    value = config.get(key)
    if not isinstance(value, dict):
        raise ValueError(f"configuration section '{key}' must be a mapping")
    return value


def _validate_identifier(value, where):
    if not isinstance(value, str) or not _CPP_IDENTIFIER.fullmatch(value):
        raise ValueError(f"{where} must be a valid C++ identifier, got {value!r}")


def _validate_fields(owner_kind, owner_name, data):
    if data is None:
        return
    if not isinstance(data, dict):
        raise ValueError(f"{owner_kind} '{owner_name}' must be a mapping")
    fields = data.get("fields") or []
    if not isinstance(fields, list):
        raise ValueError(f"{owner_kind} '{owner_name}'.fields must be a list")
    names = set()
    for index, field in enumerate(fields):
        if not isinstance(field, dict):
            raise ValueError(f"{owner_kind} '{owner_name}'.fields[{index}] must be a mapping")
        if "name" not in field or "type" not in field:
            raise ValueError(
                f"{owner_kind} '{owner_name}'.fields[{index}] requires name and type"
            )
        field_name = field["name"]
        _validate_identifier(field_name, f"{owner_kind} '{owner_name}' field name")
        if field_name in names:
            raise ValueError(f"duplicate field '{field_name}' in {owner_kind} '{owner_name}'")
        names.add(field_name)
        if not isinstance(field["type"], str) or not field["type"].strip():
            raise ValueError(f"{owner_kind} '{owner_name}' field '{field_name}' has no type")


def validate_raw_config(config):
    """Validate references and shapes before parsers mutate the source model."""
    events = _require_mapping(config, "events")
    states = _require_mapping(config, "states")
    controllers = _require_mapping(config, "controllers")
    behaviours = _require_mapping(config, "behaviours")
    screens = _require_mapping(config, "screens")
    application = _require_mapping(config, "application")

    for name, data in events.items():
        _validate_identifier(name, "event name")
        _validate_fields("event", name, data)
    for name, data in states.items():
        _validate_identifier(name, "state name")
        _validate_fields("state", name, data)

    component_sections = (
        ("controller", "Controller", controllers),
        ("behaviour", "Behaviour", behaviours),
        ("screen", "Screen", screens),
    )
    normalized_components = {}
    for kind, suffix, section in component_sections:
        for raw_name, data in section.items():
            _validate_identifier(raw_name, f"{kind} name")
            if not isinstance(data, dict):
                raise ValueError(f"{kind} '{raw_name}' must be a mapping")
            name = ensure_suffix(raw_name, suffix)
            if name in normalized_components:
                raise ValueError(
                    f"component name collision '{name}' between "
                    f"{normalized_components[name]} and {kind}"
                )
            normalized_components[name] = kind

    phases = application.get("tick_phases")
    if not isinstance(phases, list) or not phases:
        raise ValueError("application.tick_phases must be a non-empty list")
    phase_names = set()
    phase_orders = set()
    for index, phase in enumerate(phases):
        if not isinstance(phase, dict) or "name" not in phase:
            raise ValueError(f"application.tick_phases[{index}] requires a name")
        phase_name = ensure_suffix(phase["name"], "Tick")
        _validate_identifier(phase_name, "tick phase name")
        if phase_name in phase_names:
            raise ValueError(f"duplicate tick phase '{phase_name}'")
        phase_names.add(phase_name)
        if "order" in phase:
            order = phase["order"]
            if not isinstance(order, int) or isinstance(order, bool) or order < 0:
                raise ValueError(f"tick phase '{phase_name}' order must be a non-negative integer")
            if order in phase_orders:
                raise ValueError(f"duplicate tick phase order {order}")
            phase_orders.add(order)

    for numeric_name in (
        "event_pool_size_bytes",
        "event_queue_length",
        "tick_period_millis",
        "max_dispatch_recursion_depth",
    ):
        value = application.get(numeric_name)
        if not isinstance(value, int) or isinstance(value, bool) or value <= 0:
            raise ValueError(f"application.{numeric_name} must be a positive integer")

    initial_behaviour = application.get("initial_behaviour")
    if initial_behaviour not in {ensure_suffix(name, "Behaviour") for name in behaviours}:
        raise ValueError(f"unknown initial behaviour '{initial_behaviour}'")
    initial_screen = application.get("initial_screen")
    if initial_screen not in {ensure_suffix(name, "Screen") for name in screens}:
        raise ValueError(f"unknown initial screen '{initial_screen}'")

    for kind, _, section in component_sections:
        for name, component in section.items():
            for key in ("receive_events", "send_events"):
                references = ensure_list(component.get(key, []))
                for event_name in references:
                    if event_name == "INSERT_ALL":
                        continue
                    if event_name not in events:
                        raise ValueError(f"{kind} '{name}' references unknown event '{event_name}' in {key}")
            for key in ("read_states", "write_states"):
                references = ensure_list(component.get(key, []))
                for state_name in references:
                    if state_name == "INSERT_ALL":
                        continue
                    if state_name not in states:
                        raise ValueError(f"{kind} '{name}' references unknown state '{state_name}' in {key}")
            for phase_name in ensure_list(component.get("tick_phases", [])):
                if phase_name == "INSERT_ALL":
                    continue
                normalized = ensure_suffix(phase_name, "Tick")
                if normalized not in phase_names:
                    raise ValueError(f"{kind} '{name}' references unknown tick phase '{phase_name}'")

    hardware = _require_mapping(config, "hardware")
    ledc = hardware.get("ledc", {}).get("instances", {})
    timer_names = {
        ensure_suffix(name, "Timer") for name in ledc.get("timers", {})
    }
    for channel_name, channel in ledc.get("channels", {}).items():
        timer_name = ensure_suffix(channel.get("timer", ""), "Timer")
        if timer_name not in timer_names:
            raise ValueError(
                f"LEDC channel '{channel_name}' references unknown timer '{channel.get('timer')}'"
            )


def validate_gui_resources(guis, resource_root: Path):
    resource_root = resource_root.resolve()
    for gui_name, gui in guis.items():
        for resource in gui["resources"]["used"]["images"].values():
            relative_path = Path(resource["path"])
            source = (resource_root / relative_path).resolve()
            if not source.is_relative_to(resource_root):
                raise ValueError(
                    f"GUI '{gui_name}' image path escapes resource directory: {relative_path}"
                )
            if not source.is_file():
                raise ValueError(f"GUI '{gui_name}' image does not exist: {relative_path}")
