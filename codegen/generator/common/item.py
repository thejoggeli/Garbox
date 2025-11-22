from pathlib import Path
from common.util import ensure_list

class Item:
    def __init__(self, yaml_config, yaml_keys, out_path: str | Path, template_path: str):
        self.yaml_config = yaml_config
        self.yaml_keys = ensure_list(yaml_keys)
        self.out_path = Path(out_path)
        self.template_path = template_path
