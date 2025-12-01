import yaml
from copy import deepcopy
from pathlib import Path
from common.util import ( 
    ensure_list,
    ensure_dict_keys_have_suffix, 
    ensure_str_has_suffix,
    print_json
)
from loader.parse_application import parse_application
from loader.parse_events import parse_events
from loader.parse_hardware import parse_hardware_config

class Loader:

    config_dir : Path
    config: dict = None

    def __init__(self, config_dir: Path):
        self.config_dir = config_dir
        pass

    def preload_all(self):
        paths = [p for p in self.config_dir.rglob("*") if p.is_file()]
        self.config = load_yaml_multi(paths)

        # parse hardware config
        parse_hardware_config(self.config)

        # parse events (render default values)
        parse_events(self.config)

        # parse application and components including:
        # - application
        # - controllers
        # - behaviours
        # - screens
        # requires events to already be parsed)
        parse_application(self.config)


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

