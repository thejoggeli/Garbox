from pathlib import Path
from common.context import Context
from common.loader import load_hardware_config, load_events_config, load_application_config
from generate.gen_runtime import generate_runtime
from generate.gen_hardware import generate_hardware
from generate.gen_events import generate_events
from generate.gen_components import generate_components


def main():
    
    # base dir path
    script_dir = Path(__file__).parent
    codegen_dir = (script_dir / "..").resolve()

    # create generator context 
    ctx = Context(
        config_dir    = (codegen_dir / "config").resolve(),
        templates_dir = (codegen_dir / "templates").resolve(),
        stubs_dir     = (codegen_dir / "stubs").resolve(),
        app_dir       = (codegen_dir / "../src/app").resolve(),
        shared_dir    = (codegen_dir / "../src/shared").resolve()
    )
    ctx.print()
    
    # hardware generation
    hardware_config = load_hardware_config(ctx.config_dir)
    generate_hardware(ctx, hardware_config)

    # types generation
    events_config = load_events_config(ctx.config_dir)
    generate_events(ctx, events_config)

    # components generation
    app_config = load_application_config(ctx.config_dir)
    generate_components(ctx, app_config)

    # runtime generation
    generate_runtime(ctx, app_config, events_config)


if __name__ == "__main__":
    main()
