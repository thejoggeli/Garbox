from common.util import print_json
from common.str_filters import to_camel_case

class Counters:

    def __init__(self):
        self.counters = {}

    def get_next(self, name):
        if(name not in self.counters):
            self.counters[name] = 0
        else:
            self.counters[name] += 1
        return self.counters[name]


def parse_guis(xml_config):

    guis_config_out = {}

    for xml_name, xml_data in xml_config.items():
        guis_config_out[xml_name] = parse_gui(xml_name, xml_data)

    return guis_config_out

def parse_gui(xml_name, xml_data):

    gui_node = xml_data.getroot()

    components_node = gui_node.find("components")
    components = parse_components(components_node)
    
    body_node = gui_node.find("body")
    if body_node is None:
        raise ValueError("main 'gui' > 'body' not found")
    
    body_node.set("name", "body")
    body_objects = parse_object_tree(body_node, components)

    return {
        "components": components,
        "objects": body_objects,
    }


def parse_components(comps_node):

    components = {}

    if comps_node is None:
        return components

    for comp_node in comps_node.findall("*"):
        components[comp_node.tag] = parse_component(comp_node)

    return components


def parse_component(comp_node):
    name = comp_node.get("name")
    print("Parsing component:", name)

    body_node = comp_node.find("body")
    if body_node is None:
        raise ValueError("body element is missing in component")
    
    params = {}
    for param_node in comp_node.findall("param"):
        param_attrs = dict(param_node.attrib)
        if "name" not in param_attrs:
            raise ValueError("param is missing attribute 'name'")
        params[param_attrs.get("name")] = {
            "default": param_attrs.get("default", None)
        }

    body_node.set("name", "body")
    objects = parse_object_tree(body_node, components={})

    return {
        "params": params,
        "objects": objects,
    }


def parse_object_tree(root_node, components):

    counters = Counters()
    objects = {}
    
    # recursively iterate over all nodes in root_node
    # includes root_node itself
    # skips comments
    for node in root_node.iterfind(".//*"): 
        attrs = dict(node.attrib)

        # get name
        name = attrs.get("name", None)
        if name is None:
            name = f"nameless-{node.tag}-{counters.get_next(node.tag)}"
        elif name in objects:
            raise ValueError(f"duplicate name '{name}'")
        
        # convert 'to-camel-case' = 'toCamelCase'
        name = to_camel_case(name, "-")
        
        # remember name for child nodes
        node.set("name", name)

        # get parent name
        parent_name = None
        if node != root_node:

            parent = node.getparent()
            if parent is None:
                raise RuntimeError("unexpected parent None")
            
            parent_name = parent.attrib.get("name", None)
            if parent_name is None:
                raise RuntimeError("parent name must not be None")
            
        # check if node is component
        is_component = node.tag in components
    
        # create final object
        obj_data = {
            "type": node.tag, # either LvObject or component name
            "name": name,
            "attrs": {},
            "parent_name": parent_name,
            "is_component": is_component,
        }

        # remove name from attrs
        if "name" in attrs:
            del attrs["name"]
        
        # parse attrs
        for key, value in attrs.items():
            value = value.strip()
            obj_data["attrs"][key] = value

        # add object to objects
        objects[name] = obj_data


    return objects
