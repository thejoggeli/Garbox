from pathlib import Path
from common.util import print_json, print_section, print_sep
from common.context import Context
from loader.loader import Loader
from generate.gen_runtime import generate_runtime
from generate.gen_hardware import generate_hardware
from generate.gen_types import generate_types
from generate.gen_components import generate_components
from generate.gen_config import generate_config


def main():
    
    # base dir path
    root_dir = (Path(__file__).parent / "../..").resolve()
    codegen_dir = (root_dir / "codegen").relative_to(root_dir)

    # create generator context 
    ctx = Context(
        root_dir      = root_dir,
        config_dir    = (codegen_dir / "config"),
        templates_dir = (codegen_dir / "templates"),
        stubs_dir     = (codegen_dir / "stubs"),
        app_dir       = (codegen_dir / "../src/app"),
        shared_dir    = (codegen_dir / "../src/shared")
    )
    print_section("generator will use the following directory paths")
    ctx.print()

    # create loader
    print_section("loading all config files")
    loader = Loader(ctx.config_dir)
    loader.preload_all()

    # safe parsed config to json
    print_section("saving all processed files")
    loader.save_config((codegen_dir / "config_parsed"))

    quit()
    
    # hardware generation
    print_section("generating hardware files")
    generate_hardware(ctx, loader)

    # events generation
    print_section("generating types files")
    generate_types(ctx, loader)

    # components generation
    print_section("generating component files")
    generate_components(ctx, loader)

    # runtime generation
    print_section("generating runtime files")
    generate_runtime(ctx, loader)

    # config generation
    print_section("generating config files")
    generate_config(ctx, loader)

    # generation finished
    print_sep()
    print("all files generated successfully!")

if __name__ == "__main__":
    main()
