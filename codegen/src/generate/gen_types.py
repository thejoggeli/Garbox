from common.util import nested_get_dot
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def generate_types(ctx: Context, loader: Loader):
    _generate_events(ctx, loader)
    _generate_component_ids(ctx, loader)
    _generate_profiler_ids(ctx, loader)


def _generate_events(ctx: Context, loader: Loader):
    """
    Generates: 
    - shared/types/EventAlias.h
    - shared/types/EventType.h
    - shared/types/EventType.cpp
    - shared/types/EventPayload.h
    """

    events_config = loader.get_events_config()

    items = [
        Item(events_config, "*", ctx.shared_dir / "types/EventAlias.h",   "types/EventAlias.h.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventType.h",    "types/EventType.h.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventType.cpp",  "types/EventType.cpp.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventPayload.h", "types/EventPayload.h.j2", meta_key="payloads_meta"),
    ]

    generate_items(ctx, items)


def _generate_component_ids(ctx: Context, loader: Loader):
    """
    Generate ComponentId, BehaviourId, ControllerId files.
    """

    app_config = loader.get_application_config()

    beha_dict = {"names": list(app_config["behaviours"].keys())}
    ctrl_dict = {"names": list(app_config["controllers"].keys())}
    comp_dict = {"names": list(app_config["controllers"].keys()) + list(app_config["behaviours"].keys())}
    items = [
        Item(beha_dict, "names", ctx.shared_dir / "types/BehaviourId.h",    "types/BehaviourId.h.j2"),
        Item(beha_dict, "names", ctx.shared_dir / "types/BehaviourId.cpp",  "types/BehaviourId.cpp.j2"),
        Item(ctrl_dict, "names", ctx.shared_dir / "types/ControllerId.h",   "types/ControllerId.h.j2"),
        Item(ctrl_dict, "names", ctx.shared_dir / "types/ControllerId.cpp", "types/ControllerId.cpp.j2"),
        Item(comp_dict, "names", ctx.shared_dir / "types/ComponentId.h",    "types/ComponentId.h.j2"),
        Item(comp_dict, "names", ctx.shared_dir / "types/ComponentId.cpp",  "types/ComponentId.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_profiler_ids(ctx: Context, loader: Loader):

    app_config = loader.get_application_config()
    dev_config = loader.get_devtools_config()

    config = {}
    config["system_ids"] = ["MainPeriod", "MainBusy"]
    config["ticks_ids"]  = [phase["name"] for phase in app_config["setup"]["tick_phases"]]
    config["custom_ids"] = nested_get_dot(dev_config, "profiler.custom_ids", [])
    config["all_ids"]    = config["system_ids"] + config["ticks_ids"] + config["custom_ids"]

    items = [
        Item(config, "*", ctx.shared_dir/"types/ProfilerId.h",   "types/ProfilerId.h.j2"),
        Item(config, "*", ctx.shared_dir/"types/ProfilerId.cpp", "types/ProfilerId.cpp.j2"),
    ]
    generate_items(ctx, items)