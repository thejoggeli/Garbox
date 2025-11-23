from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context


def generate_runtime(ctx: Context, app_config: dict, events_config: dict):
    """
    Generate runtime .h/.cpp files for the given application.
    """

    behaviours  = app_config["behaviours"]
    controllers = app_config["controllers"]

    all_ticks = SortedSet()
    event_routes = {}

    # Behaviour ticks
    for item in behaviours.values():
        all_ticks.update(item["ticks"])

    # Controller ticks
    for item in controllers.values():
        all_ticks.update(item["ticks"])

    # Event routing table
    for event_key in events_config["types"].keys():
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
    app_name = app_config["name"]
    out_base = ctx.app_dir / f"runtime/{app_name}Runtime"
    template = "application/Runtime"

    # Aggregate runtime data
    runtime_dict = {
        "app_name":           app_name,
        "initial_behaviour":  app_config["initial_behaviour"],
        "behaviours":         behaviours,
        "controllers":        controllers,
        "all_ticks":          list(all_ticks),
        "event_routes":       event_routes,
    }

    items = [
        Item(runtime_dict, "*", out_base.with_suffix(".h"),   f"{template}.h.j2"),
        Item(runtime_dict, "*", out_base.with_suffix(".cpp"), f"{template}.cpp.j2"),
    ]

    generate_items(ctx, items)
