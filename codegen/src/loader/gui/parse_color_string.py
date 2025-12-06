import re

def parse_color_to_hex(s):
    s = s.strip().lower()

    # ------------------------------------------------------------
    # HEX
    # ------------------------------------------------------------
    if s.startswith("#"):
        s = s[1:]
        if len(s) == 3:
            # expand #123 → 112233
            return "".join(c*2 for c in s)
        if len(s) == 6:
            return s
        raise ValueError(f"Invalid hex color: {s}")

    # ------------------------------------------------------------
    # RGB / RGBA
    # ------------------------------------------------------------
    m = re.match(r"rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)", s)
    if m:
        r, g, b = map(int, m.groups())
        r = max(0, min(r, 255))
        g = max(0, min(g, 255))
        b = max(0, min(b, 255))
        return f"{r:02x}{g:02x}{b:02x}"

    # ------------------------------------------------------------
    # HSL / HSLA
    # ------------------------------------------------------------
    m = re.match(
        r"hsla?\(\s*([0-9.]+)\s*,\s*([0-9.]+)%\s*,\s*([0-9.]+)%", s
    )
    if m:
        h, s, l = map(float, m.groups())
        h = h % 360
        s /= 100
        l /= 100

        # convert HSL -> RGB manually
        # ---------------------------------
        def hue_to_rgb(p, q, t):
            if t < 0: t += 1
            if t > 1: t -= 1
            if t < 1/6: return p + (q - p) * 6 * t
            if t < 1/2: return q
            if t < 2/3: return p + (q - p) * (2/3 - t) * 6
            return p

        if s == 0:
            r = g = b = l
        else:
            q = l * (1 + s) if l < 0.5 else (l + s - l*s)
            p = 2*l - q
            r = hue_to_rgb(p, q, h/360 + 1/3)
            g = hue_to_rgb(p, q, h/360)
            b = hue_to_rgb(p, q, h/360 - 1/3)

        R = int(round(r * 255))
        G = int(round(g * 255))
        B = int(round(b * 255))
        return f"{R:02x}{G:02x}{B:02x}"

    # ------------------------------------------------------------
    raise ValueError(f"Unrecognized color format: {s}")
