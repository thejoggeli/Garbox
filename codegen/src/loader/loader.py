import yaml
import json
import os
from pathlib import Path
from loader.parse_application import parse_application
from loader.parse_events import parse_events
from loader.parse_states import parse_states
from loader.parse_hardware import parse_hardware_config
from loader.gui.parse_guis import parse_guis
from loader.validate_config import validate_gui_resources, validate_raw_config
from common.context import Context
from common.util import decycle, print_section

class Loader:

    config_dir : Path
    output_dir : Path

    config: dict = None

    def __init__(self, ctx: Context):
        self.config_dir = ctx.config_dir
        self.output_dir = ctx.output_dir
        self.res_dir = ctx.res_dir
        pass

    def preload_all(self):

        # load yaml files
        paths = sorted(
            (p for p in self.config_dir.rglob("*.yaml") if p.is_file()),
            key=lambda p: (len(p.relative_to(self.config_dir).parts), p.relative_to(self.config_dir).as_posix()),
        )
        self.config = load_yaml_multi(paths)
        validate_raw_config(self.config)

        # load xml files
        gui_dir = self.config_dir / "gui" 
        gui_paths = sorted(p for p in gui_dir.glob("*.xml") if p.is_file())
        gui_configs_text = {}
        for path in gui_paths:
            config_text = load_text(path)
            config_name = path.stem
            gui_configs_text[config_name] = config_text 

        print_section("parsing configs")

        # parse hardware config
        parse_hardware_config(self.config)

        # parse events
        parse_events(self.config)

        # parse states
        parse_states(self.config)

        # parse gui xml configs  
        self.config["guis"] = parse_guis(gui_configs_text, self.output_dir / "gui")
        validate_gui_resources(self.config["guis"], self.res_dir)

        # parse application and components including:
        # - application
        # - controllers
        # - behaviours
        # - screens
        # requires events to already be parsed
        # requires states to already be parsed
        parse_application(self.config)       


    def save_json(self, path: Path, config):
        print(f"{path}")
        os.makedirs(path.parent, exist_ok=True)
        with open(path, "w") as file:
            json.dump(decycle(config), file, indent=2)


    def save_config(self, path: Path):
        for name in self.config.keys():
            self.save_json(path / f"{name}.json", self.config[name])
            

class UniqueKeyLoader(yaml.SafeLoader):
    """YAML loader that rejects duplicate keys instead of overwriting them."""


def _construct_unique_mapping(loader, node, deep=False):
    mapping = {}
    for key_node, value_node in node.value:
        key = loader.construct_object(key_node, deep=deep)
        if key in mapping:
            mark = key_node.start_mark
            raise ValueError(
                f"duplicate YAML key '{key}' at "
                f"{mark.name}:{mark.line + 1}:{mark.column + 1}"
            )
        mapping[key] = loader.construct_object(value_node, deep=deep)
    return mapping


UniqueKeyLoader.add_constructor(
    yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG,
    _construct_unique_mapping,
)


def load_yaml(path: Path):
    print(f"{path}")
    with path.open("r") as f:
        return yaml.load(f, Loader=UniqueKeyLoader)


def merge_yaml(a, b, path=""):
    """
    Recursively merge b into a.
    - dict + dict -> merged
    Configuration namespaces may be spread over several files, but the same
    concrete definition may not be supplied twice.
    """
    if isinstance(a, dict) and isinstance(b, dict):
        result = dict(a)
        for key, b_val in b.items():
            if key in result:
                child_path = f"{path}.{key}" if path else str(key)
                if not isinstance(result[key], dict) or not isinstance(b_val, dict):
                    raise ValueError(f"duplicate configuration definition '{child_path}'")
                result[key] = merge_yaml(result[key], b_val, child_path)
            else:
                result[key] = b_val
        return result

    raise ValueError(f"duplicate configuration definition '{path}'")


def load_yaml_multi(paths: list[Path]):
    merged = {}
    for path in paths:
        data = load_yaml(path)
        if data is None:
            continue
        merged = merge_yaml(merged, data)
    return merged


def load_text(path: Path):
    print(f"{path}")
    with open(path, "r") as file:
        return file.read()
    return None
