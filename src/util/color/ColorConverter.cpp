#include "ColorConverter.h"

#include "RgbFloat.h"
#include "HslColor.h"
#include <cmath>
#include <algorithm>

namespace Garbox {

HslColor ColorConverter::rgbToHsl(const RgbFloat& rgb) {
    return rgbToHsl(rgb.r, rgb.g, rgb.b);
}

HslColor ColorConverter::rgbToHsl(float r, float g, float b) {
    float maxVal = std::max({r, g, b});
    float minVal = std::min({r, g, b});
    float delta = maxVal - minVal;

    float h = 0.0f;

    if (delta != 0.0f) {
        if (maxVal == r) {
            h = fmodf(((g - b) / delta), 6.0f);
        }
        else if (maxVal == g) {
            h = ((b - r) / delta) + 2.0f;
        }
        else {
            h = ((r - g) / delta) + 4.0f;
        }

        h /= 6.0f;

        if (h < 0.0f) {
            h += 1.0f;
        }
    }

    float l = (maxVal + minVal) / 2.0f;
    float s = (delta == 0.0f)
        ? 0.0f
        : (delta / (1.0f - fabsf(2.0f * l - 1.0f)));

    return HslColor(h, s, l);
}

RgbFloat ColorConverter::hslToRgb(const HslColor& hsl) {
    float c = (1.0f - fabsf(2.0f * hsl.l - 1.0f)) * hsl.s;
    float hp = hsl.h * 6.0f;
    float x = c * (1.0f - fabsf(fmodf(hp, 2.0f) - 1.0f));
    float m = hsl.l - c / 2.0f;

    float r1 = 0.0f;
    float g1 = 0.0f;
    float b1 = 0.0f;

    if (hp < 1.0f) {
        r1 = c; g1 = x; b1 = 0.0f;
    }
    else if (hp < 2.0f) {
        r1 = x; g1 = c; b1 = 0.0f;
    }
    else if (hp < 3.0f) {
        r1 = 0.0f; g1 = c; b1 = x;
    }
    else if (hp < 4.0f) {
        r1 = 0.0f; g1 = x; b1 = c;
    }
    else if (hp < 5.0f) {
        r1 = x; g1 = 0.0f; b1 = c;
    }
    else {
        r1 = c; g1 = 0.0f; b1 = x;
    }

    return RgbFloat(r1 + m, g1 + m, b1 + m);
}

} // namespace