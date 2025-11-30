from common.bracket_string import parse_bracket_string
from common.parse_type import render_value

def parse_updaters(updaters: dict, event_types: dict):
    """
    Convert bracket-style updater definitions into structured event/manual records.

    Input example:
        {
            "FanState": [
                "FanStatus(state)",
                "FanStatus(targetSpeed, epsilon=1e-3, default=0.0)"
            ],
            "DisplayStatus": [
                "dirtyCount"
            ]
        }

    Output example:
        {
            "FanState": [
                {"type": "event", "event": "FanStatus",
                 "field": "state", "args": {}, "default": None},
                {"type": "event", "event": "FanStatus",
                 "field": "targetSpeed", "args": {"epsilon": 1e-3},
                 "default": 0.0}
            ],
            "DisplayStatus": [
                {"type": "manual", "field": "dirtyCount",
                 "args": {}, "default": None}
            ]
        }

    Parsing steps:
        - For entries with parentheses, use parse_bracket_string()
        - For bare identifiers (e.g. "dirtyCount"), treat as manual fields
        - Expand multiple positional fields into multiple updater entries
        - After building the raw list, pass it to parse_typed_arguments()
    """

    for updater_name, entry_list in updaters.items():
        parsed_entries = []

        for entry_raw_string in entry_list:
            entry_raw_string = entry_raw_string.strip()

            # bracket-style event mapping
            parsed_entry = parse_bracket_string(entry_raw_string)
            
            entry_type = parsed_entry["name"]

            if(entry_type == "FromEvent"):

                if(len(parsed_entry["args"]) != 2):
                    raise ValueError("expected two positional arguments (event_type, field_name) for type 'FromEvent'")

                event_name = parsed_entry["args"][0]
                field_name = parsed_entry["args"][1]
                field_type = infer_field_type_from_event(event_types, event_name, field_name)

                parsed_entries.append({
                    "type": "FromEvent",
                    "event_name": event_name,
                    "field_name": field_name,
                    "field_type": field_type,
                    "kwargs": parsed_entry["kwargs"],
                })

            elif(entry_type == "Manual"):

                if(len(parsed_entry["args"]) != 2):
                    raise ValueError("expected two positional argument (field_name, field_type) for type 'Manual'")

                field_name = parsed_entry["args"][0]
                field_type = parsed_entry["args"][1]

                parsed_entries.append({
                    "type": "Manual",
                    "field_name": field_name,
                    "field_type": field_type,
                    "kwargs": parsed_entry["kwargs"],
                })
                
            else:
                raise ValueError(f"invalid updater entry type: {entry_type} in {entry_raw_string}")
            
        print(parsed_entries)

        # After all entries for one updater are parsed → type them
        render_kwargs(parsed_entries)

        updaters[updater_name] = parsed_entries


def infer_field_type_from_event(event_types: dict, event_name: str, field_name: str):
    """
    Add full type information (from event types if available)
    """
    
    if event_name not in event_types:
        raise ValueError(f"Unknown event type {event_name!r}")

    if field_name not in event_types[event_name]:
        raise ValueError(f"Field {field_name!r} not found in event {event_name!r}")
    
    return event_types[event_name][field_name]


def render_kwargs(updater_list: dict):

    for entry in updater_list:

        kwargs = entry["kwargs"]
        for arg_name, arg_value in kwargs.items():

            if(arg_name == "default"):
                kwargs[arg_name] = render_value(arg_value, entry["field_type"])
            elif(arg_name == "epsilon"):
                kwargs[arg_name] = render_value(arg_value, entry["field_type"])

