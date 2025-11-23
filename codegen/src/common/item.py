from sortedcontainers import SortedSet
from pathlib import Path
from common.template import render_template
from common.context import Context
from common.util import extract_dict, ensure_list

class Item:
    """
    An item represents a single file (.h or .cpp) that is about to be generated.
    The item class holds all the required information,:
    """
    
    # config dict (loaded from yaml file) containing the key/value pairs expected by the given template
    config:    dict

    # the keys that shall be passed to the template. Passing "*" passes all keys (might lead to unnecessary copying)
    keys: list[str]

    # output path of the generated file
    out_path:       Path

    # path to the template that shall be used  
    template_path:  str

    def __init__(
            self, 
            config: dict,
            keys: str | list[str],
            out_path: str | Path, 
            template_path: str | Path
        ):
        self.config = config
        if(keys == "*"):
            self.keys = list(config.keys())
        else:
            self.keys = ensure_list(keys)
        self.out_path = Path(out_path)
        self.template_path  = str(template_path)


def generate_items(ctx: Context, items : list[Item]):
    """
    Generate the output files for each Item in 'items'.
    - Extracts the subset of config based on item.keys
    - Removes include_h/include_cpp and adds a merged 'include' list
    - Renders templates via Jinja
    """

    for item in items:
        config = extract_dict(item.config, item.keys, copy=True)
        out_path = item.out_path
        template_path = item.template_path

        # Collect includes based on file extension
        includes = extract_includes(config, out_path.suffix)
        if includes:
            config["include"] = includes

        # Render via Jinja
        render_template(ctx.jinja_env, template_path, out_path, config)


def extract_includes(config: dict, extension: str, remove: bool = True):
    """
    Collects include_h or include_cpp entries from all config entries.
    Returns a sorted, deduplicated list.
    Optionally removes include_h/include_cpp fields from config.
    """

    if extension == ".h":
        include_key = "include_h"
    elif extension == ".cpp":
        include_key = "include_cpp"
    else:
        include_key = None

    includes_list = []
    if include_key is not None:
        includes_set = SortedSet()
        for config_entry in config.values():
            if include_key in config_entry:
                include_vals = ensure_list(config_entry[include_key])
                for include in include_vals:
                    includes_set.add(include)
        includes_list = list(includes_set)

    if remove:
        for config_entry in config.values():
            if "include_h" in config_entry:
                del config_entry["include_h"]
            if "include_cpp" in config_entry:
                del config_entry["include_cpp"]

    return includes_list
