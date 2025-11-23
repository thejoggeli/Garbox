import copy
from pathlib import Path
from common.util import ensure_list
from common.includes import extract_includes
from common.template import render_template
from common.context import Context


class Item:

    yaml_config:    dict
    yaml_keys:      list[str]
    out_path:       Path
    template_path:  str

    def __init__(
            self, 
            yaml_config: dict,
            yaml_keys: str | list[str], 
            out_path: str | Path, 
            template_path: str | Path
        ):
        self.yaml_config    = yaml_config
        self.yaml_keys      = ensure_list(yaml_keys)
        self.out_path       = Path(out_path)
        self.template_path  = str(template_path)


def generate_items(ctx: Context, items : list[Item]):
    """
    Generate the output files for each Item in 'items'.

    - Extracts the subset of YAML based on item.yaml_keys
    - Removes include_h/include_cpp and adds a merged 'include' list
    - Renders templates via Jinja
    """

    for item in items:
        yaml_config = item.yaml_config
        out_path = item.out_path
        template_path = item.template_path

        # Build a dedicated config for this item
        # {'a', 'b'} => {'a': yaml_config['a'], 'b': yaml_config['b']}
        config_copy = copy.deepcopy({
            key: yaml_config[key]
            for key in item.yaml_keys
        })

        # Collect includes based on file extension
        includes = extract_includes(config_copy, out_path.suffix)
        if includes:
            config_copy["include"] = includes

        # Render via Jinja
        render_template(ctx.jinja_env, template_path, out_path, **config_copy)
