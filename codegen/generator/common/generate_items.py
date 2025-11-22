import copy
from pathlib import Path

from common.includes import extract_includes
from common.template import render_template
from common.item import Item

def generate_items(jinja_env, items : list[Item]):
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
        render_template(jinja_env, template_path, out_path, **config_copy)
