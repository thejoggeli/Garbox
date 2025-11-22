from pathlib import Path

class GeneratorPaths:
    def __init__(self, config_dir, templates_dir, app_dir, shared_dir, stubs_dir):
        self.config_dir : Path = Path(config_dir).resolve()
        self.templates_dir : Path = Path(templates_dir).resolve()
        self.app_dir : Path = Path(app_dir).resolve()
        self.shared_dir : Path = Path(shared_dir).resolve()
        self.stubs_dir: Path = Path(stubs_dir).resolve()
    
    def print(self):
        print(f"generator will use the following directory paths:")
        print(f"config ----- {self.config_dir}")
        print(f"templates -- {self.templates_dir}")
        print(f"app -------- {self.app_dir}")
        print(f"shared_dir - {self.shared_dir}")
        print(f"stubs_dir -- {self.stubs_dir}")