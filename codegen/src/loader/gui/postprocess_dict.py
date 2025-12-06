from common.util import print_json, nested_get, ensure_list
from loader.gui.parse_color_string import parse_color_to_hex
from common.parse_type import render_value


def render_style_value(val:str):

    if val.endswith("%"):
        val = prep_val(val.removesuffix("%"))
        return f"LV_PCT({val})"
    elif val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val
    elif val == "content":
        return "LV_SIZE_CONTENT"
    return render_value(val, "int")


def render_ex(val:str, type:str):

    if type == "int" and val.endswith("%"):
        val = prep_val(val.removesuffix("%"))
        return f"LV_PCT({val})"
    elif type == "int" and val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val

    return render_value(val, type)

def prep_val(val):
    return val.lower().strip()

def split_attr(val):
    split = val.replace(",", " ")
    split = val.split(" ")
    return split

def postprocess_dict(gui_data):
    gui_data["initializer_commands"] = build_initializer_commands(gui_data)
    return gui_data


def build_initializer_commands(gui_data):

    for obj_name, obj_data in gui_data["objects"].items():

        is_component = obj_data["is_component"]

        # process attrs
        process_attrs(obj_data["attrs"])

        # handle component object
        if is_component:
            build_from_component(obj_data)

        # handle regular lvgl object
        else:
            build_from_object(obj_data)


def build_from_object(obj_data):
    obj_type = obj_data["type"]
    print(f"{obj_type}")
    build_function_calls(obj_data["name"], obj_data["type"], obj_data["attrs"])


def build_from_component(obj_data):

    obj_type = obj_data["type"]
    comp_def = obj_data["component"]

    print(f"{obj_type}")

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

        initializer_fn = nested_get(INITIALIZER_MAP, keys=(obj_type, attr_name), default=None)

        if initializer_fn is None:
            initializer_fn = nested_get(INITIALIZER_MAP, keys=("object", attr_name), default=None)

        if initializer_fn is not None:
            fn_calls.append({
                "instance": instance_name,
                "calls": ensure_list(initializer_fn(attr_data["value"])),
                "comment": f"{attr_name}=\"{attr_data['value_raw']}\"",
            })
        else:
            fn_calls.append({
                "instance": instance_name,
                "calls": [f"<{attr_name}>"],
                "comment": f"function def not found",
            })

    for fn_call in fn_calls:
        instance = fn_call["instance"]
        for call_str in fn_call["calls"]:
            print(f"- {instance}.{call_str}; // {fn_call['comment']}")
    
    return fn_calls


def process_attrs(attrs):

    align = None
    align_x = None
    align_y = None

    for attr_type, attr_data in attrs.items():

        attr_data["value_raw"] = attr_data["value"]

        # attribute 'pad'
        if attr_type == "pad":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = attr_data = [split[0], split[0], split[0], split[0]]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[0], split[1], split[1]]
            elif len(split) == 4:
                attr_data["value"] = [split[0], split[1], split[2], split[3]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected 1 or 2 or 4 integers, got '{attr_data['value']}'")
        
        # attribute 'pad-x'
        elif attr_type == "pad-x" or attr_type == "pad-y":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = [split[0], split[0]]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[1]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected 1 or 2 integers, got '{attr_data['value']}'")

        # align
        elif attr_type == "align":
            split = split_attr(attr_data["value"])
            if len(split) == 1 or isinstance(split, str):
                attr_data["value"] = [split[0], 0, 0]
            elif len(split) == 2:
                attr_data["value"] = [split[0], split[1], split[1]]
            elif len(split) == 3:
                attr_data["value"] = [split[0], split[1], split[2]]
            else:
                raise ValueError(f"invalid '{attr_type}' value. expected str and up to 2 integers, got '{attr_data['value']}'")





TEXT_ALIGN = {
    "auto": "LV_TEXT_ALIGN_AUTO",
    "left": "LV_TEXT_ALIGN_LEFT",
    "center": "LV_TEXT_ALIGN_CENTER",
    "right": "LV_TEXT_ALIGN_RIGHT",
}

DIRECTION = {
    "x":            "LV_DIR_HOR",
    "y":            "LV_DIR_VER",
    "all":          "LV_DIR_ALL",
    "none":         "LV_DIR_NONE",
}

LAYOUT = {
    "flex": "LV_LAYOUT_FLEX",
    "grid": "LV_LAYOUT_GRID",
    "none": "LV_LAYOUT_NONE",
}

FLEX_FLOW = {
    "row":              "LV_FLEX_FLOW_ROW",
    "col":              "LV_FLEX_FLOW_COLUMN",
    "row-wrap":         "LV_FLEX_FLOW_ROW_WRAP",
    "row-reverse":      "LV_FLEX_FLOW_ROW_REVERSE",
    "row-wrap-reverse": "LV_FLEX_FLOW_ROW_WRAP_REVERSE",
    "col-wrap":         "LV_FLEX_FLOW_COLUMN_WRAP",
    "col-reverse":      "LV_FLEX_FLOW_COLUMN_REVERSE",
    "col-wrap-reverse": "LV_FLEX_FLOW_COLUMN_WRAP_REVERSE",
}

ALIGN = {
    "default":          "LV_ALIGN_DEFAULT",
    "top-left":         "LV_ALIGN_TOP_LEFT",
    "top":              "LV_ALIGN_TOP_MID",
    "top-right":        "LV_ALIGN_TOP_RIGHT",
    "bottom-left":      "LV_ALIGN_BOTTOM_LEFT",
    "bottom":           "LV_ALIGN_BOTTOM_MID",
    "bottom-right":     "LV_ALIGN_BOTTOM_RIGHT",
    "left":             "LV_ALIGN_LEFT_MID",
    "right":            "LV_ALIGN_RIGHT_MID",
    "center":           "LV_ALIGN_CENTER",
    "out-top-left":     "LV_ALIGN_OUT_TOP_LEFT",
    "out-top":          "LV_ALIGN_OUT_TOP_MID",
    "out-top-right":    "LV_ALIGN_OUT_TOP_RIGHT",
    "out-bottom-left":  "LV_ALIGN_OUT_BOTTOM_LEFT",
    "out-bottom":       "LV_ALIGN_OUT_BOTTOM_MID",
    "out-bottom-right": "LV_ALIGN_OUT_BOTTOM_RIGHT",
    "out-left-top":     "LV_ALIGN_OUT_LEFT_TOP",
    "out-left":         "LV_ALIGN_OUT_LEFT_MID",
    "out-left-bottom":  "LV_ALIGN_OUT_LEFT_BOTTOM",
    "out-right-top":    "LV_ALIGN_OUT_RIGHT_TOP",
    "out-right":        "LV_ALIGN_OUT_RIGHT_MID",
    "out-right-bottom": "LV_ALIGN_OUT_RIGHT_BOTTOM",
}

INITIALIZER_MAP = {

    "object": {
        "raw-width":            lambda v: f"setRawWidth({render_ex(v,'int')})",
        "raw-height":           lambda v: f"setRawHeight({render_ex(v,'int')})",
        "width":                lambda v: f"setWidth({render_style_value(v)})",
        "height":               lambda v: f"setHeight({render_style_value(v)})",
        "x":                    lambda v: f"setPositionX({render_ex(v,'int')})",
        "y":                    lambda v: f"setPositionY({render_ex(v,'int')})",
        "hidden":               lambda v: f"setHidden({render_ex(v,'bool')})",
        "scrollable":           lambda v: f"setScrollable({render_ex(v,'bool')})",
        "scroll-one":           lambda v: f"setScrollOne({render_ex(v,'bool')})",
        "scroll-chain":         lambda v: f"setScrollChain({render_ex(v,'bool')})",
        "scroll-momentum":      lambda v: f"setScrollMomentum({render_ex(v,'bool')})",
        "scroll-elastic":       lambda v: f"setScrollElastic({render_ex(v,'bool')})",
        "scroll-dir":           lambda v: f"setScrollDirection({DIRECTION[prep_val(v)]})",
        "bg-color":             lambda v: f"setBgColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
        "bg-opacity":           lambda v: f"setBgOpacity({render_ex(v,'int')})",
        "opacity":              lambda v: f"setOpacity({render_ex(v,'int')})",
        "pad":                  lambda v: f"setPad({render_ex(v[0],'int')}, {render_ex(v[1],'int')}, {render_ex(v[2],'int')}, {render_ex(v[3],'int')})",
        "pad-left":             lambda v: f"setPadLeft({render_ex(v,'int')})",
        "pad-right":            lambda v: f"setPadRight({render_ex(v,'int')})",
        "pad-top":              lambda v: f"setPadTop({render_ex(v,'int')})",
        "pad-bottom":           lambda v: f"setPadBottom({render_ex(v,'int')})",
        "pad-x":                lambda v: f"setPadX({render_ex(v[0],'int')}, {render_ex(v[1],'int')})",
        "pad-y":                lambda v: f"setPadY({render_ex(v[0],'int')}, {render_ex(v[1],'int')})",
        "border-width":         lambda v: f"setBorderWidth({render_ex(v,'int')})",
        "border-color":         lambda v: f"setBorderColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
        "radius":               lambda v: f"setRadius({render_ex(v,'int')})",
        "text-letter-space":    lambda v: f"setTextLetterSpace({render_ex(v,'int')})",
        "text-line-space":      lambda v: f"setTextLineSpace({render_ex(v,'int')})",
        "layout":               lambda v: f"setLayout({LAYOUT[prep_val(v)]})",
        "flex-flow":            lambda v: f"setFlexFlow({FLEX_FLOW[prep_val(v)]})",
        "flex-grow":            lambda v: f"setFlexGrow({render_ex(v,'int')})",
        "align":                lambda v: f"setAlign({ALIGN[v[0]]}, {v[1]}, {v[2]})",
    },

    "label": {
        "text":                 lambda v: f"setText({render_ex(v,'string')})",
        "text-color":           lambda v: f"setTextColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
    },

    "image": {
        "src":                  lambda v: f"setSource({v})",
    },

    # "container": {
    #     # intentionally empty — all container behavior is inherited from LvObject
    # },
}
