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

        out_path = item.out_path
        template_path = item.template_path

        # extract and clone relevant data for current item into new dict
        config = extract_dict(item.config, item.keys)

        # Render via Jinja
        render_template(ctx.jinja_env, template_path, out_path, config)
