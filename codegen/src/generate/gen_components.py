from common.item import Item, generate_items
from common.context import Context

def generate_components(ctx: Context, app_config: dict):
    """
    Generate everything together. This is what should be called by the user 
    """
    _generate_components_ids(ctx, app_config)
    _generate_components(ctx, app_config["controllers"], "controller", "Controller")
    _generate_components(ctx, app_config["behaviours"], "behaviour", "Behaviour")


def _generate_components_ids(ctx: Context, app_config: dict):
    """
    Generate ComponentId, BehaviourId, ControllerId files.
    """

    beha_dict = {"names": list(app_config["behaviours"].keys())}
    ctrl_dict = {"names": list(app_config["controllers"].keys())}
    comp_dict = {"names": list(app_config["controllers"].keys()) + list(app_config["behaviours"].keys())}
    items = [
        Item(beha_dict, "names", ctx.shared_dir / "types/BehaviourId.h",    "application/BehaviourId.h.j2"),
        Item(beha_dict, "names", ctx.shared_dir / "types/BehaviourId.cpp",  "application/BehaviourId.cpp.j2"),
        Item(ctrl_dict, "names", ctx.shared_dir / "types/ControllerId.h",   "application/ControllerId.h.j2"),
        Item(ctrl_dict, "names", ctx.shared_dir / "types/ControllerId.cpp", "application/ControllerId.cpp.j2"),
        Item(comp_dict, "names", ctx.shared_dir / "types/ComponentId.h",    "application/ComponentId.h.j2"),
        Item(comp_dict, "names", ctx.shared_dir / "types/ComponentId.cpp",  "application/ComponentId.cpp.j2"),
    ]
    generate_items(ctx, items)


def _generate_components(ctx: Context, config: dict, type_key: str, type_name: str):
    """
    Generates either behaviours or controllers, depending on passed parameters
    """
    
    # list of items to be generated
    items = []
    stubs = []
    for comp_key, comp_dict in config.items():
        out_path = ctx.app_dir /  f"{type_key}s/generated/{comp_key}Abs"
        template_path = f"application/{type_key}/{type_name}Abs"
        item_dict = {
            "name": comp_key,
            f"{type_key}": comp_dict
        }
        comp_dict["name"] = comp_key
        items.append(Item(item_dict, "*", f"{out_path}.h", f"{template_path}.h.j2"))
        items.append(Item(item_dict, "*", f"{out_path}.cpp", f"{template_path}.cpp.j2"))

        out_path = ctx.stubs_dir / f"{type_key}s/{comp_key}"
        template_path = f"application/{type_key}/{type_name}Stub"
        stubs.append(Item(item_dict, "*", f"{out_path}.h", f"{template_path}.h.j2"))
        stubs.append(Item(item_dict, "*", f"{out_path}.cpp", f"{template_path}.cpp.j2"))

    # generate all hardware h/cpp files
    generate_items(ctx, items)
    generate_items(ctx, stubs)