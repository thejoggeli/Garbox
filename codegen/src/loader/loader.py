import yaml
from copy import deepcopy
from pathlib import Path
from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix,
    print_json
)
from loader.screen_utils import parse_updaters
from loader.event_utils import parse_event_types

class Loader:

    config_dir : Path
    config: dict = None

    def __init__(self, config_dir: Path):
        self.config_dir = config_dir
        pass

    def preload_all(self):
        paths = [p for p in self.config_dir.rglob("*") if p.is_file()]
        self.config = load_yaml_multi(paths)

        self.process_events_config()
        self.process_hardware_config()
        self.process_behaviours_config()
        self.process_controllers_config()
        self.process_screens_config()
        self.process_application_config()


    def process_hardware_config(self):

        # enforce required suffix rules
        hardware = self.config["hardware"]
        hardware["ledc"]["instances"]["timers"] = ensure_dict_keys_have_suffix(hardware["ledc"]["instances"]["timers"], "Timer")
        hardware["ledc"]["instances"]["channels"] = ensure_dict_keys_have_suffix(hardware["ledc"]["instances"]["channels"], "Channel")

        # ensure timer bindings in ledc channels have 'Timer' suffix
        for channel in hardware["ledc"]["instances"]["channels"].values():
            channel["timer"] = ensure_str_has_suffix(channel["timer"], "Timer")


    def process_application_config(self):

        config = self.config["application"]

        for tick_phase in config["tick_phases"]:
            tick_phase["name"] = ensure_str_has_suffix(tick_phase["name"], "Tick")

        self.build_tick_phase_usage()

        # order tick phases
        next_order = 0
        for tick_dict in config["tick_phases"]:
            if("order" not in tick_dict):
                tick_dict["order"] = next_order
                next_order += 1
        config["tick_phases"] = sorted(config["tick_phases"], key=lambda x: x["order"])


    def build_tick_phase_usage(self):
        phases = self.config["application"]["tick_phases"]
        idx = {}
        for i, phase in enumerate(phases):
            phase["behaviours"] = []
            phase["controllers"] = []
            phase["screens"] = []
            idx[phase["name"]] = i
        for section in ("controllers", "behaviours", "screens"):
            for comp_key, comp_data in self.config[section].items(): 
                comp_data["tick_phases"] = ensure_list(comp_data["tick_phases"])
                for phase in comp_data["tick_phases"]:
                    phases[idx[phase]][section].append(comp_key)

    
    def _process_components_config(self, key, suffix):

        self.config[key] = ensure_dict_keys_have_suffix(self.config[key], suffix)

        for dict_item in self.config[key].values():
            dict_item["tick_phases"] = ensure_list(dict_item["tick_phases"])
            dict_item["sends"] = ensure_list(dict_item["sends"])
            dict_item["receives"] = ensure_list(dict_item["receives"])

        for component in self.config[key].values():
            component["tick_phases"] = ensure_list(component["tick_phases"])
            for idx, name in enumerate(component["tick_phases"]):
                component["tick_phases"][idx] = ensure_str_has_suffix(name, "Tick")  


    def process_behaviours_config(self):
        self._process_components_config("behaviours", "Behaviour")  
             

    def process_controllers_config(self):
        self._process_components_config("controllers", "Controller")
             

    def process_screens_config(self):
        self._process_components_config("screens", "Screen")

        for screen_data in self.config["screens"].values():
            if "updaters" in screen_data:
                parse_updaters(screen_data["updaters"], self.config["event_types"])
                # print_json(screen_data["updaters"])
                print_json(screen_data["updaters"]["HeatpadSense"])
                

    def process_events_config(self):

        parse_event_types(self.config["event_types"])


def load_yaml(path: Path):
    print(f"{path}")
    with path.open("r") as f:
        return yaml.safe_load(f)


def merge_yaml(a, b):
    """
    Recursively merge b into a.
    - dict + dict -> merged
    - list + list -> concatenated
    - everything else -> b overwrites a
    """
    if isinstance(a, dict) and isinstance(b, dict):
        result = dict(a)
        for key, b_val in b.items():
            if key in result:
                result[key] = merge_yaml(result[key], b_val)
            else:
                result[key] = b_val
        return result

    if isinstance(a, list) and isinstance(b, list):
        return a + b

    return b


def load_yaml_multi(paths: list[Path]):
    merged = {}
    for path in paths:
        data = load_yaml(path)
        if data is None:
            continue
        merged = merge_yaml(merged, data)
    return merged

