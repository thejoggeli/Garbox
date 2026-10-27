from pathlib import Path
from common.template import create_jinja_env

class Context:

    # path members
    root_dir:       Path
    config_dir:     Path
    output_dir:     Path
    templates_dir:  Path
    app_dir:        Path
    gen_dir:        Path
    bin_dir:        Path
    res_dir:        Path
    shared_dir:     Path
    stubs_dir:      Path

    # jinja env member
    jina_env = None

    def __init__(self, 
        root_dir, 
        config_dir, 
        output_dir, 
        templates_dir, 
        res_dir, 
        app_dir, 
        gen_dir, 
        bin_dir, 
        shared_dir, 
        stubs_dir):

        # Keep paths absolute. Relative paths accidentally made generation
        # depend on the caller's current working directory.
        self.root_dir       = Path(root_dir).resolve()
        self.config_dir     = Path(config_dir).resolve()
        self.output_dir     = Path(output_dir).resolve()
        self.templates_dir  = Path(templates_dir).resolve()
        self.app_dir        = Path(app_dir).resolve()
        self.gen_dir        = Path(gen_dir).resolve()
        self.bin_dir        = Path(bin_dir).resolve()
        self.res_dir        = Path(res_dir).resolve()
        self.shared_dir     = Path(shared_dir).resolve()
        self.stubs_dir      = Path(stubs_dir).resolve()

        # create jinja environment
        self.jinja_env = create_jinja_env(template_dir=self.templates_dir)
    
    def print(self):
        print(f"root       => {self.root_dir}")
        print(f"config     => {self._display(self.config_dir)}")
        print(f"output     => {self._display(self.output_dir)}")
        print(f"templates  => {self._display(self.templates_dir)}")
        print(f"bin        => {self._display(self.bin_dir)}")
        print(f"app        => {self._display(self.app_dir)}")
        print(f"gen        => {self._display(self.gen_dir)}")
        print(f"res        => {self._display(self.res_dir)}")
        print(f"shared_dir => {self._display(self.shared_dir)}")
        print(f"stubs_dir  => {self._display(self.stubs_dir)}")

    def _display(self, path: Path):
        try:
            return path.relative_to(self.root_dir)
        except ValueError:
            return path
