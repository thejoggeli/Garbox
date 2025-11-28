from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader
from common.util import nested_get_dot
from pathlib import Path

def generate_components(ctx: Context, loader: Loader):
    """
    Generate all component files.
    
    # For each Controller:
    - app/controller/abstract/<Name>ControllerAbs.h
    - app/controller/abstract/<Name>ControllerAbs.cpp
    - godecen/stubs/controller/<Name>Controller.h
    - godecen/stubs/controller/<Name>Controller.cpp

    # For each Behaviour:
    - app/behaviour/abstract/<Name>BehaviourAbs.h
    - app/behaviour/abstract/<Name>BehaviourAbs.cpp
    - godecen/stubs/behaviour/<Name>Behaviour.h
    - godecen/stubs/behaviour/<Name>Behaviour.cpp
    
    # Types:
    - shared/types/ComponentId.h
    - shared/types/ComponentId.cpp
    - shared/types/ControllerId.h
    - shared/types/ControllerId.cpp
    - shared/types/BehaviourId.h
    - shared/types/BehaviourId.cpp

    """

    _generate_components(ctx, loader, "controller", "Controller")
    _generate_components(ctx, loader, "behaviour", "Behaviour")


def _generate_components(ctx: Context, loader: Loader, type_key: str, type_name: str):
    """
    Generates either behaviours or controllers, depending on passed parameters
    """

    config = loader.config
    components = config[f"{type_key}s"]

    stubs_prefix = nested_get_dot(config, "application.stubs_prefix", "Example")
    stubs_suffix = nested_get_dot(config, "application.stubs_suffix", "")
    
    # list of items to be generated
    items = []
    stubs = []
    for comp_key, comp_dict in components.items():
        out_path = ctx.app_dir /  f"{type_key}s/abstract/{comp_key}Abs"
        template_path = f"components/{type_key}/{type_name}Abs"
        item_dict = {
            "name": comp_key,
            f"{type_key}": comp_dict
        }
        comp_dict["name"] = comp_key
        # generate abstract files, e.g. app/controller/abstract/FanControllerAbs.h
        items.append(Item(item_dict, "*", f"{out_path}.h", f"{template_path}.h.j2"))
        items.append(Item(item_dict, "*", f"{out_path}.cpp", f"{template_path}.cpp.j2"))

        # generate stub files, e.g. codegen/stubs/controller/ExampleFanController.hy
        path_parts = [ctx.stubs_dir, f"{type_key}s/", comp_dict.get("subdir", ""), f"{stubs_prefix}{comp_key}{stubs_suffix}"]
        out_path = Path(*path_parts)
        template_path = f"components/{type_key}/{type_name}Stub"
        stubs.append(Item(item_dict, "*", f"{out_path}.h", f"{template_path}.h.j2"))
        stubs.append(Item(item_dict, "*", f"{out_path}.cpp", f"{template_path}.cpp.j2"))

    # generate all hardware h/cpp files
    generate_items(ctx, items)
    generate_items(ctx, stubs)