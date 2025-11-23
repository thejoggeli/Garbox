import copy
from common.item import Item
from common.item import Item, generate_items
from common.context import Context


def generate_events(ctx: Context, events_config: dict):
    """
    Load event YAML, merge types into payload, and generate
    EventType.h/.cpp + EventPayload.h.
    """

    items = [
        Item(events_config, "types",    ctx.shared_dir / "types/EventType.h",    "events/EventType.h.j2"),
        Item(events_config, "types",    ctx.shared_dir / "types/EventType.cpp",  "events/EventType.cpp.j2"),
        Item(events_config, "payloads", ctx.shared_dir / "types/EventPayload.h", "events/EventPayload.h.j2"),
    ]

    generate_items(ctx, items)
