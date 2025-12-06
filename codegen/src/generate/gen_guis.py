from common.context import Context
from common.item import Item, generate_items
from common.str_filters import upper_first
from loader.gui.image_load import load_lvgl_image
from loader.loader import Loader
from sortedcontainers import SortedSet

def generate_guis(ctx: Context, loader: Loader):

    _generate_classes(ctx, loader)
    _generate_resources(ctx, loader)


def make_attribute_macro(c_filename):
    name = c_filename.upper()
    name = name.replace("-", "_")
    return f"LV_ATTRIBUTE_IMAGE_{name}"

def get_class_name(obj_type):

    to_plain_obj = {"container", "root", "body"}

    if obj_type in to_plain_obj:
        return "LvObject"
    
    return "Lv" + upper_first(obj_type)


def _extract_used_object_types(gui_data):

    obj_types = SortedSet()

    for comp_data in gui_data["components"].values():
        for obj_data in comp_data["objects"].values():
            obj_types.add(obj_data["type"])

    for obj_data in gui_data["objects_flat"].values():
        obj_types.add(obj_data["type"])

    return list(obj_types)


def _type_to_class_name_map(gui_data):

    obj_types = _extract_used_object_types(gui_data)
    map = {}

    for obj_type in obj_types:

        if obj_type not in map:
            map[obj_type] = {
                "type": obj_type,
                "class_name": get_class_name(obj_type)
            }

    return map

def _extract_include_types(gui_data):

    obj_types = _extract_used_object_types(gui_data)
    map = {}

    for obj_type in obj_types:
        class_name = get_class_name(obj_type)
        if class_name not in map:
            map[class_name] = {
                "type": obj_type,
                "class_name": class_name,
            }

    return map



def _generate_classes(ctx: Context, loader: Loader):

    items = []

    for gui_name, gui_data in loader.config["guis"].items():

        config = {
            "name": gui_name,
            "gui": gui_data,
            "include_types": _extract_include_types(gui_data),
            "type_to_class_name": _type_to_class_name_map(gui_data),
        }

        out_path_h   = ctx.app_dir/f"guis/{gui_name}Gui.h" 
        out_path_cpp = ctx.app_dir/f"guis/{gui_name}Gui.cpp" 

        items.append(Item(config, "*", out_path_h,   "gui/gui.h.j2"))
        items.append(Item(config, "*", out_path_cpp, "gui/gui.cpp.j2"))

    generate_items(ctx, items)


def _generate_resources(ctx: Context, loader: Loader):

    items = []

    for gui_data in loader.config["guis"].values():

        all_used = gui_data["resources"]["used"]

        for image_name, image_resource in all_used["images"].items():

            image = load_lvgl_image(
                path = ctx.res_dir / image_resource["path"],
                name = image_name,
                color_format = image_resource["format"],
            )

            config = {
                "attribute_macro": make_attribute_macro(image_name),
                "image": image,
                "resource": image_resource,
            }

            items.append(Item(
                config, keys = "*", 
                out_path = ctx.bin_dir/"images"/f"{image_name}.c", 
                template_path="gui/resources/image.j2"
            ))

    generate_items(ctx, items)
