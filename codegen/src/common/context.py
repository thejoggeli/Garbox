from pathlib import Path
from common.template import create_jinja_env

class Context:

    # path members
    config_dir:     Path
    templates_dir:  Path
    app_dir:        Path
    shared_dir:     Path
    stubs_dir:      Path

    # jinja env member
    jina_env = None

    def __init__(self, config_dir, templates_dir, app_dir, shared_dir, stubs_dir):

        # set paths
        self.config_dir     = Path(config_dir).resolve()
        self.templates_dir  = Path(templates_dir).resolve()
        self.app_dir        = Path(app_dir).resolve()
        self.shared_dir     = Path(shared_dir).resolve()
        self.stubs_dir      = Path(stubs_dir).resolve()

        # create jinja environment
        self.jinja_env = create_jinja_env(template_dir=self.templates_dir)
    
    def print(self):
        print(f"config ----- {self.config_dir}")
        print(f"templates -- {self.templates_dir}")
        print(f"app -------- {self.app_dir}")
        print(f"shared_dir - {self.shared_dir}")
        print(f"stubs_dir -- {self.stubs_dir}")