from sortedcontainers import SortedSet
from common.parse_type import render_value
from common.str_filters import ensure_suffix
from common.util import ensure_list

def parse_events(config: dict):

    events = config["events"]

    includes_set = SortedSet()

    for event_name, event_data in events.items():

        includes_list = ensure_list(event_data.get("include", []))
        for include in includes_list:
            includes_set.add(include)

        if(event_data is None):
            events[event_name] = {"meta": {}, "fields": {}}
            continue
    
        event_meta = event_data.get("meta", {})
        event_fields = event_data.get("fields", [])
        if event_fields is None:
            event_fields = []

        parsed_fields = {}

        for event_field in event_fields:

            field_name = event_field["name"]
            field_type = event_field["type"]
            default_value = event_field.get("default", None)

            parsed_fields[field_name] = {
                "name": field_name,
                "type": field_type,
                "default": render_value(default_value, field_type)
            }

        events[event_name] = {
            "name": event_name,
            "name_with_suffix": ensure_suffix(event_name, "Event"),
            "meta": event_meta,
            "fields": parsed_fields,
        }

    config["events_includes"] = list(includes_set)
