from common.util import print_json

def make_resource_name(gui_name, resource_type, value):
    return gui_name + "_" + resource_type + "_" + value.replace("-", "_").replace(" ", "_")

def build_resources(gui_name, gui_data):
    
    used_resources = parse_used_resources(gui_name, gui_data)
    gui_data["resources"]["used"] = used_resources
    
    build_declarations(gui_data)
    
    return None


def build_declarations(gui_data):

    image_cmds = []

    for img_name, img_data in gui_data["resources"]["used"]["images"].items():
        image_cmds.append(f"LV_IMAGE_DECLARE({img_name})")

    gui_data["resources"]["declare"] = {
        "images": image_cmds
    }


def parse_used_resources(gui_name: str, gui_data):
    
    result = {
        "fonts": {},
        "images": {},
    }

    def add_to_result(key, attr_data):

        name = attr_data["value"]

        if name not in gui_data["resources"][key]:
            raise ValueError(f"resource is not defined: '{name}'")

        if name in result[key]:
            return
    
        result[key][name] = gui_data["resources"][key][name]

    for obj_name, obj_data in gui_data["objects_flat"].items():

        obj_type = obj_data["type"]
        attrs = obj_data["attrs"]

        # image src
        for attr_name, attr_data in attrs.items():

            if attr_name == "src" and obj_type == "image":
                attr_data["value"] = make_resource_name(gui_name, "image", attr_data["value"])
                add_to_result("images", attr_data)

            elif attr_name == "font":
                attr_data["value"] = make_resource_name(gui_name, "font", attr_data["value"])
                add_to_result("fonts", attr_data)

    return result