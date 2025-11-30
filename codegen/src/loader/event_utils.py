from common.bracket_string import BracketString
from common.parse_type import render_value, get_null_value
from common.util import ensure_list

def parse_event_types(event_types: dict):

    print(event_types)

    for event_name, event_entries in event_types.items():

        parsed_entries = {
            "meta": {},
            "fields": {}, 
        }

        if(event_entries is None):
            event_types[event_name] = parsed_entries
            continue
        
        if not isinstance(event_entries, list):
            raise TypeError(f"event data must be of type list: {event_name}") 

        for raw_entry_string in event_entries:

            print(f"raw_entry_string: {raw_entry_string}")

            print(raw_entry_string)

            if(raw_entry_string.strip() == ""):
                continue

            # bracket-style event mapping
            bracket_string = BracketString(raw_entry_string)
            
            entry_type = bracket_string.name

            if(entry_type == "Field"):

                if(len(bracket_string.args) != 2):
                    raise ValueError(f"expected two positional arguments (field_name, field_type) for type 'Field' in {event_name}:{bracket_string.name}")

                field_name = bracket_string.require_get_arg(0)
                field_type = bracket_string.require_get_arg(1)
                
                default_value = bracket_string.try_get_kwarg("default", get_null_value(field_type))

                parsed_entries["fields"][field_name] = {
                    "name": field_name,
                    "type": field_type,
                    "default": render_value(default_value, field_type)
                }

            else:
                raise ValueError(f"invalid event entry type: {entry_type} in {raw_entry_string}")

        event_types[event_name] = parsed_entries

