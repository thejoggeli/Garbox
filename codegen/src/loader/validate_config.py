import re
from pathlib import Path

from common.str_filters import ensure_suffix
from common.util import ensure_list


_CPP_IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_CPP_KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class",
    "compl", "concept", "const", "constexpr", "const_cast", "continue", "co_await",
    "co_return", "co_yield", "decltype", "default", "delete", "do", "double",
    "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false",
    "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable",
    "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or",
    "or_eq", "private", "protected", "public", "register", "reinterpret_cast",
    "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
    "static_cast", "struct", "switch", "template", "this", "thread_local", "throw",
    "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
}


def _require_mapping(config, key):
    value = config.get(key)
    if not isinstance(value, dict):
        raise ValueError(f"configuration section '{key}' must be a mapping")
    return value


def _validate_identifier(value, where):
    if not isinstance(value, str) or not _CPP_IDENTIFIER.fullmatch(value):
        raise ValueError(f"{where} must be a valid C++ identifier, got {value!r}")
    if value in _CPP_KEYWORDS:
        raise ValueError(f"{where} must not be a reserved C++ keyword, got {value!r}")


def _validate_include_list(value, where):
    for index, include in enumerate(ensure_list(value)):
        if (
            not isinstance(include, str)
            or not include
            or any(character in include for character in ('\n', '\r', '"'))
        ):
            raise ValueError(f"{where}[{index}] must be a safe non-empty include path")


def _validate_reference_list(value, where, known, sentinel=None):
    references = ensure_list(value)
    seen = set()
    for index, reference in enumerate(references):
        if not isinstance(reference, str):
            raise ValueError(f"{where}[{index}] must be a string")
        if reference in seen:
            raise ValueError(f"duplicate reference '{reference}' in {where}")
        seen.add(reference)
        if sentinel is not None and reference == sentinel:
            if len(references) != 1:
                raise ValueError(f"{where} sentinel '{sentinel}' must be the only entry")
            continue
        if reference not in known:
            raise ValueError(f"{where} references unknown value '{reference}'")
    return references


def _validate_safe_relative_path(value, where):
    if not isinstance(value, str) or not value.strip():
        raise ValueError(f"{where} must be a non-empty relative path")
    if "\\" in value or ":" in value:
        raise ValueError(f"{where} must stay inside its generated directory")
    path = Path(value)
    if path.is_absolute() or ".." in path.parts:
        raise ValueError(f"{where} must stay inside its generated directory")
    if value != path.as_posix() or any(part in ("", ".") for part in path.parts):
        raise ValueError(f"{where} contains an invalid path component")


def _validate_normalized_names(section, suffix, kind):
    normalized = {}
    for name in section:
        _validate_identifier(name, f"{kind} name")
        output_name = ensure_suffix(name, suffix)
        if output_name in normalized:
            raise ValueError(
                f"{kind} name collision '{output_name}' after suffix normalization: "
                f"'{normalized[output_name]}' and '{name}'"
            )
        normalized[output_name] = name


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
        field_type = field["type"]
        if not isinstance(field_type, str) or not field_type.strip():
            raise ValueError(f"{owner_kind} '{owner_name}' field '{field_name}' has no type")
        if any(character in field_type for character in ("\n", "\r", ";", "{", "}", "#")):
            raise ValueError(
                f"{owner_kind} '{owner_name}' field '{field_name}' has an unsafe type"
            )

    _validate_include_list(data.get("include", []), f"{owner_kind} '{owner_name}'.include")


def validate_raw_config(config):
    """Validate references and shapes before parsers mutate the source model."""
    events = _require_mapping(config, "events")
    states = _require_mapping(config, "states")
    controllers = _require_mapping(config, "controllers")
    behaviours = _require_mapping(config, "behaviours")
    screens = _require_mapping(config, "screens")
    application = _require_mapping(config, "application")

    _validate_normalized_names(events, "Event", "event")
    _validate_normalized_names(states, "State", "state")

    for name, data in events.items():
        _validate_fields("event", name, data)
    for name, data in states.items():
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
            if "subdir" in data:
                _validate_safe_relative_path(data["subdir"], f"{kind} '{raw_name}'.subdir")
            for flag in ("receive_events_all", "read_states_all", "tick_phases_all"):
                if flag in data and not isinstance(data[flag], bool):
                    raise ValueError(f"{kind} '{raw_name}'.{flag} must be boolean")

            if kind == "screen" and "renderer" in data:
                renderer = data["renderer"]
                if not isinstance(renderer, str) or renderer.strip().lower() not in {
                    "lvgl-simple", "lvgl-gui"
                }:
                    raise ValueError(f"screen '{raw_name}' has unsupported renderer {renderer!r}")
                if renderer.strip().lower() == "lvgl-gui":
                    renderer_gui = data.get("renderer_gui")
                    if (
                        not isinstance(renderer_gui, str)
                        or Path(renderer_gui).name != renderer_gui
                        or not renderer_gui.endswith(".xml")
                    ):
                        raise ValueError(
                            f"screen '{raw_name}'.renderer_gui must be an XML filename"
                        )

    app_name = application.get("app_name")
    if (
        not isinstance(app_name, str)
        or not app_name
        or any(character in app_name for character in ('\n', '\r', '"', '\\'))
    ):
        raise ValueError("application.app_name must be a safe non-empty string")

    phases = application.get("tick_phases")
    if not isinstance(phases, list) or not phases:
        raise ValueError("application.tick_phases must be a non-empty list")
    phase_names = set()
    phase_orders = set()
    for index, phase in enumerate(phases):
        if not isinstance(phase, dict) or "name" not in phase:
            raise ValueError(f"application.tick_phases[{index}] requires a name")
        raw_phase_name = phase["name"]
        _validate_identifier(raw_phase_name, "tick phase name")
        phase_name = ensure_suffix(raw_phase_name, "Tick")
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
        if "delay_millis" in phase:
            delay = phase["delay_millis"]
            if not isinstance(delay, int) or isinstance(delay, bool) or delay < 0:
                raise ValueError(
                    f"tick phase '{phase_name}' delay_millis must be a non-negative integer"
                )

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
                _validate_reference_list(
                    component.get(key, []),
                    f"{kind} '{name}'.{key}",
                    events,
                    "INSERT_ALL" if key == "receive_events" else None,
                )
            for key in ("read_states", "write_states"):
                _validate_reference_list(
                    component.get(key, []),
                    f"{kind} '{name}'.{key}",
                    states,
                    "INSERT_ALL" if key == "read_states" else None,
                )
            phase_references = _validate_reference_list(
                component.get("tick_phases", []),
                f"{kind} '{name}'.tick_phases",
                {
                    phase_name
                    for phase in phases
                    for phase_name in (
                        phase["name"],
                        ensure_suffix(phase["name"], "Tick"),
                    )
                },
                "INSERT_ALL",
            )
            for phase_name in phase_references:
                if phase_name != "INSERT_ALL":
                    normalized = ensure_suffix(phase_name, "Tick")
                    if normalized not in phase_names:
                        raise ValueError(
                            f"{kind} '{name}'.tick_phases references unknown value '{phase_name}'"
                        )

    hardware = _require_mapping(config, "hardware")
    for hardware_kind, hardware_config in hardware.items():
        if not isinstance(hardware_config, dict):
            raise ValueError(f"hardware.{hardware_kind} must be a mapping")
        instances = hardware_config.get("instances", {})
        if not isinstance(instances, dict):
            raise ValueError(f"hardware.{hardware_kind}.instances must be a mapping")
        if hardware_kind == "ledc":
            for group in ("timers", "channels"):
                group_data = instances.get(group, {})
                if not isinstance(group_data, dict):
                    raise ValueError(f"hardware.ledc.instances.{group} must be a mapping")
                _validate_normalized_names(
                    group_data,
                    "Timer" if group == "timers" else "Channel",
                    f"LEDC {group[:-1]}",
                )
                for instance_name, instance in group_data.items():
                    if not isinstance(instance, dict):
                        raise ValueError(
                            f"hardware.ledc.instances.{group}.{instance_name} must be a mapping"
                        )
        else:
            _validate_normalized_names(instances, "", f"{hardware_kind} instance")
            for instance_name, instance in instances.items():
                if not isinstance(instance, dict):
                    raise ValueError(
                        f"hardware.{hardware_kind}.instances.{instance_name} must be a mapping"
                    )
        _validate_include_list(
            hardware_config.get("include_h", []), f"hardware.{hardware_kind}.include_h"
        )
        _validate_include_list(
            hardware_config.get("include_cpp", []), f"hardware.{hardware_kind}.include_cpp"
        )

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
