from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix
)
from common.str_filters import ensure_suffix
from loader.parse_screens import parse_screens

COMPONENT_TYPES = ["controllers", "behaviours", "screens"]

def parse_application(config: dict):

    # process components
    process_components(config, "behaviours", "Behaviour")  
    process_components(config, "controllers", "Controller")
    process_components(config, "screens", "Screen")

    # access all components
    config["components"] = {
        **config["behaviours"],
        **config["controllers"],
        **config["screens"]
    }

    # process tick phases
    process_tick_phases(config)

    # parse screens
    parse_screens(config)


def process_components(config, key, suffix):

    config[key] = ensure_dict_keys_have_suffix(config[key], suffix)

    for name, component in config[key].items():

        # init 'receive_events' field
        component["receive_events"] = ensure_list(component.get("receive_events", []))
        component["receive_events_all"] = component.get("receive_events_all", False)
        if len(component["receive_events"]) > 0:
            first_entry = component["receive_events"][0]

            # insert all event types (N event handlers)
            if first_entry == "INSERT_ALL":
                component["receive_events"] = list(config["event_types"].keys())

        # init 'tick_phases' field
        component["tick_phases"] = ensure_list(component["tick_phases"])
        component["tick_phases_all"] = component.get("tick_phases_all", False)
        if len(component["tick_phases"]) > 0:
            first_entry = component["tick_phases"][0]

            # insert all tick phases (N tick handlers)
            if first_entry == "INSERT_ALL":
                component["tick_phases"] = [phase["name"] for phase in config["application"]["tick_phases"]]
            
        # tick phase entry must have suffix 'Tick'
        for idx, name in enumerate(component["tick_phases"]):
            component["tick_phases"][idx] = ensure_suffix(name, "Tick")  
        
        # init 'send_events' field
        component["send_events"] = ensure_list(component.get("send_events", []))

        # component name
        component["name"] = name
        

def process_tick_phases(config):

    for tick_phase in config["application"]["tick_phases"]:
        tick_phase["name"] = ensure_suffix(tick_phase["name"], "Tick")

    # add list of which component uses each tick to application.tick_phases
    phases = config["application"]["tick_phases"]
    idx = {}
    for i, phase in enumerate(phases):
        phase["behaviours"] = {}
        phase["controllers"] = {}
        phase["screens"] = {}
        idx[phase["name"]] = i
    for section in COMPONENT_TYPES:
        for comp_key, comp_data in config[section].items(): 
            
            # add component to all tick phases
            if(comp_data["tick_phases_all"] == True):
                for phase_idx in range(len(phases)):
                    phases[phase_idx][section][comp_key] = comp_data

            # add component to selected tick phases
            else:
                comp_data["tick_phases"] = comp_data["tick_phases"]
                for phase in comp_data["tick_phases"]:
                    phases[idx[phase]][section][comp_key] = comp_data

    # order tick phases
    next_order = 0
    for tick_dict in config["application"]["tick_phases"]:
        if("order" not in tick_dict):
            tick_dict["order"] = next_order
            next_order += 1

    config["application"]["tick_phases"] = sorted(config["application"]["tick_phases"], key=lambda x: x["order"])


