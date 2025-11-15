#include "ColorInterpolator.h"

#include <cmath>
#include "util/color/types/HslColor.h"
#include "util/color/types/LabColor.h"
#include "util/color/types/RgbFloat.h"

namespace Garbox {

RgbFloat ColorInterpolator::interpolateRgb(const RgbFloat& a, const RgbFloat& b, float t){
    return RgbFloat(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t
    );
}

HslColor ColorInterpolator::interpolateHsl(const HslColor& a, const HslColor& b, float t){
    float dh = b.h - a.h;
    if (std::fabs(dh) > 0.5f){
        if (dh > 0.0f){
            dh -= 1.0f;
        }
        else {
            dh += 1.0f;
        }
    }

    float h = std::fmod(a.h + dh * t, 1.0f);
    if (h < 0.0f){
        h += 1.0f;
    }

    float s = a.s + (b.s - a.s) * t;
    float l = a.l + (b.l - a.l) * t;

    return HslColor(h, s, l);
}

LabColor ColorInterpolator::interpolateLab(const LabColor& a, const LabColor& b, float t){
    // linear interpolation in Lab space
    const float l = a.L + (b.L - a.L) * t;
    const float A = a.a + (b.a - a.a) * t;
    const float B = a.b + (b.b - a.b) * t;
    return LabColor(l, A, B);
}

} // namespace