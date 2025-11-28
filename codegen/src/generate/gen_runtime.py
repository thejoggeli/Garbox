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

    # Build event routing table
    event_routes = {}
    for event_key in loader.config["events"]["types"].keys():
        event_routes[event_key] = {
            "controllers": SortedSet(),
            "behaviours":  SortedSet()
        }

    # Controller event targets
    for ctrl_key, ctrl_dict in loader.config["controllers"].items():
        r = ctrl_dict.get("receives")
        if not r:
            continue
        for event in r:
            event_routes[event]["controllers"].add(ctrl_key)

    # Behaviour event targets
    for beh_key, beh_dict in loader.config["behaviours"].items():
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
    app_name = loader.config["application"]["app_name"]
    out_base = ctx.app_dir / f"runtime/{app_name}Runtime"

    # behaviours paths
    behaviour_paths = []
    for name, behaviour in loader.config["behaviours"].items():
        path = Path("app/behaviours")
        if("subdir" in behaviour): 
            path = path / behaviour["subdir"]
        behaviour_paths.append(path / name)
        
    # controllers paths
    controller_paths = []
    for name, controller in loader.config["controllers"].items():
        path = Path("app/controllers")
        if("subdir" in controller): 
            path = path / controller["subdir"]
        controller_paths.append(path / name)

    # Aggregate runtime data
    runtime_dict = {
        "application":      loader.config["application"],
        "behaviours":       loader.config["behaviours"],
        "controllers":      loader.config["controllers"],
        "devtools":         loader.config["devtools"],
        "event_routes":     event_routes,
        "behaviour_paths":  behaviour_paths,
        "controller_paths": controller_paths,
    }

    items = [
        Item(runtime_dict, "*", out_base.with_suffix(".h"),   "runtime/Runtime.h.j2"),
        Item(runtime_dict, "*", out_base.with_suffix(".cpp"), "runtime/Runtime.cpp.j2"),
    ]

    generate_items(ctx, items)
