from sortedcontainers import SortedSet
from pathlib import Path
from common.template import render_template, render_sections_to_existing_file
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

    # allow override
    allow_override: bool

    def __init__(
            self, 
            config: dict,
            out_path: str | Path, 
            template_path: str | Path,
            sections: list[GenSection] = None,
            allow_override: bool = True
        ):
        self.config = config
        self.out_path = Path(out_path)
        self.template_path  = str(template_path)
        self.sections = sections
        self.allow_override = allow_override

def generate_items(ctx: Context, items : list[Item]):
    """
    Generate the output files for each Item in 'items'.
    """

    # render all items via jinja
    for item in items:

        file_exists = Path(item.out_path).exists()
        has_sections = item.sections is not None

        # do not override
        if file_exists and item.allow_override == False:
            continue

        # replace only sections if item has sections and output file already exists
        if has_sections and file_exists:
            render_sections_to_existing_file(
                ctx.jinja_env,
                item.out_path,
                item.sections
            )

        # render entire template from scratch and fully replace current file        
        else:
            render_template(
                ctx.jinja_env, 
                item.template_path, 
                item.out_path, 
                item.config, 
                item.sections
            )
