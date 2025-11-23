from pathlib import Path
from jinja2 import Environment, FileSystemLoader, StrictUndefined


def create_jinja_env(template_dir: Path):
    return Environment(
        loader=FileSystemLoader(template_dir),
        trim_blocks=True,
        lstrip_blocks=True,
        undefined=StrictUndefined,
    )


def render_template(jinja_env, template_name: str, output_path: Path, **context):
    template = jinja_env.get_template(template_name)
    result = template.render(**context)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(result)

    print(f"{output_path}")
