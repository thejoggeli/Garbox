import yaml
import json
import os
from pathlib import Path
from loader.parse_application import parse_application
from loader.parse_events import parse_events
from loader.parse_hardware import parse_hardware_config
from loader.gui.parse_guis import parse_guis
from common.context import Context
from common.util import decycle, print_section

class Loader:

    config_dir : Path
    output_dir : Path

    config: dict = None

    def __init__(self, ctx: Context):
        self.config_dir = ctx.config_dir
        self.output_dir = ctx.output_dir
        pass

    def preload_all(self):

        # load yaml files
        paths = [p for p in self.config_dir.rglob("*.yaml") if p.is_file()]
        self.config = load_yaml_multi(paths)

        # load xml files
        gui_dir = self.config_dir / "gui" 
        gui_paths = [p for p in gui_dir.glob("*.xml") if p.is_file()]
        gui_configs_text = {}
        for path in gui_paths:
            config_text = load_text(path)
            config_name = path.stem
            gui_configs_text[config_name] = config_text 

        print_section("parsing configs")

        # parse hardware config
        parse_hardware_config(self.config)

        # parse events (render default values)
        parse_events(self.config)

        # parse gui xml configs  
        self.config["guis"] = parse_guis(gui_configs_text, self.output_dir / "gui")

        # parse application and components including:
        # - application
        # - controllers
        # - behaviours
        # - screens
        # requires events to already be parsed)
        parse_application(self.config)       


    def save_json(self, path: Path, config):
        print(f"{path}")
        os.makedirs(path.parent, exist_ok=True)
        with open(path, "w") as file:
            json.dump(decycle(config), file, indent=2)


    def save_config(self, path: Path):
        for name in self.config.keys():
            self.save_json(path / f"{name}.json", self.config[name])
            

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


def load_text(path: Path):
    print(f"{path}")
    with open(path, "r") as file:
        return file.read()
    return None
