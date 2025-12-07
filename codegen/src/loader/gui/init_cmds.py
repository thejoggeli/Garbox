from common.util import print_json
from loader.gui.init_cmds_fns import eval_initializer


def build_initializer_commands(gui_data):

    fn_calls = []
    for obj_name, obj_data in gui_data["objects_flat"].items():
        
        result = build_function_calls(obj_data["name"], obj_data["type"], obj_data["attrs"])

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
            raise ValueError(f"invalid attribute '{attr_name}' in tag <{obj_type}>")
    
    return fn_calls

