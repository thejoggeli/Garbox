from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader

def generate_runtime(ctx: Context, loader: Loader):
    """
    Generates 
    - app/runtime/<Name>Runtime.h
    - app/runtime/<Name>Runtime.cpp
    """

    app_config = loader.get_application_config()
    devtools_config = loader.get_devtools_config()
    events_config = loader.get_events_config()

    # Build event routing table
    event_routes = {}
    for event_key in events_config["types"].keys():
        event_routes[event_key] = {
            "controllers": SortedSet(),
            "behaviours":  SortedSet()
        }

    # Controller event targets
    for ctrl_key, ctrl_dict in app_config["controllers"].items():
        r = ctrl_dict.get("receives")
        if not r:
            continue
        for event in r:
            event_routes[event]["controllers"].add(ctrl_key)

    # Behaviour event targets
    for beh_key, beh_dict in app_config["behaviours"].items():
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
    app_name = app_config["setup"]["name"]
    out_base = ctx.app_dir / f"runtime/{app_name}Runtime"
    template = "application/Runtime"

    # behaviours paths
    behaviour_paths = []
    for name, behaviour in app_config["behaviours"].items():
        path = Path("app/behaviours")
        if("subdir" in behaviour): 
            path = path / behaviour["subdir"]
        behaviour_paths.append(path / name)
        
    # controllers paths
    controller_paths = []
    for name, controller in app_config["controllers"].items():
        path = Path("app/controllers")
        if("subdir" in controller): 
            path = path / controller["subdir"]
        controller_paths.append(path / name)

    # Aggregate runtime data
    runtime_dict = {
        "setup":            app_config["setup"],
        "behaviours":       app_config["behaviours"],
        "controllers":      app_config["controllers"],
        "event_routes":     event_routes,
        "behaviour_paths":  behaviour_paths,
        "controller_paths": controller_paths,
        "devtools":         devtools_config,
    }

    items = [
        Item(runtime_dict, "*", out_base.with_suffix(".h"),   f"{template}.h.j2"),
        Item(runtime_dict, "*", out_base.with_suffix(".cpp"), f"{template}.cpp.j2"),
    ]

    generate_items(ctx, items)
