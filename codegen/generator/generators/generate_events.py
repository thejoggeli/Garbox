import copy
from common.item import Item
from common.generate_items import generate_items
from common.generator_paths import GeneratorPaths


def generate_events(jinja_env, paths: GeneratorPaths, config):
    """
    Load event YAML, merge types into payload, and generate
    EventType.h/.cpp + EventPayload.h.
    """

    # Flatten event types into payload
    types_copy = copy.deepcopy(config["type"])
    types_copy.pop("include_h", None)
    types_copy.pop("include_cpp", None)

    if "payload" not in config:
        config["payload"] = types_copy
    else:
        config["payload"].update(types_copy)

    items = [
        Item(config, "type",    paths.shared_dir / "types/EventType.h",    "events/EventType.h.j2"),
        Item(config, "type",    paths.shared_dir / "types/EventType.cpp",  "events/EventType.cpp.j2"),
        Item(config, "payload", paths.shared_dir / "types/EventPayload.h", "events/EventPayload.h.j2"),
    ]

    generate_items(jinja_env, items)
