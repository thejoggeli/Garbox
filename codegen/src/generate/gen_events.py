from common.item import Item
from common.item import Item, generate_items
from common.context import Context


def generate_events(ctx: Context, events_config: dict):
    """
    Generates: 
    - shared/types/EventType.h
    - shared/types/EventType.cpp
    - shared/types/EventPayload.h
    """

    items = [
        Item(events_config, "*", ctx.shared_dir / "types/EventType.h",    "events/EventType.h.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventType.cpp",  "events/EventType.cpp.j2"),
        Item(events_config, "*", ctx.shared_dir / "types/EventPayload.h", "events/EventPayload.h.j2"),
    ]

    generate_items(ctx, items)
