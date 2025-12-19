from common.parse_type import render_value
from common.str_filters import ensure_suffix

def parse_events(config: dict):

    event_types = config["event_types"]

    for event_name, event_data in event_types.items():

        if(event_data is None):
            event_types[event_name] = {"meta": {}, "fields": {}}
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

        event_types[event_name] = {
            "name": event_name,
            "name_with_suffix": ensure_suffix(event_name, "Event"),
            "meta": event_meta,
            "fields": parsed_fields,
        }

