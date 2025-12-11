from loader.gui.init_cmds_types import *
from common.parse_type import render_value
from common.parse_color_string import parse_color_to_hex
import math

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


def render_zoom(v):
    # numeric literal => return unchanged
    if isinstance(v, int):
        return v

    if isinstance(v, float):
        return int(v)

    if isinstance(v, str):
        s = v.strip().lower()

        # percent format: "125%" or "25.7%"
        if s.endswith("%"):
            num_str = s[:-1].strip()
            try:
                pct = float(num_str)
            except ValueError:
                raise ValueError(f"Invalid zoom percentage '{v}'")

            # LVGL zoom unit: 256 = 100%
            return int(round((pct / 100.0) * 256))

        # raw integer in string form
        try:
            return int(s)
        except ValueError:
            raise ValueError(f"Invalid zoom value '{v}'")

    raise ValueError(f"Invalid zoom type '{v}'")



def render_angle(v):
    # raw numeric value
    if isinstance(v, int):
        return v

    if isinstance(v, float):
        # treat this as raw LVGL value
        return int(round(v))

    if isinstance(v, str):
        s = v.strip().lower()

        # degree formats
        if s.endswith(("deg", "degs")):
            num = s.replace("degs", "").replace("deg", "").strip()
            try:
                deg = float(num)
            except ValueError:
                raise ValueError(f"Invalid degree angle '{v}'")
            return int(round(deg * 10))

        # radian formats
        if s.endswith(("rad", "rads")):
            num = s.replace("rads", "").replace("rad", "").strip()
            try:
                rad = float(num)
            except ValueError:
                raise ValueError(f"Invalid radian angle '{v}'")
            deg = rad * (180.0 / math.pi)
            return int(round(deg * 10))

        # raw integer (string)
        try:
            return int(s)
        except ValueError:
            raise ValueError(f"Invalid angle value '{v}'")

    raise ValueError(f"Invalid angle type '{v}'")


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
        v = [split[0], split[0], split[0]]
    elif len(split) == 3:
        v = [split[0], split[1], split[2]]
    else:
        raise ValueError(f"invalid flex-align value. expected 1 or 3 values, got '{v}'")
    
    return f"{FLEX_ALIGN[v[0]]}, {FLEX_ALIGN[v[1]]}, {FLEX_ALIGN[v[2]]}"


def render_transform_size(val):
    split = split_attr(val)
    if len(split) == 2:
        return f"{render_px(split[0])}, {render_px(split[1])}"
    raise ValueError(f"invalid transform-size value. expected 2 values, got '{val}'")
    

def render_transform_pivot(val):
    split = split_attr(val)
    if len(split) == 2:
        return f"{render_px(split[0])}, {render_px(split[1])}"
    raise ValueError(f"invalid transform-pivot value. expected 2 values, got '{val}'")
    

def render_transform_zoom(val):
    split = split_attr(val)
    if len(split) == 1:
        return f"{render_zoom(split[0])}, {render_zoom(split[0])}"
    if len(split) == 2:
        return f"{render_zoom(split[0])}, {render_zoom(split[1])}"
    raise ValueError(f"invalid transform-zoom value. expected 1 or 2 values, got '{val}'")
    

def render_transform_skew(val):
    split = split_attr(val)
    if len(split) == 2:
        return f"{render_angle(split[0])}, {render_angle(split[1])}"
    raise ValueError(f"invalid transform-skew value. expected 2 values, got '{val}'")
    
