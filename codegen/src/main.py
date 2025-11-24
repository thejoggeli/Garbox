from pathlib import Path
from common.context import Context
from common.loader import Loader
from generate.gen_runtime import generate_runtime
from generate.gen_hardware import generate_hardware
from generate.gen_types import generate_types
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
    print(f"generator will use the following directory paths")
    ctx.print()

    # create loader
    print("loading all config files")
    loader = Loader(ctx.config_dir)
    loader.preload_all()
    
    # hardware generation
    print("generating hardware files")
    generate_hardware(ctx, loader)

    # events generation
    print("generating types files")
    generate_types(ctx, loader)

    # components generation
    print("generating component files")
    generate_components(ctx, loader)

    # runtime generation
    print("generating runtime files")
    generate_runtime(ctx, loader)

    # generation finished
    print("all files generated successfully")

if __name__ == "__main__":
    main()
