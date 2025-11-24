from common.item import Item
from common.item import Item, generate_items
from common.context import Context
from common.loader import Loader


def generate_events(ctx: Context, loader: Loader):
    """
    Generates: 
    - shared/types/EventAlias.h
    - shared/types/EventType.h
    - shared/types/EventType.cpp
    - shared/types/EventPayload.h
    """

    events_config = loader.get_events_config()

    items = [
        Item(events_config, "*", ctx.shared_dir / "types/EventAlias.h",   "events/EventAlias.h.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventType.h",    "events/EventType.h.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventType.cpp",  "events/EventType.cpp.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventPayload.h", "events/EventPayload.h.j2", meta_key="payloads_meta"),
    ]

    generate_items(ctx, items)
