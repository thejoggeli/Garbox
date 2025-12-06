from PIL import Image

def convert_to_rgb565(path):
    """
    Convert a PIL RGBA image into raw RGB565 bytes (2 bytes per pixel).
    """

    img = Image.open(path)

    img = img.convert("RGBA")
    w, h = img.size
    px = img.load()

    out = bytearray()

    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]

            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F
            b5 = (b >> 3) & 0x1F

            rgb565 = (r5 << 11) | (g6 << 5) | b5

            out.append((rgb565 >> 8) & 0xFF)
            out.append(rgb565 & 0xFF)

    return w, h, bytes(out)


def convert_to_rgb565a8(path):
    """
    Convert to LVGL-style RGB565A8:
        - first 2 bytes: RGB565
        - next 1 byte: alpha (8 bit)
    """

    img = Image.open(path)
    img = img.convert("RGBA")
    
    w, h = img.size
    px = img.load()

    out = bytearray()

    for y in range(h):
        for x in range(w):
            r, g, b, a = px[x, y]

            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F
            b5 = (b >> 3) & 0x1F

            rgb565 = (r5 << 11) | (g6 << 5) | b5

            out.append((rgb565 >> 8) & 0xFF)
            out.append(rgb565 & 0xFF)
            out.append(a)

    return w, h, bytes(out)