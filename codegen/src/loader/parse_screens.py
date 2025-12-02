from common.parse_type import render_value
from sortedcontainers import SortedSet
from common.util import print_json, ensure_list
from common.template import upper_first

def parse_screens(config: dict):

    # process screen data
    for screen_data in config["screens"].values():
        if "model" not in screen_data:
            screen_data["model"] = {
                "fields": []
            }
        parse_model(screen_data["model"], config["event_types"])
        parse_model_helpers(screen_data["model"])
        collect_replay_events(screen_data)

        # replace
        screen_data = screen_data.get("replay", False)


def keys_difference(all_keys: list, keys_to_remove: list):
    remove = set(keys_to_remove)
    result = [k for k in all_keys if k not in remove]
    return result

def parse_model(model: dict, event_types: dict):
    """
    Infers field types and default values from event
    """

    parsed_fields = {}
    
    for field_data in model["fields"]:

        if not isinstance(field_data, dict):
            raise TypeError("model field descriptors must be of type dict")

        parsed = {
            "name":         field_data.get("name",     None),
            "type":         field_data.get("type",     None),
            "bind":         field_data.get("bind",     None),
            "dirty":        field_data.get("dirty",    None),
            "group":        field_data.get("group",    None),
            "epsilon":      field_data.get("epsilon",  None),
            "default":      field_data.get("default",  None),
            "debounce":     field_data.get("debounce", None)      
        }

        remaining_keys = keys_difference(field_data.keys(), parsed.keys())
        if len(remaining_keys) > 0:
            raise KeyError(f"invalid key(s): {remaining_keys}")    
        
        if parsed["name"] is None:
            raise KeyError("expected 'name' key")    
        if parsed["name"] in parsed_fields:
            raise ValueError(f"duplicate model field name: {parsed['name']}")
        if (parsed["type"] is None and parsed["bind"] is None):
            raise KeyError("expected one of either 'bind' or 'type' to be present")
        if (parsed["type"] is not None and parsed["bind"] is not None):
            raise KeyError("expected either only 'bind' or only 'type' key, not both")

        if parsed["bind"] is not None:

            event_type, event_field = parsed["bind"].split("->")

            # check if referenced event and field exists
            if event_type not in event_types:
                raise ValueError(f"Unknown event type {event_type}")                
            if event_field not in event_types[event_type]["fields"]:
                raise ValueError(f"Field {event_field!r} not found in event {event_type!r}")

            # infer field type from event
            parsed["type"] = event_types[event_type]["fields"][event_field]["type"]
            parsed["event_type"] = event_type.strip()
            parsed["event_field"] = event_field.strip()
            
            # infer default value from event if no default value set in entry
            if parsed["default"] is None:
                parsed["default"] = event_types[event_type]["fields"][event_field].get("default", None)

        if parsed["type"] != "float" and parsed["epsilon"] is not None:
            raise TypeError("epsilon kwarg is only supported for field type 'float'")
        
        if parsed["epsilon"] is not None:
            parsed["epsilon"] = render_value(parsed["epsilon"], "float")

        if parsed["group"] is None:
            parsed["group"] = upper_first(parsed["name"])
        
        if parsed["dirty"] is not None:
            parsed["dirty"] = ensure_list(parsed["dirty"])

        if parsed["debounce"] is not None:
            parsed["debounce"] = render_value(parsed["debounce"], "uint32_t")

        parsed["default"] = render_value(parsed["default"], parsed["type"])
        parsed = {k: v for k, v in parsed.items() if v is not None} # remove all 'None' key/value pairs
        parsed_fields[parsed["name"]] = parsed
        
    model["fields"] = parsed_fields


def parse_model_helpers(model: dict):

    events = {}
    manual = {}
    groups = {}
    debounce = {}

    for field_name, field_data in model["fields"].items():
        
        event_type = field_data.get("event_type", None)

        # add to manual
        if event_type is None:
            manual[field_name] = field_data
        
        # add to events
        else: 
            if event_type not in events:
                events[event_type] = {}
            events[event_type][field_name] = field_data

        # add to groups
        group = field_data["group"]
        if group is not None:
            if group not in groups:
                groups[group] = {}
            groups[group][field_name] = field_data

        # add to debounce
        debounce_val = field_data.get("debounce", None)
        if debounce_val is not None:
            debounce[field_name] = field_data

    model["events"] = events
    model["manual"] = manual
    model["groups"] = groups
    model["debounce"] = debounce
    

def collect_replay_events(screen: dict):
    events_set = SortedSet()
    events_set.update(screen["model"]["events"].keys())
    events_set.update(screen["receives"])
    screen["replay_events"] = list(events_set)
