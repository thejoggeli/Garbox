import yaml
import copy
import json
from pathlib import Path
from jinja2 import Environment, FileSystemLoader, StrictUndefined

# global paths
script_dir = Path(__file__).parent
template_dir = script_dir.joinpath("templates")
src_dir = script_dir.joinpath("../src").resolve()

# create Jinja environment
jinja_env = Environment(
    loader=FileSystemLoader(template_dir),
    trim_blocks=True,
    lstrip_blocks=True,
    undefined=StrictUndefined,
)

class Item:
    def __init__(self, yaml_config, yaml_keys, out_path, template_path):
        self.yaml_config = yaml_config
        self.yaml_keys = ensure_list(yaml_keys)
        self.out_path = out_path
        self.template_path = template_path


def print_json(data):
    print(json.dumps(data, indent=2))

        
def ensure_list(x):
    if x is None:
        return []
    return x if isinstance(x, list) else [x]


def ensure_suffix(yaml_config: dict, parent: str, suffix: str):
    """
    Ensures that all keys under yaml_config[parent] end with the given suffix.
    If a key doesn't, rename it to key+suffix.

    Modifies yaml_config in place.
    """

    block = yaml_config.get(parent)
    if block is None:
        raise KeyError(f"Parent key '{parent}' not found in YAML config")

    if not isinstance(block, dict):
        raise TypeError(f"yaml_config['{parent}'] must be a dict")

    renamed = {}

    for key, value in block.items():

        key_str = str(key)  # ensure string keys
        if(key_str == "include_cpp" or key_str == "include_h"):
            renamed[key_str] = value
            continue

        new_key = key_str if key_str.endswith(suffix) else key_str + suffix
        if new_key in renamed:
            raise KeyError(
                f"Suffix operation would overwrite existing key '{new_key}'. "
                f"Original key: '{key_str}'"
            )
        renamed[new_key] = value

    # Replace block content with renamed version
    yaml_config[parent] = renamed

    return yaml_config


def ensure_value_suffix(yaml_config: dict, parent: str, field: str, suffix: str):
    """
    Ensures that yaml_config[parent][*][field] ends with suffix.
    Example: ledcChannel[*]['timer'] must end with 'Timer'.
    """
    block = yaml_config.get(parent)
    if block is None:
        raise KeyError(f"Parent key '{parent}' not found")

    if not isinstance(block, dict):
        raise TypeError(f"yaml_config['{parent}'] must be a dict")

    for key, entry in block.items():
        if(key == "include_cpp" or key == "include_h"):
            continue
        if field not in entry:
            raise KeyError(
                f"Entry '{key}' in '{parent}' does not contain required field '{field}'"
            )
        
        if isinstance(entry[field], list):
            for i, v in enumerate(entry[field]):
                val = str(v)
                if not val.endswith(suffix):
                    entry[field][i] = val + suffix
        else:
            val = str(entry[field])
            if not val.endswith(suffix):
                entry[field] = val + suffix

    return yaml_config


def extract_includes(config, extension, remove=True):
    include_key = None
    if(extension == ".h"):
        include_key = "include_h"
    elif(extension == ".cpp"):
        include_key = "include_cpp"
    
    includes_list = []
    if(include_key is not None):
        includes_set = set()
        for config_entry in config.values():
            if include_key in config_entry:
                for include in config_entry[include_key]:
                    includes_set.add(include)
        includes_list = list(includes_set)
        

    if(remove):
        for config_entry in config.values():
            if "include_h" in config_entry:
                del config_entry["include_h"]
            if "include_cpp" in config_entry:
                del config_entry["include_cpp"]
    
    return includes_list


def load_yaml(path: Path):
    with path.open("r") as f:
        return yaml.safe_load(f)


def render_template(jinja_env, template_name: str, output_path: Path, **context):
    template = jinja_env.get_template(template_name)
    result = template.render(**context)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(result)

    print(f"Generated {output_path}")


def generate_items(items : list[Item], base_path=src_dir):

    # generate the source code for all passed items
    for item in items:

        # generate full output and template path
        yaml_config = item.yaml_config
        out_path = base_path.joinpath(item.out_path).resolve()
        template_path = item.template_path

        # get current item config from yaml
        # ['a', 'b'] => {'a': yaml_config['a'], 'b': yaml_config['b']}
        config_copy = copy.deepcopy({ key: yaml_config[key] for key in item.yaml_keys })
        
        # create include list
        includes = extract_includes(config_copy, out_path.suffix)
        if(includes is not None and len(includes) > 0):
            config_copy["include"] = includes

        # generate the code file using jinja
        render_template(jinja_env, template_path, out_path, **config_copy)


def generate_hardware():

    yaml_path = script_dir.joinpath("config/hardware.yaml")
    yaml_config = load_yaml(yaml_path)

    # ensure suffixes
    ensure_suffix(yaml_config, "ledcTimer", "Timer")
    ensure_suffix(yaml_config, "ledcChannel", "Channel")
    ensure_value_suffix(yaml_config, "ledcChannel", "timer", "Timer")

    # list of items to be generated
    items = [
        Item(yaml_config, "adc",   out_path="app/hardware/AdcInstances.h",     template_path="hardware/AdcInstances.h.j2"),
        Item(yaml_config, "adc",   out_path="app/hardware/AdcInstances.cpp",   template_path="hardware/AdcInstances.cpp.j2"),
        Item(yaml_config, "gpio",  out_path="app/hardware/GpioInstances.h",    template_path="hardware/GpioInstances.h.j2"),
        Item(yaml_config, "gpio",  out_path="app/hardware/GpioInstances.cpp",  template_path="hardware/GpioInstances.cpp.j2"),
        Item(yaml_config, "i2c",   out_path="app/hardware/I2cInstances.h",     template_path="hardware/I2cInstances.h.j2"),
        Item(yaml_config, "i2c",   out_path="app/hardware/I2cInstances.cpp",   template_path="hardware/I2cInstances.cpp.j2"),
        Item(yaml_config, "spi",   out_path="app/hardware/SpiInstances.h",     template_path="hardware/SpiInstances.h.j2"),
        Item(yaml_config, "spi",   out_path="app/hardware/SpiInstances.cpp",   template_path="hardware/SpiInstances.cpp.j2"),
        Item(yaml_config, "timer", out_path="app/hardware/TimerInstances.h",   template_path="hardware/TimerInstances.h.j2"),
        Item(yaml_config, "timer", out_path="app/hardware/TimerInstances.cpp", template_path="hardware/TimerInstances.cpp.j2"),
        Item(yaml_config, ["ledcTimer", "ledcChannel"], out_path="app/hardware/LedcInstances.h",   template_path="hardware/LedcInstances.h.j2"),
        Item(yaml_config, ["ledcTimer", "ledcChannel"], out_path="app/hardware/LedcInstances.cpp", template_path="hardware/LedcInstances.cpp.j2"),
    ]

    # HardwareInit
    init_mapping = {
        "adc": "AdcInstances",
        "gpio": "GpioInstances",
        "i2c": "I2cInstances",
        "spi": "SpiInstances",
        "timer": "TimerInstances",
        "ledcTimer": "LedcInstances",
    }
    yaml_config["init"] = {}
    for key in init_mapping:
        if key in yaml_config and len(yaml_config[key].keys()) > 0:
            yaml_config["init"][key] = init_mapping[key]
    items.append(Item(yaml_config, "init", out_path="app/hardware/HardwareInit.h",   template_path="hardware/HardwareInit.h.j2"))
    items.append(Item(yaml_config, "init", out_path="app/hardware/HardwareInit.cpp", template_path="hardware/HardwareInit.cpp.j2"))

    # generate all hardware h/cpp files
    generate_items(items)


def generate_events():

    yaml_path = script_dir.joinpath("config/events.yaml")
    yaml_config = load_yaml(yaml_path)

    types_copy = copy.deepcopy(yaml_config["type"])
    if "include_h" in types_copy:
        del types_copy["include_h"]
    if "include_cpp" in types_copy:
        del types_copy["include_cpp"]
    if("payload" not in yaml_config):
        yaml_config["payload"] = types_copy
    else:
        yaml_config["payload"].update(types_copy)

    # list of items to be generated
    items = [
        Item(yaml_config, "type", out_path="shared/types/EventType.h",       template_path="events/EventType.h.j2"),
        Item(yaml_config, "type", out_path="shared/types/EventType.cpp",     template_path="events/EventType.cpp.j2"),
        Item(yaml_config, "payload", out_path="shared/types/EventPayload.h", template_path="events/EventPayload.h.j2"),
    ]

    # generate all hardware h/cpp files
    generate_items(items)


def generate_components(yaml_config, type_key, type_name):

    # list of items to be generated
    items = []
    stubs = []
    for controller_key, controller_dict in yaml_config.items():
        out_path = f"app/{type_key}s/generated/{controller_key}Abs"
        template_path = f"application/{type_key}/{type_name}Abs"
        item_dict = {
            "name": controller_key,
            f"{type_key}": controller_dict
        }
        controller_dict["name"] = controller_key
        items.append(Item(item_dict, [type_key, "name"], f"{out_path}.h", f"{template_path}.h.j2"))
        items.append(Item(item_dict, [type_key, "name"], f"{out_path}.cpp", f"{template_path}.cpp.j2"))

        out_path = f"stubs/{type_key}s/{controller_key}"
        template_path = f"application/{type_key}/{type_name}Stub"
        stubs.append(Item(item_dict, [type_key, "name"], f"{out_path}.h", f"{template_path}.h.j2"))
        stubs.append(Item(item_dict, [type_key, "name"], f"{out_path}.cpp", f"{template_path}.cpp.j2"))

    # generate all hardware h/cpp files
    generate_items(items)
    generate_items(stubs, base_path=script_dir)


def generate_runtime(yaml_config, events_config):

    behaviours = yaml_config["behaviours"]
    controllers = yaml_config["controllers"]
    ticks = set()
    behaviour_ticks = set()
    controller_ticks = set()
    event_routes = {}

    # behaviour ticks
    for item_dict in behaviours.values():
        ticks.update(item_dict["ticks"])
        behaviour_ticks.update(item_dict["ticks"])

    # controller ticks
    for item_dict in controllers.values():
        ticks.update(item_dict["ticks"])
        controller_ticks.update(item_dict["ticks"])

    for event_key in events_config["type"].keys():
        event_routes[event_key] = {"controllers": set(), "behaviours": set()}

    # controller event routes
    for ctrl_key, ctrl_dict in controllers.items():
        if(ctrl_dict["receives"] is None or len(ctrl_dict["receives"]) == 0):
            continue
        for event in ctrl_dict["receives"]:
            event_routes[event]["controllers"].add(ctrl_key)
        
    # behaviour event routes
    for behaviour_key, behaviour_dict in behaviours.items():
        if(behaviour_dict["receives"] is None or len(behaviour_dict["receives"]) == 0):
            continue
        for event in behaviour_dict["receives"]:
            event_routes[event]["behaviours"].add(behaviour_key)

    for route in event_routes.values():
        route["controllers"] = list(route["controllers"])
        route["behaviours"] = list(route["behaviours"])

    name =  yaml_config["name"]
    out_path = f"app/runtime/{name}Runtime"
    template_path = f"application/Runtime"
    runtime_dict = {
        "name": name,
        "initial_behaviour": yaml_config["initial_behaviour"],
        "behaviours": behaviours,
        "controllers": controllers,
        "ticks": list(ticks),
        "behaviour_ticks": list(behaviour_ticks),
        "controller_ticks": list(controller_ticks),
        "event_routes": event_routes,
    }
    items = []
    items.append(Item(runtime_dict, list(runtime_dict.keys()), f"{out_path}.h", f"{template_path}.h.j2"))
    items.append(Item(runtime_dict, list(runtime_dict.keys()), f"{out_path}.cpp", f"{template_path}.cpp.j2"))
    generate_items(items)


def generate_components_ids(yaml_config):
    all_names = set()
    all_names.update(yaml_config["controllers"])
    all_names.update(yaml_config["behaviours"])

    out_path = f"shared/types/ComponentId"
    template_path = f"application/ComponentId"
    comp_dict = {
        "names": list(all_names)
    }
    items = []
    items.append(Item(comp_dict, list(comp_dict.keys()), f"{out_path}.h", f"{template_path}.h.j2"))
    items.append(Item(comp_dict, list(comp_dict.keys()), f"{out_path}.cpp", f"{template_path}.cpp.j2"))
    out_path = f"shared/types/BehaviourId"
    template_path = f"application/BehaviourId"
    items.append(Item(yaml_config, "behaviours", f"{out_path}.h", f"{template_path}.h.j2"))
    items.append(Item(yaml_config, "behaviours", f"{out_path}.cpp", f"{template_path}.cpp.j2"))
    out_path = f"shared/types/ControllerId"
    template_path = f"application/ControllerId"
    items.append(Item(yaml_config, "controllers", f"{out_path}.h", f"{template_path}.h.j2"))
    items.append(Item(yaml_config, "controllers", f"{out_path}.cpp", f"{template_path}.cpp.j2"))
    generate_items(items)


def generate_application():

    yaml_path = script_dir.joinpath("config/application.yaml")
    yaml_config = load_yaml(yaml_path)

    events_path = script_dir.joinpath("config/events.yaml")
    events_config = load_yaml(events_path)

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
    generate_components(yaml_config["controllers"], "controller", "Controller")
    generate_components(yaml_config["behaviours"], "behaviour", "Behaviour")
    generate_runtime(yaml_config, events_config)
    generate_components_ids(yaml_config)

def main():
    generate_hardware()
    generate_events()
    generate_application()


if __name__ == "__main__":
    main()
