from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix,
)
from loader.parse_screens import parse_screens

COMPONENT_TYPES = ["controllers", "behaviours", "screens"]

def parse_application(config: dict):

    # process components
    process_components(config, "behaviours", "Behaviour")  
    process_components(config, "controllers", "Controller")
    process_components(config, "screens", "Screen")

    # process tick phases
    process_tick_phases(config)

    # parse screens
    parse_screens(config)


def process_components(config, key, suffix):

    config[key] = ensure_dict_keys_have_suffix(config[key], suffix)

    for dict_item in config[key].values():
        dict_item["tick_phases"] = ensure_list(dict_item["tick_phases"])
        dict_item["sends"] = ensure_list(dict_item["sends"])
        dict_item["receives"] = ensure_list(dict_item["receives"])

    for component in config[key].values():
        component["tick_phases"] = ensure_list(component["tick_phases"])
        for idx, name in enumerate(component["tick_phases"]):
            component["tick_phases"][idx] = ensure_str_has_suffix(name, "Tick")  


def process_tick_phases(config):

    for tick_phase in config["application"]["tick_phases"]:
        tick_phase["name"] = ensure_str_has_suffix(tick_phase["name"], "Tick")

    # add list of which component uses each tick to application.tick_phases
    phases = config["application"]["tick_phases"]
    idx = {}
    for i, phase in enumerate(phases):
        phase["behaviours"] = []
        phase["controllers"] = []
        phase["screens"] = []
        idx[phase["name"]] = i
    for section in COMPONENT_TYPES:
        for comp_key, comp_data in config[section].items(): 
            comp_data["tick_phases"] = ensure_list(comp_data["tick_phases"])
            for phase in comp_data["tick_phases"]:
                phases[idx[phase]][section].append(comp_key)

    # order tick phases
    next_order = 0
    for tick_dict in config["application"]["tick_phases"]:
        if("order" not in tick_dict):
            tick_dict["order"] = next_order
            next_order += 1

    config["application"]["tick_phases"] = sorted(config["application"]["tick_phases"], key=lambda x: x["order"])


