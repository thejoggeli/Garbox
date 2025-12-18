from common.util import nested_get_dot
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_shared(ctx: Context, loader: Loader):
    _generate_events(ctx, loader)
    _generate_states(ctx, loader)
    _generate_component_ids(ctx, loader)
    _generate_profiler_ids(ctx, loader)
    _generate_tick_phases(ctx, loader)


def _generate_events(ctx: Context, loader: Loader):
    """
    Generates: 
    - shared/types/EventAlias.h
    - shared/types/EventType.h
    - shared/types/EventType.cpp
    - shared/types/EventPayload.h
    """

    payloads_dict = {
        "event_payloads": loader.config["event_payloads"],
        "event_types": loader.config["event_types"],
    }

    items = [
        Item(loader.config, "event_types", ctx.shared_dir / "types/EventAlias.h",   "shared/EventAlias.h.j2"),
        Item(loader.config, "event_types", ctx.shared_dir / "types/EventType.h",    "shared/EventType.h.j2"),
        Item(loader.config, "event_types", ctx.shared_dir / "types/EventType.cpp",  "shared/EventType.cpp.j2"),
        Item(payloads_dict, "*",           ctx.shared_dir / "types/EventPayload.h", "shared/EventPayload.h.j2"),
    ]

    generate_items(ctx, items)


def _generate_states(ctx: Context, loader: Loader):
    """
    Generates: 
    - shared/types/StateType.h
    - shared/types/StateType.cpp
    """

    items = [
        Item(loader.config, "states", ctx.shared_dir / "types/StateType.h",   "shared/StateType.h.j2"),
        Item(loader.config, "states", ctx.shared_dir / "types/StateType.cpp", "shared/StateType.cpp.j2"),
    ]

    generate_items(ctx, items)


def _generate_component_ids(ctx: Context, loader: Loader):
    """
    Generate ComponentId, BehaviourId, ControllerId files.
    """

    beha_dict   = {"names": list(loader.config["behaviours"].keys())}
    ctrl_dict   = {"names": list(loader.config["controllers"].keys())}
    screen_dict = {"names": list(loader.config["screens"].keys())}
    comp_dict   = {"names": list(loader.config["controllers"].keys()) + list(loader.config["behaviours"].keys()) + list(loader.config["screens"].keys())}
    
    items = [
        Item(beha_dict,   "names", ctx.shared_dir / "types/BehaviourId.h",    "shared/BehaviourId.h.j2"),
        Item(beha_dict,   "names", ctx.shared_dir / "types/BehaviourId.cpp",  "shared/BehaviourId.cpp.j2"),
        Item(ctrl_dict,   "names", ctx.shared_dir / "types/ControllerId.h",   "shared/ControllerId.h.j2"),
        Item(ctrl_dict,   "names", ctx.shared_dir / "types/ControllerId.cpp", "shared/ControllerId.cpp.j2"),
        Item(screen_dict, "names", ctx.shared_dir / "types/ScreenId.h",       "shared/ScreenId.h.j2"),
        Item(screen_dict, "names", ctx.shared_dir / "types/ScreenId.cpp",     "shared/ScreenId.cpp.j2"),
        Item(comp_dict,   "names", ctx.shared_dir / "types/ComponentId.h",    "shared/ComponentId.h.j2"),
        Item(comp_dict,   "names", ctx.shared_dir / "types/ComponentId.cpp",  "shared/ComponentId.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_profiler_ids(ctx: Context, loader: Loader):

    app_config = loader.config["application"]
    dev_config = loader.config["devtools"]

    config = {}
    config["system_ids"] = ["MainPeriod", "MainBusy"]
    config["ticks_ids"]  = [phase["name"] for phase in app_config["tick_phases"]]
    config["custom_ids"] = nested_get_dot(dev_config, "profiler.custom_ids", [])
    config["all_ids"]    = config["system_ids"] + config["ticks_ids"] + config["custom_ids"]

    items = [
        Item(config, "*", ctx.shared_dir/"types/ProfilerId.h",   "shared/ProfilerId.h.j2"),
        Item(config, "*", ctx.shared_dir/"types/ProfilerId.cpp", "shared/ProfilerId.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_tick_phases(ctx: Context, loader: Loader):

    config = loader.config["application"]

    items = [
        Item(config, "tick_phases", ctx.shared_dir/"types/TickPhase.h",   "shared/TickPhase.h.j2"),
        Item(config, "tick_phases", ctx.shared_dir/"types/TickPhase.cpp", "shared/TickPhase.cpp.j2"),
    ]
    generate_items(ctx, items)