def resolve_components(gui_data: dict):

    objects_flat = {}

    for obj_name, obj_data in gui_data["objects_raw"].items():

        is_component = obj_data["is_component"]

        # resolve component into objects
        if is_component:
            comp_objects = resolve_component(obj_data)

            # add all objects from component to flat objects list
            for comp_obj_name, comp_obj_data in comp_objects.items():
                if comp_obj_name in objects_flat:
                    raise KeyError(f"duplicate full object identifier: '{comp_obj_name}'")
                objects_flat[comp_obj_name] = comp_obj_data

        # handle regular lvgl object
        else:
            objects_flat[obj_name] = obj_data

    return objects_flat


def resolve_component(obj_data):

    obj_type = obj_data["type"]
    comp_def = obj_data["component"]

    attrs_for_body  = {k: v for k, v in obj_data["attrs"].items()    if v["type"] == "attr"}
    param_attrs     = {k: v for k, v in obj_data["attrs"].items()    if v["type"] == "p-attr"}
    optional_params = {k: v for k, v in comp_def["params"].items()   if v["required"] == False}

    objects = {}

    for objdef_name, objdef_data in comp_def["objects"].items():

        attrs = {}

        for attr_name, attr_data in objdef_data["attrs"].items():
            out_value = None
            out_value_raw = attr_data["value_raw"] 
            value = attr_data["value"] 

            # resolve v-attr (insert value either from passed params or from default)
            if attr_data["type"] == "v-attr":
                
                # get value from passed param
                if value in param_attrs:
                    out_value = param_attrs[value]["value"]
                    out_value_raw = param_attrs[value]["value_raw"]

                # fall back to default value
                elif value in optional_params: 
                    out_value = optional_params[value]["default"]
                    out_value_raw = optional_params[value]["default"]

                # handle param not found
                else:
                    raise ValueError(f"parameter attribute missing: {obj_type}.{objdef_name}.{attr_name}")
            
            # handle regular attr 
            else:
                out_value = attr_data["value"]

            attrs[attr_name] = {
                "type": attr_data["type"],
                "value": out_value,
                "value_raw": out_value_raw,
            }
        
        full_name = f"{obj_data['name']}.{objdef_data['name']}"

        if full_name in objects:
            raise KeyError(f"duplicate full object identifier: '{full_name}'")

        objects[full_name] = {
            "type": objdef_data["type"],
            "name": full_name,
            "attrs": attrs,
        }

    # override default component body attrs with passed component instance attrs
    body_idx = f"{obj_data['name']}.body"
    body_attrs = objects[body_idx]["attrs"]
    for attr_name, attr_data in attrs_for_body.items():
        body_attrs[attr_name] = {
            "type": attr_data["type"],
            "value": attr_data["value"],
            "value_raw": attr_data["value_raw"],
        }

    return objects