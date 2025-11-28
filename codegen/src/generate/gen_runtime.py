from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def _collect_event_routes(loader: Loader):
    event_routes = {}
    for event_key in loader.config["events"]["types"].keys():
        event_routes[event_key] = {
            "controllers": SortedSet(),
            "behaviours":  SortedSet(),
            "screens":  SortedSet(),
        }
    for section in ("controllers", "behaviours", "screens"):
        for key, data in loader.config[section].items():
            for event in data.get("receives", []):
                event_routes[event][section].add(key)
    return event_routes

def _collect_paths(loader: Loader, type: str):
    paths = []
    for key, data in loader.config[type].items():
        path = Path(f"app/{type}")
        if("subdir" in data): 
            path = path / data["subdir"]
        paths.append(path / key)
    return paths

def generate_runtime(ctx: Context, loader: Loader):
    """
    Generates 
    - app/runtime/<Name>Runtime.h
    - app/runtime/<Name>Runtime.cpp
    """

    # Output paths
    app_name = loader.config["application"]["app_name"]
    out_base = ctx.app_dir / f"runtime/{app_name}Runtime"

    # Aggregate runtime data
    runtime_dict = {
        "devtools":         loader.config["devtools"],
        "event_routes":     _collect_event_routes(loader),
        "application":      loader.config["application"],
        "behaviours":       loader.config["behaviours"],
        "controllers":      loader.config["controllers"],
        "screens":          loader.config["screens"],
        "behaviour_paths":  _collect_paths(loader, 'behaviours'),
        "controller_paths": _collect_paths(loader, 'controllers'),
        "screen_paths":     _collect_paths(loader, 'screens'),
    }

    items = [
        Item(runtime_dict, "*", out_base.with_suffix(".h"),   "runtime/Runtime.h.j2"),
        Item(runtime_dict, "*", out_base.with_suffix(".cpp"), "runtime/Runtime.cpp.j2"),
    ]

    generate_items(ctx, items)
