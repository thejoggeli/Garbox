from loader.gui.init_cmds_types import *
from common.parse_type import render_value
from common.parse_color_string import parse_color_to_hex


def prep_val(val):
    return val.lower().strip()


def split_attr(val):
    split = val.replace(",", " ")
    split = val.split(" ")
    return split


def render_color(val):
    return parse_color_to_hex(val)


def render_string(val):
    return render_value(val ,'string')


def render_int(val):
    return render_value(val ,'int')


def render_bool(val):
    return render_value(val ,'bool')


def render_size(v:str, prefix, dim):
    v = v.lower().strip()
    if v == "content": 
        return f"set{prefix}{dim}Content()"
    return f"set{prefix}{dim}({render_style_val(v)})"


def render_opa(v):
    
    # case: literal string "cover"
    if isinstance(v, str) and v.strip().lower() == "cover":
        return 255
    
    # case: "NN%" string
    if isinstance(v, str) and v.endswith("%"):
        try:
            pct = float(v[:-1])
        except ValueError:
            raise ValueError(f"Invalid opacity percent: {v}")

        value = int(round((pct / 100.0) * 255))
        return max(0, min(255, value))

    # case: int or convertible-to-int
    try:
        num = int(v)
    except Exception:
        raise ValueError(f"Invalid opacity value: {v}")

    return max(0, min(255, num))


def render_style_val(val):

    if isinstance(val, int):
        return val

    if isinstance(val, str) and val.endswith("%"):
        val = prep_val(val.removesuffix("%"))
        return f"LV_PCT({val})"
    
    if isinstance(val, str) and val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val
    
    if val == "content":
        val = prep_val(val.removesuffix("px"))
        return "LV_SIZE_CONTENT"
    
    return render_value(val, "int")


def render_px(val):

    if isinstance(val, int):
        return val
    
    if isinstance(val, str) and val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val

    raise ValueError(f"invalid val in render_px: {val}")


def render_border_side(val):

    split = split_attr(val)

    out = ""
    delim = ""
    for side in split:
        out = out + delim + BORDER_SIDE[side]
        delim = " | "

    return out


def render_margin_padding(val):

    split = split_attr(val)

    if len(split) == 1 or isinstance(split, str):
        v = [split[0], split[0], split[0], split[0]]
    elif len(split) == 2:
        v = [split[0], split[0], split[1], split[1]]
    elif len(split) == 4:
        v = [split[0], split[1], split[2], split[3]]
    else:
        raise ValueError(f"invalid margin or padding value. expected 1 or 2 or 4 integers, got '{val}'")
    
    return f"{render_style_val(v[0])}, {render_style_val(v[1])}, {render_style_val(v[2])}, {render_style_val(v[3])}"
    

def render_align(val):
    
    split = split_attr(val)

    if len(split) == 1 or isinstance(split, str):
        v = [split[0], 0, 0]
    elif len(split) == 2:
        v = [split[0], split[1], split[1]]
    elif len(split) == 3:
        v = [split[0], split[1], split[2]]
    else:
        raise ValueError(f"invalid align value. expected str and up to 2 integers, got '{val}'")
    
    return f"{ALIGN[v[0]]}, {v[1]}, {v[2]}"

def render_flex_align(val):

    split = split_attr(val)

    if len(split) == 1 or isinstance(split, str):
        v = [split[0], "start", "start"]
    elif len(split) == 2:
        v = [split[0], split[1], "start"]
    elif len(split) == 3:
        v = [split[0], split[1], split[2]]
    else:
        raise ValueError(f"invalid flex-align value. expected 1 or 2 or 3 values, got '{v}'")
    
    return f"{FLEX_ALIGN[v[0]]}, {FLEX_ALIGN[v[1]]}, {FLEX_ALIGN[v[2]]}"