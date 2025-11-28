import yaml
from copy import deepcopy
from pathlib import Path
from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix,
    print_json
)


class Loader:

    config_dir : Path
    config: dict = None

    def __init__(self, config_dir: Path):
        self.config_dir = config_dir
        pass

    def preload_all(self):
        paths = [p for p in self.config_dir.rglob("*") if p.is_file()]
        self.config = load_yaml_multi(paths)

        print_json(self.config)

        self.process_hardware_config()
        self.process_application_config()
        self.process_behaviours_config()
        self.process_controllers_config()
        self.process_screens_config()
        self.process_events_config()
    

    def process_hardware_config(self):

        # enforce required suffix rules
        hardware = self.config["hardware"]
        hardware["ledc_timer"] = ensure_dict_keys_have_suffix(hardware["ledc_timer"], "Timer")
        hardware["ledc_channel"] = ensure_dict_keys_have_suffix(hardware["ledc_channel"], "Channel")

        for channel in hardware["ledc_channel"].values():
            channel["timer"] = ensure_str_has_suffix(channel["timer"], "Timer")


    def process_application_config(self):

        config = self.config["application"]

        for tick_phase in config["tick_phases"]:
            tick_phase["name"] = ensure_str_has_suffix(tick_phase["name"], "Tick")

        # order tick phases
        next_order = 0
        for tick_dict in config["tick_phases"]:
            if("order" not in tick_dict):
                tick_dict["order"] = next_order
                next_order += 1
        config["tick_phases"] = sorted(config["tick_phases"], key=lambda x: x["order"])

    
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
        

    def process_events_config(self):

        config = self.config["events"]

        # copy event types into payload (but keep payload includes seperate)
        payloads: dict = deepcopy(config["types"])
        payloads.pop("include_h", None)
        payloads.pop("include_cpp", None)

        # set or update (if already exists) payloads config entriy
        if "payloads" not in config:
            config["payloads"] = payloads
        else:
            config["payloads"].update(payloads)


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

