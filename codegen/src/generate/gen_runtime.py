from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_runtime(ctx: Context, loader: Loader):
    _generate_runtime(ctx, loader)
    _generate_snapshot_registry(ctx, loader)


def _collect_event_routes(loader: Loader):
    event_routes = {}
    for event_key, event_data, in loader.config["event_types"].items():
        event_routes[event_key] = {
            "controllers": SortedSet(),
            "behaviours":  SortedSet(),
            "screens":  SortedSet(),
            "screens_with_fields": SortedSet(),
            "event": event_data,
            "count": 0
        }
    for section in ("controllers", "behaviours", "screens"):
        for key, data in loader.config[section].items():
            for event in data.get("receive_events", []):
                event_routes[event][section].add(key)
                event_routes[event]["count"] += 1

    for screen_key, screen_data in loader.config["screens"].items():
        for event in screen_data["model"]["events"].keys():
            event_routes[event]["screens_with_fields"].add(screen_key)
            event_routes[event]["count"] += 1

    return event_routes

def _collect_paths(loader: Loader, type: str):
    paths = []
    for key, data in loader.config[type].items():
        path = Path(f"app/{type}")
        if("subdir" in data): 
            path = path / data["subdir"]
        paths.append(path / key)
    return sorted(paths)


def _generate_runtime(ctx: Context, loader: Loader):
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
        "components":       loader.config["components"],
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


def _generate_snapshot_registry(ctx: Context, loader: Loader):
    """
    Generates 
    - app/runtime/SnapshotRegistry.h
    - app/runtime/SnapshotRegistry.cpp
    """

    # filter only events with kind='snapshot'
    event_types = loader.config["event_types"]
    events = {name: ev for name, ev in event_types.items() if ev["kind"] == "snapshot"}

    registry_dict = {
        "events": events,
    }
        
    items = [
        Item(registry_dict, "*", ctx.app_dir/"runtime/SnapshotRegistry.h",   "runtime/SnapshotRegistry.h.j2"),
        Item(registry_dict, "*", ctx.app_dir/"runtime/SnapshotRegistry.cpp", "runtime/SnapshotRegistry.cpp.j2"),
    ]

    generate_items(ctx, items)