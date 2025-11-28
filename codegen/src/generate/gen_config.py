from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def generate_config(ctx: Context, loader: Loader):

    filename = f"{loader.config['application']['app_name']}Config"

    items = [
        Item(loader.config, "application", ctx.app_dir/f"config/{filename}.h",  "config/Config.h.j2"),
    ]
    generate_items(ctx, items)