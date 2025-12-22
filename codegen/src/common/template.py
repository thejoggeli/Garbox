from pathlib import Path
from jinja2 import Environment, FileSystemLoader, StrictUndefined
from common.gen_section import GenSection
from common.str_filters import lower_first, upper_first, ljust, rjust, ensure_suffix, sentinel
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
    env.filters["sentinel"] = sentinel
    return env


def render_template(jinja_env,
    template_name: str,
    output_path: Path,
    context: dict,
    sections: list[GenSection] | None = None,
):

    print(output_path)
    
    template = jinja_env.get_template(template_name)
    result_text = template.render(**context)

    # insert sections
    if sections:
        for section in sections:
            section_template = jinja_env.get_template(section.template_path)
            section_text = section_template.render(**section.config)
            result_text = section.apply(result_text, section_text)

    # collapse 2+ blank lines into exactly one
    result_text = re.sub(r"\n{2,}", "\n\n", result_text)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(result_text)


def render_sections_to_existing_file(
    jinja_env,
    input_path: Path,
    sections: list[GenSection],
):

    print(input_path)

    if not input_path.exists():
        raise FileNotFoundError(input_path)

    result_text = input_path.read_text()

    # insert sections
    for section in sections:
        section_template = jinja_env.get_template(section.template_path)
        section_text = section_template.render(**section.config)
        result_text = section.apply(result_text, section_text)

    # collapse 2+ blank lines into exactly one
    result_text = re.sub(r"\n{2,}", "\n\n", result_text)

    input_path.write_text(result_text)
