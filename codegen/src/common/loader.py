import yaml
from copy import deepcopy
from pathlib import Path
from common.util import ensure_list, ensure_suffix, ensure_value_suffix


def load_hardware_config(config_dir: Path):
    yaml_path = config_dir / "hardware.yaml"
    yaml_config = load_yaml(yaml_path)

    # enforce required suffix rules
    ensure_suffix(yaml_config, "ledcTimer", "Timer")
    ensure_suffix(yaml_config, "ledcChannel", "Channel")
    ensure_value_suffix(yaml_config, "ledcChannel", "timer", "Timer")

    return yaml_config


def load_application_config(config_dir: Path):

    yaml_path = config_dir / "application.yaml"
    yaml_config = load_yaml(yaml_path)

    for dict_item in yaml_config["behaviours"].values():
        dict_item["ticks"] = ensure_list(dict_item["ticks"])
        dict_item["sends"] = ensure_list(dict_item["sends"])
        dict_item["receives"] = ensure_list(dict_item["receives"])

    for dict_item in yaml_config["controllers"].values():
        dict_item["ticks"] = ensure_list(dict_item["ticks"])
        dict_item["sends"] = ensure_list(dict_item["sends"])
        dict_item["receives"] = ensure_list(dict_item["receives"])

    ensure_suffix(yaml_config, "behaviours", "Behaviour")
    ensure_value_suffix(yaml_config, "behaviours", "ticks", "Tick")
    ensure_suffix(yaml_config, "controllers", "Controller")
    ensure_value_suffix(yaml_config, "controllers", "ticks", "Tick")

    return yaml_config


def load_events_config(config_dir: Path):
    events_path = config_dir / "events.yaml"
    system_events_path = config_dir / "core/system_events.yaml"
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

    return config


def load_yaml(path: Path):
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
        with path.open("r") as f:
            data = yaml.safe_load(f)
        if data is None:
            continue
        merged = merge_yaml(merged, data)
    return merged

