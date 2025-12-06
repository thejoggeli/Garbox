from pathlib import Path
from common.template import create_jinja_env

class Context:

    # path members
    root_dir:       Path
    config_dir:     Path
    templates_dir:  Path
    app_dir:        Path
    shared_dir:     Path
    stubs_dir:      Path

    # jinja env member
    jina_env = None

    def __init__(self, root_dir, config_dir, templates_dir, app_dir, shared_dir, stubs_dir):

        # set paths
        self.root_dir       = Path(root_dir).resolve()
        self.config_dir     = Path(config_dir).resolve().relative_to(root_dir)
        self.templates_dir  = Path(templates_dir).resolve().relative_to(root_dir)
        self.app_dir        = Path(app_dir).resolve().relative_to(root_dir)
        self.shared_dir     = Path(shared_dir).resolve().relative_to(root_dir)
        self.stubs_dir      = Path(stubs_dir).resolve().relative_to(root_dir)

        # create jinja environment
        self.jinja_env = create_jinja_env(template_dir=self.templates_dir)
    
    def print(self):
        print(f"config     => {self.config_dir}")
        print(f"templates  => {self.templates_dir}")
        print(f"app        => {self.app_dir}")
        print(f"shared_dir => {self.shared_dir}")
        print(f"stubs_dir  => {self.stubs_dir}")