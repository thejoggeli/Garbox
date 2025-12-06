from common.str_filters import to_camel_case
from lxml import etree
from pathlib import Path
from loader.gui.resources import make_resource_name

class Counters:

    def __init__(self):
        self.counters = {}

    def get_next(self, name):
        if(name not in self.counters):
            self.counters[name] = 0
        else:
            self.counters[name] += 1
        return self.counters[name]
    

def xml_to_dict(gui_name: str, xml_text: str):

    # <gui> node
    gui_node = etree.fromstring(xml_text)

    # <resources> node
    resources_node = gui_node.find("resources")
    resources = parse_resources(gui_name, resources_node)

    # <components> node
    components_node = gui_node.find("components")
    components = parse_components(components_node)
    
    # <screen> node
    screen_node = gui_node.find("root")
    if screen_node is None:
        raise ValueError("required node not found: gui > root")
    
    screen_node.set("name", "root")
    root_objects = parse_object_tree(screen_node, components=components)

    return {
        "name": gui_name,
        "resources": resources,
        "components": components,
        "objects_raw": root_objects,
    }

def parse_resources(gui_name: str, resources_node):

    resources = {
        "images": {},
        "fonts": {},
    }

    if resources_node is None:
        return resources
    
    for res_node in resources_node.findall("*"):

        dict_key = f"{res_node.tag}s"
        
        if dict_key not in resources:
            raise ValueError(f"invalid resource tag type: {res_node.tag}")
        
        name = res_node.get("name", None)
        if name is None:
            raise ValueError(f"resource tag '{res_node.tag}' requires attribute 'name'")

        if name in resources:
            raise ValueError(f"duplicate resource name type='{res_node.tag}' name='{name}'")
        
        attrs = dict(res_node.attrib)
        attrs["original_name"] = name

        resource_name = make_resource_name(gui_name, res_node.tag, name)
        attrs["name"] = resource_name

        resources[dict_key][resource_name] = attrs

    return resources


def parse_components(comps_node):

    components = {}

    if comps_node is None:
        return components

    for comp_node in comps_node.findall("*"):
        components[comp_node.tag] = parse_component(comp_node)

    return components


def parse_component(comp_node):

    body_node = comp_node.find("body")
    if body_node is None:
        raise ValueError("body element is missing in component")
    
    params = {}

    # scan param attributes ($attr => o-attr / r-attr)
    for param_name, param_value in dict(comp_node.attrib).items():

        # o-attr => optional
        if(param_name.startswith("r-")):
            name = param_name[2:]
            required = True
            default = None
        # r-attr => required
        elif(param_name.startswith("o-")):
            name = param_name[2:]
            required = False
            default = param_value
        # invalid prefix
        else:
            raise ValueError(f"invalid param prefix: {param_name}")

        # param names must be unique
        if name in params:
            raise KeyError(f"duplicated param name '{name}'")

        # add param config
        params[name] = {
            "name": name,
            "default": default,
            "required": required,
        }

    # scan <param> tags
    for param_node in comp_node.findall("param"):
        param_attrs = dict(param_node.attrib)
        if "name" not in param_attrs:
            raise ValueError("param is missing attribute 'name'")
        
        # param names must be unique
        name = param_attrs.get("name")
        if name in params:
            raise KeyError(f"duplicated param name '{name}'")

        # add param config
        params[name] = {
            "name": name,
            "default": param_attrs.get("default", None),
            "required": "default" not in param_attrs,
        }

    body_node.set("name", "body")
    objects = parse_object_tree(body_node)

    return {
        "params": params,
        "objects": objects,
    }


def parse_object_tree(root_node, components={}):

    counters = Counters()
    objects = {}
    
    def process_node(node):

        attrs = dict(node.attrib)

        # get name
        name = attrs.get("name", None)

        # handle name attr to found
        if name is None:
            name = f"_nameless-{node.tag}_{counters.get_next(node.tag)}"

        # handle duplicate name
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
    
        # get component
        isComponent = node.tag in components
        component = components[node.tag] if isComponent else None

        # create final object
        obj_data = {
            "type": node.tag, # either LvObject or component name
            "name": name,
            "attrs": {},
            "parent_name": parent_name,
            "is_component": isComponent,
            "component": component, 
        }

        # remove name from attrs
        if "name" in attrs:
            del attrs["name"]
        
        # parse component attrs => 
        for key, value in attrs.items():
            value = value.strip()

            # handle v-attrs
            if key.startswith("v-"):
                obj_data["attrs"][key[2:]] = {
                    "type": "v-attr",
                    "value": value,
                    "value_raw": value,
                }
            # handle p-attrs
            elif key.startswith("p-"):
                obj_data["attrs"][key[2:]] = {
                    "type": "p-attr",
                    "value": value,
                    "value_raw": value,
                }
            # handle regular attribute
            else:
                obj_data["attrs"][key] = {
                    "type": "attr",
                    "value": value,
                    "value_raw": value,
                }

        # add object to objects
        objects[name] = obj_data

    # process root node first
    process_node(root_node)

    # recursively iterate over all nodes in root_node
    for node in root_node.iterfind(".//*"): 
        process_node(node)

    return objects


