from pathlib import Path
from jinja2 import Environment, FileSystemLoader, StrictUndefined
from common.str_filters import lower_first, upper_first, ljust, rjust, ensure_suffix
import re


def create_jinja_env(template_dir: Path):
    env = Environment(
        loader=FileSystemLoader(template_dir),
        trim_blocks=True,
        lstrip_blocks=True,
        undefined=StrictUndefined,
    )
    env.filters["upper_first"] = upper_first
    env.filters["lower_first"] = lower_first
    env.filters["ljust"] = ljust
    env.filters["rjust"] = rjust
    env.filters["ensure_suffix"] = ensure_suffix
    return env


def render_template(jinja_env, template_name: str, output_path: Path, context: dict):
    template = jinja_env.get_template(template_name)
    result_text = template.render(**context)

    # collapse 2+ blank lines into exactly one
    result_text = re.sub(r'\n{2,}', '\n\n', result_text)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(result_text)

    print(f"{output_path}")
