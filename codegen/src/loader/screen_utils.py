from common.bracket_string import BracketString
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

        for raw_entry_string in entry_list:

            bracket_string = BracketString(raw_entry_string)
            
            entry_type = bracket_string.name

            if(entry_type == "FromEvent"):

                if(len(bracket_string.args) != 2):
                    raise ValueError("expected two positional arguments (event_type, field_name) for type 'FromEvent'")

                event_name = bracket_string.require_get_arg(0)
                field_name = bracket_string.require_get_arg(1)
                field_type = infer_field_type_from_event(event_types, event_name, field_name)

                parsed_entries.append({
                    "entry_type": "FromEvent",
                    "event_name": event_name,
                    "field_name": field_name,
                    "field_type": field_type,
                    "kwargs": bracket_string.kwargs,
                })

            elif(entry_type == "Manual"):

                if(len(bracket_string.args) != 2):
                    raise ValueError("expected two positional argument (field_name, field_type) for type 'Manual'")

                field_name = bracket_string.require_get_arg(0)
                field_type = bracket_string.require_get_arg(1)

                parsed_entries.append({
                    "entry_type": "Manual",
                    "field_name": field_name,
                    "field_type": field_type,
                    "kwargs": bracket_string.kwargs,
                })
                
            else:
                raise ValueError(f"invalid updater entry type: {entry_type} in {raw_entry_string}")

        # After all entries for one updater are parsed -> type them
        render_kwargs(parsed_entries)

        updaters[updater_name] = parsed_entries


def infer_field_type_from_event(event_types: dict, event_name: str, field_name: str):
    """
    Add full type information (from event types if available)
    """
    
    if event_name not in event_types:
        raise ValueError(f"Unknown event type {event_name}")

    if field_name not in event_types[event_name]["fields"]:
        raise ValueError(f"Field {field_name!r} not found in event {event_name!r}")
        
    return event_types[event_name]["fields"][field_name]["type"]


def render_kwargs(updater_entries: dict):

    for entry in updater_entries:

        # print(entry)

        kwargs = entry["kwargs"]
        for arg_name, arg_value in kwargs.items():

            if(arg_name == "default"):
                kwargs[arg_name] = render_value(arg_value, entry["field_type"])
            elif(arg_name == "epsilon"):
                kwargs[arg_name] = render_value(arg_value, entry["field_type"])

