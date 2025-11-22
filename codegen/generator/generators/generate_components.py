from sortedcontainers import SortedSet
from common.item import Item
from common.generate_items import generate_items
from common.generator_paths import GeneratorPaths


def generate_components_ids(jinja_env, paths: GeneratorPaths, config):
    """
    Generate ComponentId, BehaviourId, ControllerId files.
    """

    all_names = SortedSet()
    all_names.update(config["controllers"])
    all_names.update(config["behaviours"])

    items = []

    # ComponentId
    comp_dict = {"names": list(all_names)}
    items.append(Item(comp_dict, "names", paths.shared_dir / "types/ComponentId.h",   "application/ComponentId.h.j2"))
    items.append(Item(comp_dict, "names", paths.shared_dir / "types/ComponentId.cpp", "application/ComponentId.cpp.j2"))

    # BehaviourId
    items.append(Item(config, "behaviours", paths.shared_dir / "types/BehaviourId.h",   "application/BehaviourId.h.j2"))
    items.append(Item(config, "behaviours", paths.shared_dir / "types/BehaviourId.cpp", "application/BehaviourId.cpp.j2"))

    # ControllerId
    items.append(Item(config, "controllers", paths.shared_dir / "types/ControllerId.h",   "application/ControllerId.h.j2"))
    items.append(Item(config, "controllers", paths.shared_dir / "types/ControllerId.cpp", "application/ControllerId.cpp.j2"))

    generate_items(jinja_env, items)


def generate_controllers(jinja_env, paths: GeneratorPaths, config):
    _generate_components(jinja_env, paths, config["controllers"], "controller", "Controller")

    
def generate_behaviours(jinja_env, paths: GeneratorPaths, config):
    _generate_components(jinja_env, paths, config["behaviours"], "behaviour", "Behaviour")


def _generate_components(jinja_env, paths: GeneratorPaths, config, type_key: str, type_name: str):
    
    # list of items to be generated
    items = []
    stubs = []
    for controller_key, controller_dict in config.items():
        out_path = paths.app_dir /  f"{type_key}s/generated/{controller_key}Abs"
        template_path = f"application/{type_key}/{type_name}Abs"
        item_dict = {
            "name": controller_key,
            f"{type_key}": controller_dict
        }
        controller_dict["name"] = controller_key
        items.append(Item(item_dict, [type_key, "name"], f"{out_path}.h", f"{template_path}.h.j2"))
        items.append(Item(item_dict, [type_key, "name"], f"{out_path}.cpp", f"{template_path}.cpp.j2"))

        out_path = paths.stubs_dir / f"{type_key}s/{controller_key}"
        template_path = f"application/{type_key}/{type_name}Stub"
        stubs.append(Item(item_dict, [type_key, "name"], f"{out_path}.h", f"{template_path}.h.j2"))
        stubs.append(Item(item_dict, [type_key, "name"], f"{out_path}.cpp", f"{template_path}.cpp.j2"))

    # generate all hardware h/cpp files
    generate_items(jinja_env, items)
    generate_items(jinja_env, stubs)