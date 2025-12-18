from pathlib import Path
from common.util import print_json, print_section, print_sep
from common.context import Context
from loader.loader import Loader
from generate.gen_runtime import generate_runtime
from generate.gen_hardware import generate_hardware
from generate.gen_shared import generate_shared
from generate.gen_states import generate_states
from generate.gen_components import generate_components
from generate.gen_config import generate_config
from generate.gen_guis import generate_guis


def main():
    
    # base dir path
    root_dir = (Path(__file__).parent / "../..").resolve()
    codegen_dir = (root_dir / "codegen").relative_to(root_dir)

    # create generator context 
    ctx = Context(
        root_dir      = root_dir,
        config_dir    = (codegen_dir / "config"),
        output_dir    = (codegen_dir / "output"),
        stubs_dir     = (codegen_dir / "output/stubs"),
        templates_dir = (codegen_dir / "templates"),
        res_dir       = (root_dir    / "res"),
        app_dir       = (root_dir    / "src/app"),
        gen_dir       = (root_dir    / "src/app_generated"),
        bin_dir       = (root_dir    / "src/bin"),
        shared_dir    = (root_dir    / "src/shared")
    )
    print_section("generator will use the following directory paths")
    ctx.print()

    # create loader
    print_section("loading all config files")
    loader = Loader(ctx)
    loader.preload_all()

    # safe parsed config to json
    print_section("saving all processed files")
    loader.save_config(ctx.output_dir / "parsed")
    
    # hardware generation
    print_section("generating hardware files")
    generate_hardware(ctx, loader)

    # shared generation
    print_section("generating shared files")
    generate_shared(ctx, loader)

    # states generation
    print_section("generating state files")
    generate_states(ctx, loader)

    # components generation
    print_section("generating component files")
    generate_components(ctx, loader)

    # runtime generation
    print_section("generating runtime files")
    generate_runtime(ctx, loader)

    # config generation
    print_section("generating config files")
    generate_config(ctx, loader)

    # guis
    print_section("generating gui files")
    generate_guis(ctx, loader)

    # generation finished
    print_section("all files generated successfully!")

if __name__ == "__main__":
    main()
