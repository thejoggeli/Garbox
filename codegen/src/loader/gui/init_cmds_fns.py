
from common.util import print_json, nested_get, ensure_list
from loader.gui.init_cmds_render import *
from loader.gui.init_cmds_types import *


def eval_initializer(obj_type, attr_name, value):

    initializer_fn = nested_get(INITIALIZER_MAP, keys=(obj_type, attr_name), default=None)

    # fall back to object base class
    if initializer_fn is None:
        initializer_fn = nested_get(INITIALIZER_MAP, keys=("object", attr_name), default=None)

    if initializer_fn is not None:
        return ensure_list(initializer_fn(value))

    return None


INITIALIZER_MAP = {

    "object": {
        
        "hidden":           lambda v: f"setHidden({render_bool(v)})",
        "radius":           lambda v: f"setRadius({render_style_val(v)})",

        # position and size
        "x":                lambda v: f"setPositionX({render_style_val(v)})",
        "y":                lambda v: f"setPositionY({render_style_val(v)})",
        "raw-width":        lambda v: render_size(v, prefix="Raw", dim="Width"),
        "raw-height":       lambda v: render_size(v, prefix="Raw", dim="Height"),
        "width":            lambda v: render_size(v, prefix="", dim="Width"),
        "height":           lambda v: render_size(v, prefix="", dim="Height"),

        # scrolling
        "scrollable":       lambda v: f"setScrollable({render_bool(v)})",
        "scroll-one":       lambda v: f"setScrollOne({render_bool(v)})",
        "scroll-chain":     lambda v: f"setScrollChain({render_bool(v)})",
        "scroll-momentum":  lambda v: f"setScrollMomentum({render_bool(v)})",
        "scroll-elastic":   lambda v: f"setScrollElastic({render_bool(v)})",
        "scroll-dir":       lambda v: f"setScrollDirection({DIRECTION[prep_val(v)]})",

        # background and opacity
        "bg-color":         lambda v: f"setBgColor(lv_color_hex(0x{render_color(v)}))",
        "bg-opa":           lambda v: f"setBgOpa({render_opa(v)})",
        "opa":              lambda v: f"setOpa({render_opa(v)})",

        # padding
        "pad":              lambda v: f"setPad({render_margin_padding(v)})",
        "pad-left":         lambda v: f"setPadLeft({render_style_val(v)})",
        "pad-right":        lambda v: f"setPadRight({render_style_val(v)})",
        "pad-top":          lambda v: f"setPadTop({render_style_val(v)})",
        "pad-bottom":       lambda v: f"setPadBottom({render_style_val(v)})",
        "pad-all":          lambda v: f"setPadAll({render_style_val(v)})",
        "pad-hor":          lambda v: f"setPadHor({render_style_val(v)})",
        "pad-ver":          lambda v: f"setPadVer({render_style_val(v)})",
        "pad-gap":          lambda v: f"setPadGap({render_style_val(v)})",
        "pad-row":          lambda v: f"setPadRow({render_style_val(v)})",
        "pad-column":       lambda v: f"setPadColumn({render_style_val(v)})",
        "pad-radial":       lambda v: f"setPadRadial({render_style_val(v)})",

        # margin
        "margin":           lambda v: f"setMargin({render_margin_padding(v)})",
        "margin-left":      lambda v: f"setMarginLeft({render_style_val(v)})",
        "margin-right":     lambda v: f"setMarginRight({render_style_val(v)})",
        "margin-top":       lambda v: f"setMarginTop({render_style_val(v)})",
        "margin-bottom":    lambda v: f"setMarginBottom({render_style_val(v)})",
        "margin-all":       lambda v: f"setMarginAll({render_style_val(v)})",
        "margin-hor":       lambda v: f"setMarginHor({render_style_val(v)})",
        "margin-ver":       lambda v: f"setMarginVer({render_style_val(v)})",

        # border
        "border-width":     lambda v: f"setBorderWidth({render_style_val(v)})",
        "border-color":     lambda v: f"setBorderColor(lv_color_hex(0x{render_color(v)}))",
        "border-opa":       lambda v: f"setBorderOpa({render_opa(v)})",
        "border-side":      lambda v: f"setBorderSide({render_border_side(v)})",
        "border-post":      lambda v: f"setBorderPost({render_color(v)})",

        # layout
        "align":            lambda v: f"setAlign({render_align(v)})",
        "layout":           lambda v: f"setLayout({LAYOUT[prep_val(v)]})",
        "flex-flow":        lambda v: f"setFlexFlow({FLEX_FLOW[prep_val(v)]})",
        "flex-grow":        lambda v: f"setFlexGrow({render_int(v)})",
        "flex-align":       lambda v: f"setFlexAlign({render_flex_align(v)})",

        # text helpers
        "text-letter-space":    lambda v: f"setTextLetterSpace({render_style_val(v)})",
        "text-line-space":      lambda v: f"setTextLineSpace({render_style_val(v)})",
        "text-color":           lambda v: f"setTextColor(lv_color_hex(0x{render_color(v)}))",
        "text-align":           lambda v: f"setTextAlign({TEXT_ALIGN[prep_val(v)]})",
        "text-decor":           lambda v: f"setTextDecor({TEXT_DECOR[prep_val(v)]})",
        "text-opa":             lambda v: f"setTextOpa({render_opa(v)})",
        "text-outline-color":   lambda v: f"setTextOutlineColor(lv_color_hex(0x{render_color(v)}))",
        "text-outline-opa":     lambda v: f"setTextOutlineOpa({render_opa(v)})",
        "text-outline-width":   lambda v: f"setTextOutlineWidth({render_style_val(v)})",

    },

    # label specific functions
    "label": {
        "text":                 lambda v: f"setText({render_string(v)})",
    },

    # image specific functions
    "image": {
        "src":                  lambda v: f"setSource(&{v})",
    },

}

