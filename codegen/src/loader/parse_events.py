from common.parse_type import render_value

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

        event_kind = event_data.get("kind", None)
        if event_kind is None:
            raise ValueError(f"event 'kind' is missing: {event_name}")
        
        if event_kind == "snapshot" and len(event_fields) == 0:
            raise ValueError(f"event of kind 'snapshot' must have at least 1 field: {event_name}")

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
            "meta": event_meta,
            "kind": event_kind,
            "fields": parsed_fields,
        }

