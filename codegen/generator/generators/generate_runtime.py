from sortedcontainers import SortedSet
from pathlib import Path
from common.item import Item
from common.generate_items import generate_items
from common.generator_paths import GeneratorPaths


def generate_runtime(jinja_env, paths: GeneratorPaths, application_config, events_config):
    """
    Generate runtime .h/.cpp files for the given application.
    """

    behaviours  = application_config["behaviours"]
    controllers = application_config["controllers"]

    ticks            = SortedSet()
    behaviour_ticks  = SortedSet()
    controller_ticks = SortedSet()
    event_routes     = {}

    # Behaviour ticks
    for item in behaviours.values():
        ticks.update(item["ticks"])
        behaviour_ticks.update(item["ticks"])

    # Controller ticks
    for item in controllers.values():
        ticks.update(item["ticks"])
        controller_ticks.update(item["ticks"])

    # Event routing table
    for event_key in events_config["type"].keys():
        event_routes[event_key] = {
            "controllers": SortedSet(),
            "behaviours":  SortedSet()
        }

    # Controller event targets
    for ctrl_key, ctrl_dict in controllers.items():
        r = ctrl_dict.get("receives")
        if not r:
            continue
        for event in r:
            event_routes[event]["controllers"].add(ctrl_key)

    # Behaviour event targets
    for beh_key, beh_dict in behaviours.items():
        r = beh_dict.get("receives")
        if not r:
            continue
        for event in r:
            event_routes[event]["behaviours"].add(beh_key)

    # Convert sets → lists
    for route in event_routes.values():
        route["controllers"] = list(route["controllers"])
        route["behaviours"]  = list(route["behaviours"])

    # Output paths
    name        = application_config["name"]
    out_base    = paths.app_dir / f"runtime/{name}Runtime"
    template    = "application/Runtime"

    # Aggregate runtime data
    runtime_dict = {
        "name":               name,
        "initial_behaviour":  application_config["initial_behaviour"],
        "behaviours":         behaviours,
        "controllers":        controllers,
        "ticks":              list(ticks),
        "behaviour_ticks":    list(behaviour_ticks),
        "controller_ticks":   list(controller_ticks),
        "event_routes":       event_routes,
    }

    keys = list(runtime_dict.keys())

    items = [
        Item(runtime_dict, keys, out_base.with_suffix(".h"),   f"{template}.h.j2"),
        Item(runtime_dict, keys, out_base.with_suffix(".cpp"), f"{template}.cpp.j2"),
    ]

    generate_items(jinja_env, items)
