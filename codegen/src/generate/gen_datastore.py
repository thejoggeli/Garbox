from pathlib import Path
from sortedcontainers import SortedSet
from common.item import Item, generate_items
from common.context import Context
from loader.loader import Loader


def generate_datastore(ctx: Context, loader: Loader):
    _generate_datastore(ctx, loader)
    _generate_aliases(ctx, loader)


def _generate_datastore(ctx: Context, loader: Loader):
    """
    Generates 
    - app/datastore/DataStore.h
    - app/datastore/DataStore.cpp
    """

    items = [
        Item(loader.config["datastore"], "*", ctx.app_dir/"datastore/DataStore.h",   "datastore/DataStore.h.j2"),
        Item(loader.config["datastore"], "*", ctx.app_dir/"datastore/DataStore.cpp", "datastore/DataStore.cpp.j2"),
    ]

    generate_items(ctx, items)


def _generate_aliases(ctx: Context, loader: Loader):    
    """
    Generates 
    - app/datastore/DataStoreAlias.h
    """

    out_path = ctx.app_dir / f"datastore/DataStoreAlias.h"
    template_path = "datastore/DataStoreAlias.h.j2"

    items = [
        Item(loader.config["datastore"], "*", out_path, template_path),
    ]

    generate_items(ctx, items)
