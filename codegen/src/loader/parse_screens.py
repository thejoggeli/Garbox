from copy import deepcopy
from common.parse_type import render_value
from sortedcontainers import SortedSet

def parse_screens(config: dict):

    # process screen data
    for screen_data in config["screens"].values():
        if "updaters" not in screen_data:
            screen_data["updaters"] = {}
        parse_updaters(screen_data["updaters"], config["event_types"])
        group_event_updaters(screen_data)
        collect_replay_events(screen_data)


def parse_updaters(updaters: dict, event_types: dict):
    """
    Infers field types and default values from event
    """

    for updater_name, updater_list in updaters.items():

        if not isinstance(updater_list, list):
            raise ValueError("updater value must be list")

        parsed_entries = []
        
        for field_data in updater_list:

            event_name = field_data.get("event", None)

            field_name = None
            field_type = None
            default_value = None

            if(event_name is None):

                # check if required fields are in entry
                if "name" not in field_data:
                    raise KeyError("expected 'name' key in updater list entry")                
                if "type" not in field_data:
                    raise KeyError("expected 'type' key in updater list entry")
                
                field_name = field_data["name"]
                field_type = field_data["type"]
                default_value = field_data.get("default", None)

            else:

                # check if required fields are in entry
                if "name" not in field_data:
                    raise KeyError("expected 'name' key in updater list entry")                
                if "type" in field_data:
                    raise KeyError("expected no 'type' key in updater list entry if 'event' key is present")          

                # check if referenced event and field exists
                field_name = field_data["name"]
                if event_name not in event_types:
                    raise ValueError(f"Unknown event type {event_name}")                
                if field_name not in event_types[event_name]["fields"]:
                    raise ValueError(f"Field {field_name!r} not found in event {event_name!r}")

                # infer field type from event
                field_type = event_types[event_name]["fields"][field_name]["type"]
                
                # infer default value from event if no default value set in entry
                if "default" in field_data:
                    default_value = field_data["default"]
                else: 
                    default_value = event_types[event_name]["fields"][field_name].get("default", None)

            parsed_entries.append({
                "event": event_name,
                "name": field_name,
                "type": field_type,
                "default": render_value(default_value, field_type),
            })
            
        updaters[updater_name] = parsed_entries


def group_event_updaters(screen_data: dict):

    events = {}
    manual = []

    for updater_name, updater_list in screen_data["updaters"].items():
        
        for field_data in updater_list:

            event_name = field_data.get("event", None)

            if(event_name is None):
                manual_entry_data = deepcopy(field_data)
                manual_entry_data["updater"] = updater_name
                manual.append(manual_entry_data)
            
            else:
                if event_name not in events:
                    events[event_name] = {
                        "updaters": [],
                        "updater_types": [],
                    }

                event_entry_data = deepcopy(field_data)
                del event_entry_data["event"]
                event_entry_data["updater"] = updater_name

                events[event_name]["updaters"].append(event_entry_data)

    for event_name, event_data in events.items():
        updater_types = SortedSet()
        for event_updater in event_data["updaters"]:
            updater_types.add(event_updater["updater"])
        events[event_name]["updater_types"] = list(updater_types)        

    screen_data["updaters_by_event"] = events
    screen_data["updaters_manual"] = manual


def collect_replay_events(screen_data: dict):

    events_set = SortedSet()

    for events_name in screen_data["updaters_by_event"].keys():
        events_set.add(events_name)

    for events_name in screen_data["receives"]:
        events_set.add(events_name)

    screen_data["replay_events"] = list(events_set)
