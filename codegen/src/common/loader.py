import yaml
from copy import deepcopy
from pathlib import Path
from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix
)


class Loader:

    _config_dir : Path
    _application_config : dict = None
    _devtools_config : dict = None
    _events_config : dict = None
    _hardware_config : dict = None

    def __init__(self, config_dir: Path):
        self.config_dir = config_dir
        pass


    def preload_all(self):
        self.get_application_config()
        self.get_devtools_config()
        self.get_events_config()
        self.get_hardware_config()


    def get_devtools_config(self):

        if(self._devtools_config is not None):
            return self._devtools_config

        yaml_path = self.config_dir / "devtools.yaml"
        yaml_config = load_yaml(yaml_path)

        self._devtools_config = yaml_config
        return self._devtools_config


    def get_hardware_config(self):

        if(self._hardware_config is not None):
            return self._hardware_config

        yaml_path = self.config_dir / "hardware.yaml"
        yaml_config = load_yaml(yaml_path)

        # enforce required suffix rules
        yaml_config["ledc_timer"] = ensure_dict_keys_have_suffix(yaml_config["ledc_timer"], "Timer")
        yaml_config["ledc_channel"] = ensure_dict_keys_have_suffix(yaml_config["ledc_channel"], "Channel")

        for channel in yaml_config["ledc_channel"].values():
            channel["timer"] = ensure_str_has_suffix(channel["timer"], "Timer")

        self._hardware_config = yaml_config
        return self._hardware_config


    def get_application_config(self):

        if(self._application_config is not None):
            return self._application_config

        yaml_path = self.config_dir / "application.yaml"
        yaml_config = load_yaml(yaml_path)

        for tick_phase in yaml_config["setup"]["tick_phases"]:
            tick_phase["name"] = ensure_str_has_suffix(tick_phase["name"], "Tick")

        for dict_item in yaml_config["behaviours"].values():
            dict_item["tick_phases"] = ensure_list(dict_item["tick_phases"])
            dict_item["sends"] = ensure_list(dict_item["sends"])
            dict_item["receives"] = ensure_list(dict_item["receives"])

        for dict_item in yaml_config["controllers"].values():
            dict_item["tick_phases"] = ensure_list(dict_item["tick_phases"])
            dict_item["sends"] = ensure_list(dict_item["sends"])
            dict_item["receives"] = ensure_list(dict_item["receives"])

        ensure_dict_keys_have_suffix(yaml_config["behaviours"], "Behaviour")
        ensure_dict_keys_have_suffix(yaml_config["controllers"], "Controller")

        for behaviour in yaml_config["behaviours"].values():
            behaviour["tick_phases"] = ensure_list(behaviour["tick_phases"])
            for idx, name in enumerate(behaviour["tick_phases"]):
                behaviour["tick_phases"][idx] = ensure_str_has_suffix(name, "Tick")            

        for controller in yaml_config["controllers"].values():
            controller["tick_phases"] = ensure_list(controller["tick_phases"])
            for idx, name in enumerate(controller["tick_phases"]):
                controller["tick_phases"][idx] = ensure_str_has_suffix(name, "Tick")        

        self._application_config = yaml_config
        return self._application_config


    def get_events_config(self):

        if(self._events_config is not None):
            return self._events_config

        events_path = self.config_dir / "events.yaml"
        system_events_path = self.config_dir / "core/system_events.yaml"
        config = load_yaml_multi([system_events_path, events_path])

        # copy event types into payload (but keep payload includes seperate)
        payloads: dict = deepcopy(config["types"])
        payloads.pop("include_h", None)
        payloads.pop("include_cpp", None)

        # set or update (if already exists) payloads config entriy
        if "payloads" not in config:
            config["payloads"] = payloads
        else:
            config["payloads"].update(payloads)

        self._events_config = config
        return self._events_config


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

