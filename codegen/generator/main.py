from pathlib import Path
from common.generator_paths import GeneratorPaths
from common.template import create_jinja_env
from common.loader import ( 
    load_hardware_config,
    load_events_config,
    load_application_config
)
from common.util import print_json
from generators.generate_hardware import generate_hardware
from generators.generate_events import generate_events
from generators.generate_components import (
    generate_components_ids,
    generate_controllers,
    generate_behaviours
)
from generators.generate_runtime import generate_runtime

# base paths
script_dir = Path(__file__).parent
codegen_dir = (script_dir / "..").resolve()

def main():

    paths = GeneratorPaths(
        config_dir    = (codegen_dir / "config").resolve(),
        templates_dir = (codegen_dir / "templates").resolve(),
        stubs_dir     = (codegen_dir / "stubs").resolve(),
        app_dir       = (codegen_dir / "../src/app").resolve(),
        shared_dir    = (codegen_dir / "../src/shared").resolve()
    )
    paths.print()

    # create jinja environment
    jinja_env = create_jinja_env(template_dir=paths.templates_dir)

    # hardware generation
    hardware_config = load_hardware_config(paths.config_dir)
    generate_hardware(jinja_env, paths, hardware_config)

    # types generation
    events_config = load_events_config(paths.config_dir)
    generate_events(jinja_env, paths, events_config)

    # components generation
    application_config = load_application_config(paths.config_dir)
    generate_components_ids(jinja_env, paths, application_config)
    generate_behaviours(jinja_env, paths, application_config)
    generate_controllers(jinja_env, paths, application_config)

    # runtime generation
    generate_runtime(jinja_env, paths, application_config, events_config)


if __name__ == "__main__":
    main()
