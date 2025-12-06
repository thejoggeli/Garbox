from common.str_filters import to_camel_case
from lxml import etree
from pathlib import Path
from loader.gui.preprocess_xmls import preprocess_xml_text
from lxml import etree


class Counters:

    def __init__(self):
        self.counters = {}

    def get_next(self, name):
        if(name not in self.counters):
            self.counters[name] = 0
        else:
            self.counters[name] += 1
        return self.counters[name]
    

def xml_to_dict(xml_text):

    gui_node = etree.fromstring(xml_text)

    components_node = gui_node.find("components")
    components = parse_components(components_node)
    
    root_node = gui_node.find("root")
    if root_node is None:
        raise ValueError("required node not found: gui > root")
    
    root_node.set("name", "root")
    root_objects = parse_object_tree(root_node, components=components)

    return {
        "components": components,
        "objects": root_objects,
    }


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
    
    # recursively iterate over all nodes in root_node
    # includes root_node itself
    # skips comments
    for node in root_node.iterfind(".//*"): 
        attrs = dict(node.attrib)

        # get name
        name = attrs.get("name", None)
        if name is None:
            name = f"_nameless-{node.tag}_{counters.get_next(node.tag)}"
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
                }
            # handle p-attrs
            elif key.startswith("p-"):
                obj_data["attrs"][key[2:]] = {
                    "type": "p-attr",
                    "value": value,
                }
            # handle regular attribute
            else:
                obj_data["attrs"][key] = {
                    "type": "attr",
                    "value": value,
                }

        # add object to objects
        objects[name] = obj_data


    return objects


