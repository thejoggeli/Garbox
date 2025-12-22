from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader
from common.util import nested_get_dot
from common.gen_section import GenSection
from pathlib import Path

def generate_components(ctx: Context, loader: Loader):
    """
    Generate all component files.

    # For each Behaviour:
    - generated/behaviour/Name>BehaviourAbs.h
    - generated/behaviour/Name>BehaviourAbs.cpp
    - godecen/output/stubs/behaviour/<Name>Behaviour.h
    - godecen/output/stubs/behaviour/<Name>Behaviour.cpp
    
    # For each Controller:
    - generated/controller/<Name>ControllerAbs.h
    - generated/controller/<Name>ControllerAbs.cpp
    - godecen/output/stubs/controller/<Name>Controller.h
    - godecen/output/stubs/controller/<Name>Controller.cpp
    
    # For each Screen:
    - generated/screen/<Name>ScreenAbs.h
    - generated/screen/<Name>ScreenAbs.cpp
    - godecen/output/stubs/screen/<Name>Screen.h
    - godecen/output/stubs/screen/<Name>Screen.cpp

    """

    # generate components
    _generate_components(ctx, loader, "behaviour", "Behaviour")
    _generate_components(ctx, loader, "controller", "Controller")
    _generate_components(ctx, loader, "screen", "Screen")

    # generate stubs
    _generate_stubs(ctx, loader, "behaviour", "Behaviour")
    _generate_stubs(ctx, loader, "controller", "Controller")
    _generate_stubs(ctx, loader, "screen", "Screen")


def _generate_components(ctx: Context, loader: Loader, type_key: str, type_name: str):
    """
    Generates either behaviours or controllers, depending on passed parameters
    """

    config = loader.config
    components = config[f"{type_key}s"]
    
    # list of items to be generated
    items = []
    for comp_key, comp_dict in components.items():

        out_path = ctx.gen_dir /  f"{type_key}s/{comp_key}Abs"
        template_path = f"components/{type_key}/{type_name}Abs"
        item_dict = {
            "type": type_key,
            "name": comp_key,
            "runtime_name": "Runtime",
            f"{type_key}": comp_dict
        }
        comp_dict["name"] = comp_key

        # generate abstract files, e.g. generated/controllers/FanControllerAbs.h
        items.append(Item(item_dict, f"{out_path}.h", f"{template_path}.h.j2"))
        items.append(Item(item_dict, f"{out_path}.cpp", f"{template_path}.cpp.j2"))

    # generate all h/cpp files
    generate_items(ctx, items)


def _generate_stubs(ctx: Context, loader: Loader, type_key: str, type_name: str):

    config = loader.config
    components = config[f"{type_key}s"]
    
    # list of items to be generated
    items = []
    for comp_key, comp_dict in components.items():

        item_dict = {
            "component": comp_dict
        }

        filename = comp_key
        path_parts = [ctx.app_dir, f"components/{type_key}s/", comp_dict.get("subdir", ""), filename]
        out_path = Path(*path_parts)
        template_path = f"components/stub"

        # sections
        sections = [
            GenSection(filename, "include", item_dict, f"{template_path}/section/include.h.j2"),
            GenSection(filename, "interface", item_dict, f"{template_path}/section/interface.h.j2"),
        ]

        # append items
        items.append(Item(
            config          = item_dict, 
            out_path        = f"{out_path}.h", 
            template_path   = f"{template_path}/Component.h.j2", 
            sections        = sections
        ))
        items.append(Item(
            config          = item_dict, 
            out_path        = f"{out_path}.cpp", 
            template_path   = f"{template_path}/Component.cpp.j2", 
            allow_override  = False
        ))

    # generate all h/cpp files
    generate_items(ctx, items)

    # 

