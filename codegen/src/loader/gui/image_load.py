from loader.gui.image_convert import (
    convert_to_rgb565,
    convert_to_rgb565a8
)

SUPPORTED_FORMATS = {
    "RGB565": {
        "converter": convert_to_rgb565,
        "cf_enum": "LV_COLOR_FORMAT_RGB565",
        "bytes_per_pixel": 2,
    },
    "RGB565A8": {
        "converter": convert_to_rgb565a8,
        "cf_enum": "LV_COLOR_FORMAT_RGB565A8",
        "bytes_per_pixel": 3,
    },
}

def load_lvgl_image(path, name, color_format):
    """
    Convert image at 'path' into LVGL raw bytes.

    color_format must be one of:
        - "RGB565"
        - "RGB565A8"
    """
    if color_format not in SUPPORTED_FORMATS:
        raise ValueError(
            f"Unsupported LVGL color format: {color_format!r}. "
            f"Supported: {list(SUPPORTED_FORMATS.keys())}"
        )

    spec = SUPPORTED_FORMATS[color_format]

    w, h, raw = spec["converter"](path)

    return {
        "name": name,
        "width": w,
        "height": h,
        "data_size": len(raw),
        "bytes_per_pixel": spec["bytes_per_pixel"],
        "color_format": spec["cf_enum"],
        "data_bytes": raw,
        "data_hex": [f"0x{b:02x}" for b in raw],
    }

