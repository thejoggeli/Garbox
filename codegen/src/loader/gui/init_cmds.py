from common.util import print_json
from loader.gui.init_cmds_fns import eval_initializer
from loader.gui.init_cmds_attrs import process_attrs


def build_initializer_commands(gui_data):

    fn_calls = []
    for obj_name, obj_data in gui_data["objects"].items():

        is_component = obj_data["is_component"]

        # process attrs
        process_attrs(obj_data["attrs"])

        # handle component object
        if is_component:
            result = build_from_component(obj_data)

        # handle regular lvgl object
        else:
            result = build_from_object(obj_data)

        fn_calls.extend(result)

    # assemble calls
    assembled_calls = []
    for fn_call in fn_calls:
        instance = fn_call["instance"]
        for call_str in fn_call["calls"]:
            assembled_call = f"{instance}.{call_str}; // {fn_call['comment']};"
            # print(assembled_call)
            assembled_calls.append(assembled_call)

    return assembled_calls

def build_from_object(obj_data):
    fn_calls = build_function_calls(obj_data["name"], obj_data["type"], obj_data["attrs"])
    return fn_calls


def build_from_component(obj_data):

    obj_type = obj_data["type"]
    comp_def = obj_data["component"]

    body_attrs      = {k: v for k, v in obj_data["attrs"].items()    if v["type"] == "attr"}
    param_attrs     = {k: v for k, v in obj_data["attrs"].items()    if v["type"] == "p-attr"}
    optional_params = {k: v for k, v in comp_def["params"].items()   if v["required"] == False}

    build_list = []

    for objdef_name, objdef_data in comp_def["objects"].items():

        attrs = {}

        for attr_name, attr_data in objdef_data["attrs"].items():
            out_value = None
            raw_value = attr_data["value"] 

            # resolve v-attr (insert value either from passed params or from default)
            if attr_data["type"] == "v-attr":
                
                # get value from passed param
                if raw_value in param_attrs:
                    out_value = param_attrs[raw_value]["value"]

                # fall back to default value
                elif raw_value in optional_params: 
                    out_value = optional_params[raw_value]["default"]

                # handle param not found
                else:
                    raise ValueError(f"parameter attribute missing: {obj_type}.{objdef_name}.{attr_name}")
            
            # handle regular attr 
            else:
                out_value = attr_data["value"]

            attrs[attr_name] = {
                "type": attr_data["type"],
                "value": out_value,
            }
        
        build_list.append({
            "instance_name": f"{obj_data['name']}.{objdef_data['name']}",
            "obj_type": objdef_data["type"],
            "attrs": attrs,
        })

    # override default component body attrs with passed component instance attrs
    body_idx = 0
    for attr_name, attr_data in body_attrs.items():
        build_list[body_idx]["attrs"][attr_name] = {
            "type": attr_data["type"],
            "value": attr_data["value"],
        }

    # build function calls
    fn_calls = []
    for build_entry in build_list:
        process_attrs(build_entry["attrs"])
        result = build_function_calls(build_entry["instance_name"], build_entry["obj_type"], build_entry["attrs"])
        fn_calls.extend(result)

    return fn_calls


def build_function_calls(instance_name: str, obj_type: str, attrs: dict):

    fn_calls = []

    for attr_name, attr_data in attrs.items():

        evaluated_calls = eval_initializer(obj_type, attr_name, attr_data["value"])

        if evaluated_calls is not None:
            fn_calls.append({
                "instance": instance_name,
                "calls": evaluated_calls,
                "comment": f"{attr_name}=\"{attr_data['value_raw']}\"",
            })
        else:
            fn_calls.append({
                "instance": instance_name,
                "calls": [f"<{attr_name}>"],
                "comment": f"function def not found",
            })
    
    return fn_calls

