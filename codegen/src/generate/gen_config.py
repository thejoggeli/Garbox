from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_config(ctx: Context, loader: Loader):

    filename = f"GeneratedConfig"

    items = [
        Item(loader.config, "application", ctx.gen_dir/f"config/{filename}.h",  "config/Config.h.j2"),
    ]
    generate_items(ctx, items)