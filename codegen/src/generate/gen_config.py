from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def generate_config(ctx: Context, loader: Loader):

    app_config = loader.get_application_config()
    filename = f"{app_config['setup']['app_name']}Config"

    items = [
        Item(app_config, "setup", ctx.app_dir/f"config/{filename}.h",  "config/Config.h.j2"),
    ]
    generate_items(ctx, items)