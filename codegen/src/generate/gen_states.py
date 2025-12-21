from common.util import print_json
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_states(ctx: Context, loader: Loader):
    _generate_states(ctx, loader)
    _generate_state_registry(ctx, loader)


def _generate_states(ctx: Context, loader: Loader):
    """
    Generates: 
    - generated/states/types/<name>State.h
    - generated/states/types/<name>State.cpp
    """

    items = []

    for state_name, state_data in loader.config["states"].items():

        filename = state_data["name_with_suffix"]

        items.append(Item(state_data, ctx.gen_dir/f"states/types/{filename}.h",   "states/State.h.j2"))
        items.append(Item(state_data, ctx.gen_dir/f"states/types/{filename}.cpp", "states/State.cpp.j2"))

    generate_items(ctx, items)


def _generate_state_registry(ctx: Context, loader: Loader):
    """
    Generates: 
    - generated/states/StateRegistry.h
    - generated/states/StateRegistry.cpp
    """

    template = "states/StateRegistry"
    data_dict = {
        "states": loader.config["states"]
    }

    items = []
    items.append(Item(data_dict, ctx.gen_dir/f"states/StateRegistry.h",   f"{template}.h.j2"))
    items.append(Item(data_dict, ctx.gen_dir/f"states/StateRegistry.cpp", f"{template}.cpp.j2"))

    generate_items(ctx, items)
