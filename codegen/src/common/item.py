from sortedcontainers import SortedSet
from pathlib import Path
from common.template import render_template
from common.context import Context
from common.util import extract_dict, ensure_list
from common.gen_section import GenSection

class Item:
    """
    An item represents a single file (.h or .cpp) that is about to be generated.
    The item class holds all the required information,:
    """
    
    # config dict (loaded from yaml file) containing the key/value pairs expected by the given template
    config:    dict

    # output path of the generated file
    out_path:       Path

    # path to the template that shall be used  
    template_path:  str

    # sections <gen:begin section='x'>/<gen:end section='x'> to be applied
    sections: list[GenSection]

    def __init__(
            self, 
            config: dict,
            out_path: str | Path, 
            template_path: str | Path,
            sections: list[GenSection] = None
        ):
        self.config = config
        self.out_path = Path(out_path)
        self.template_path  = str(template_path)
        self.sections = sections


def generate_items(ctx: Context, items : list[Item]):
    """
    Generate the output files for each Item in 'items'.
    """

    # render all items via jinja
    for item in items:

        render_template(
            ctx.jinja_env, 
            item.template_path, 
            item.out_path, 
            item.config, 
            item.sections
        )
