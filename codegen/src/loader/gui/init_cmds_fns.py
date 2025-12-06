
from common.parse_color_string import parse_color_to_hex
from common.util import print_json, nested_get, ensure_list
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


def render_px(val):

    if isinstance(val, int):
        return val
    
    if isinstance(val, str) and val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val

    raise ValueError(f"invalid val in render_px: {val}")


def render_px_pct(val):

    if isinstance(val, int):
        return val

    if isinstance(val, str) and val.endswith("%"):
        val = prep_val(val.removesuffix("%"))
        return f"LV_PCT({val})"
    
    if isinstance(val, str) and val.endswith("px"):
        val = prep_val(val.removesuffix("px"))
        return val

    raise ValueError(f"invalid val in render_px_pct: {val}")

def prep_val(val):
    return val.lower().strip()

def eval_initializer(obj_type, attr_name, value):

    initializer_fn = nested_get(INITIALIZER_MAP, keys=(obj_type, attr_name), default=None)

    # fall back to object base class
    if initializer_fn is None:
        initializer_fn = nested_get(INITIALIZER_MAP, keys=("object", attr_name), default=None)

    if initializer_fn is not None:
        return ensure_list(initializer_fn(value))

    return None

def fn_size(v, prefix, dim):
    v = prep_val(v)
    if v == "content": 
        return f"set{prefix}{dim}Content()"
    return f"set{prefix}{dim}({render_style_value(v)})"

INITIALIZER_MAP = {

    "object": {
        
        "hidden":   lambda v: f"setHidden({render_value(v,'bool')})",
        "radius":   lambda v: f"setRadius({render_px_pct(v)})",

        # position and size
        "x":            lambda v: f"setPositionX({render_px_pct(v)})",
        "y":            lambda v: f"setPositionY({render_px_pct(v)})",
        "raw-width":    lambda v: fn_size(v, prefix="Raw", dim="Width"),
        "raw-height":   lambda v: fn_size(v, prefix="Raw", dim="Height"),
        "width":        lambda v: fn_size(v, prefix="", dim="Width"),
        "height":       lambda v: fn_size(v, prefix="", dim="Height"),

        # scrolling
        "scrollable":       lambda v: f"setScrollable({render_value(v,'bool')})",
        "scroll-one":       lambda v: f"setScrollOne({render_value(v,'bool')})",
        "scroll-chain":     lambda v: f"setScrollChain({render_value(v,'bool')})",
        "scroll-momentum":  lambda v: f"setScrollMomentum({render_value(v,'bool')})",
        "scroll-elastic":   lambda v: f"setScrollElastic({render_value(v,'bool')})",
        "scroll-dir":       lambda v: f"setScrollDirection({DIRECTION[prep_val(v)]})",

        # background and opacity
        "bg-color":     lambda v: f"setBgColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
        "bg-opacity":   lambda v: f"setBgOpacity({render_value(v,'int')})",
        "opacity":      lambda v: f"setOpacity({render_value(v,'int')})",

        # padding
        "pad":          lambda v: f"setPad({render_px_pct(v[0])}, {render_px_pct(v[1])}, {render_px_pct(v[2])}, {render_px_pct(v[3])})",
        "pad-left":     lambda v: f"setPadLeft({render_px_pct(v)})",
        "pad-right":    lambda v: f"setPadRight({render_px_pct(v)})",
        "pad-top":      lambda v: f"setPadTop({render_px_pct(v)})",
        "pad-bottom":   lambda v: f"setPadBottom({render_px_pct(v)})",
        "pad-all":      lambda v: f"setPadAll({render_px_pct(v)})",
        "pad-hor":      lambda v: f"setPadHor({render_px_pct(v)})",
        "pad-ver":      lambda v: f"setPadVer({render_px_pct(v)})",
        "pad-gap":      lambda v: f"setPadGap({render_px_pct(v)})",
        "pad-row":      lambda v: f"setPadRow({render_px_pct(v)})",
        "pad-column":   lambda v: f"setPadColumn({render_px_pct(v)})",
        "pad-radial":   lambda v: f"setPadRadial({render_px_pct(v)})",

        # margin
        "margin":        lambda v: f"setMargin({render_px_pct(v[0])}, {render_px_pct(v[1])}, {render_px_pct(v[2])}, {render_px_pct(v[3])})",
        "margin-left":   lambda v: f"setMarginLeft({render_px_pct(v)})",
        "margin-right":  lambda v: f"setMarginRight({render_px_pct(v)})",
        "margin-top":    lambda v: f"setMarginTop({render_px_pct(v)})",
        "margin-bottom": lambda v: f"setMarginBottom({render_px_pct(v)})",
        "margin-all":    lambda v: f"setMarginAll({render_px_pct(v)})",
        "margin-hor":    lambda v: f"setMarginHor({render_px_pct(v)})",
        "margin-ver":    lambda v: f"setMarginVer({render_px_pct(v)})",

        # border
        "border-width": lambda v: f"setBorderWidth({render_px_pct(v)})",
        "border-color": lambda v: f"setBorderColor(lv_color_hex(0x{parse_color_to_hex(v)}))",

        # layout
        "align":        lambda v: f"setAlign({ALIGN[v[0]]}, {v[1]}, {v[2]})",
        "layout":       lambda v: f"setLayout({LAYOUT[prep_val(v)]})",
        "flex-flow":    lambda v: f"setFlexFlow({FLEX_FLOW[prep_val(v)]})",
        "flex-grow":    lambda v: f"setFlexGrow({render_value(v,'int')})",
        "flex-align":   lambda v: f"setFlexAlign({FLEX_ALIGN[v[0]]}, {FLEX_ALIGN[v[1]]}, {FLEX_ALIGN[v[2]]})",

        # text helpers
        "text-letter-space":    lambda v: f"setTextLetterSpace({render_px_pct(v)})",
        "text-line-space":      lambda v: f"setTextLineSpace({render_px_pct(v)})",
        "text-color":           lambda v: f"setTextColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
        "text-align":           lambda v: f"setTextAlign({TEXT_ALIGN[prep_val(v)]})",
        "text-decor":           lambda v: f"setTextDecor({TEXT_DECOR[prep_val(v)]})",
        "text-opa":             lambda v: f"setTextOpa({render_value(v,'int')})",
        "text-outline-color":   lambda v: f"setTextOutlineColor(lv_color_hex(0x{parse_color_to_hex(v)}))",
        "text-outline-opa":     lambda v: f"setTextOutlineOpa({render_value(v,'int')})",
        "text-outline-width":   lambda v: f"setTextOutlineWidth({render_px_pct(v)})",

    },

    # label specific functions
    "label": {
        "text":                 lambda v: f"setText({render_value(v,'string')})",
    },

    # image specific functions
    "image": {
        "src":                  lambda v: f"setSource(&{v})",
    },

}

DIRECTION = {
    "hor":  "LV_DIR_HOR",
    "ver":  "LV_DIR_VER",
    "all":  "LV_DIR_ALL",
    "none": "LV_DIR_NONE",
}

LAYOUT = {
    "flex": "LV_LAYOUT_FLEX",
    "grid": "LV_LAYOUT_GRID",
    "none": "LV_LAYOUT_NONE",
}

FLEX_FLOW = {
    "row":                  "LV_FLEX_FLOW_ROW",
    "row-wrap":             "LV_FLEX_FLOW_ROW_WRAP",
    "row-reverse":          "LV_FLEX_FLOW_ROW_REVERSE",
    "row-wrap-reverse":     "LV_FLEX_FLOW_ROW_WRAP_REVERSE",
    "column":               "LV_FLEX_FLOW_COLUMN",
    "column-wrap":          "LV_FLEX_FLOW_COLUMN_WRAP",
    "column-reverse":       "LV_FLEX_FLOW_COLUMN_REVERSE",
    "column-wrap-reverse":  "LV_FLEX_FLOW_COLUMN_WRAP_REVERSE",
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

TEXT_ALIGN = {
    "auto": "LV_TEXT_ALIGN_AUTO",
    "left": "LV_TEXT_ALIGN_LEFT",
    "center": "LV_TEXT_ALIGN_CENTER",
    "right": "LV_TEXT_ALIGN_RIGHT",
}

TEXT_DECOR = {
    "none":          "LV_TEXT_DECOR_NONE",
    "underline":     "LV_TEXT_DECOR_UNDERLINE",
    "strikethrough": "LV_TEXT_DECOR_STRIKETHROUGH",
}

BORDER_SIDE = {
    "none":      "LV_BORDER_SIDE_NONE",
    "bottom":    "LV_BORDER_SIDE_BOTTOM",
    "top":       "LV_BORDER_SIDE_TOP",
    "left":      "LV_BORDER_SIDE_LEFT",
    "right":     "LV_BORDER_SIDE_RIGHT",
    "full":      "LV_BORDER_SIDE_FULL",
    "internal":  "LV_BORDER_SIDE_INTERNAL",
}

BLEND_MODE = {
    "normal":      "LV_BLEND_MODE_NORMAL",
    "additive":    "LV_BLEND_MODE_ADDITIVE",
    "subtractive": "LV_BLEND_MODE_SUBTRACTIVE",
    "multiply":    "LV_BLEND_MODE_MULTIPLY",
    "difference":  "LV_BLEND_MODE_DIFFERENCE",
}

BASE_DIR = {
    "ltr":   "LV_BASE_DIR_LTR",
    "rtl":   "LV_BASE_DIR_RTL",
    "auto":  "LV_BASE_DIR_AUTO",
    "neutral": "LV_BASE_DIR_NEUTRAL",
}

GRAD_DIR = {
    "none":   "LV_GRAD_DIR_NONE",
    "hor":    "LV_GRAD_DIR_HOR",
    "ver":    "LV_GRAD_DIR_VER",
}

LINE_ROUNDED = {
    "off": 0,
    "on":  1,
}

ARC_ROUNDED = {
    "off": 0,
    "on":  1,
}

FLEX_ALIGN = {
    "start": "LV_FLEX_ALIGN_START",
    "center": "LV_FLEX_ALIGN_CENTER",
    "end": "LV_FLEX_ALIGN_END",
    "space-between": "LV_FLEX_ALIGN_SPACE_BETWEEN",
    "space-around": "LV_FLEX_ALIGN_SPACE_AROUND",
    "space-evenly": "LV_FLEX_ALIGN_SPACE_EVENLY",
}

GRID_ALIGN = {
    "start":  "LV_GRID_ALIGN_START",
    "center": "LV_GRID_ALIGN_CENTER",
    "end":    "LV_GRID_ALIGN_END",
    "stretch": "LV_GRID_ALIGN_STRETCH",
}
