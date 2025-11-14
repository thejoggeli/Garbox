#include "ColorConverter.h"

#include <algorithm>
#include <math.h>
#include <cstdint>

#include "ColorConstants.h"
#include "types/HslColor.h"
#include "types/LabColor.h"
#include "types/Rgb565.h"
#include "types/Rgb888.h"
#include "types/RgbFloat.h"
#include "util/color/ColorFunctions.h"
#include "util/function/default/GammaFunctions.h"

namespace Garbox {

static inline float sampledCbrt(float v){
    static const MathFunctionIfc& cbrt = ColorFunctions::GetLabCbrt();
    return cbrt.evaluate(v);
}

static inline float clamp01(float v){
    if(v < 0.0f){
        return 0.0f;
    }
    else if(v > 1.0f){
        return 1.0f;
    }
    else {
        return v;
    }
}

static inline uint8_t clamp8(int v){
    if(v < 0){
        return static_cast<uint8_t>(0);
    }
    else if(v > 255){
        return static_cast<uint8_t>(255);
    }
    else {
        return static_cast<uint8_t>(v);
    }
}

// ----------------------------------------------------------------------------------
// --- Lab --------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// RgbFloat <-> XYZ (D65)
// Input RgbFloat assumed in [0..1]
static inline void rgbFloatToXyz(const RgbFloat& rgbLinear, float& x, float& y, float& z){
    // sRGB D65 matrix (from IEC 61966-2-1)
    x = (0.4124564f * rgbLinear.r + 0.3575761f * rgbLinear.g + 0.1804375f * rgbLinear.b) * 100.0f;
    y = (0.2126729f * rgbLinear.r + 0.7151522f * rgbLinear.g + 0.0721750f * rgbLinear.b) * 100.0f;
    z = (0.0193339f * rgbLinear.r + 0.1191920f * rgbLinear.g + 0.9503041f * rgbLinear.b) * 100.0f;
}

static inline RgbFloat xyzToRgbFloat(float x, float y, float z){
    // Normalize from percent
    x *= 0.01f;
    y *= 0.01f;
    z *= 0.01f;

    // Inverse sRGB D65 matrix
    float rLin =  3.2404542f * x - 1.5371385f * y - 0.4985314f * z;
    float gLin = -0.9692660f * x + 1.8760108f * y + 0.0415560f * z;
    float bLin =  0.0556434f * x - 0.2040259f * y + 1.0572252f * z;

    // Clamp linear before companding
    if(rLin < 0.0f){ rLin = 0.0f; }
    else if(rLin > 1.0f){ rLin = 1.0f; }

    if(gLin < 0.0f){ gLin = 0.0f; }
    else if(gLin > 1.0f){ gLin = 1.0f; }

    if(bLin < 0.0f){ bLin = 0.0f; }
    else if(bLin > 1.0f){ bLin = 1.0f; }

    return RgbFloat(rLin, gLin, bLin);
}

// XYZ <=> Lab
static inline float fLab(float t){
    if(t > ColorConstants::LabEpsilon){
        return sampledCbrt(t);
    }
    else {
        return (ColorConstants::LabKappa * t + 16.0f) / 116.0f;
    }
}

static inline float fLabInv(float ft){
    const float ft3 = ft * ft * ft;
    if(ft3 > ColorConstants::LabEpsilon){
        return ft3;
    }
    else {
        return (116.0f * ft - 16.0f) / ColorConstants::LabKappa;
    }
}

static inline LabColor xyzToLab(float x, float y, float z){
    const float xr = x / ColorConstants::LabD65Xn;
    const float yr = y / ColorConstants::LabD65Yn;
    const float zr = z / ColorConstants::LabD65Zn;

    const float fx = fLab(xr);
    const float fy = fLab(yr);
    const float fz = fLab(zr);

    return LabColor(
        116.0f * fy - 16.0f, // L
        500.0f * (fx - fy),  // a
        200.0f * (fy - fz)   // b
    );
}

static inline void labToXyz(const LabColor& lab, float& x, float& y, float& z){
    const float fy = (lab.L + 16.0f) / 116.0f;
    const float fx = fy + (lab.a / 500.0f);
    const float fz = fy - (lab.b / 200.0f);

    const float xr = fLabInv(fx);
    const float yr = fLabInv(fy);
    const float zr = fLabInv(fz);

    x = xr * ColorConstants::LabD65Xn;
    y = yr * ColorConstants::LabD65Yn;
    z = zr * ColorConstants::LabD65Zn;
}

// HSL helpers
static inline float hueToRgb(float p, float q, float t){
    if(t < 0.0f){
        t += 1.0f;
    }
    else if(t > 1.0f){
        t -= 1.0f;
    }

    if(t < 1.0f / 6.0f){
        return p + (q - p) * 6.0f * t;
    }
    else if(t < 1.0f / 2.0f){
        return q;
    }
    else if(t < 2.0f / 3.0f){
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    }
    else {
        return p;
    }
}

static inline RgbFloat hslToRgb(const HslColor& hsl) {
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

static inline HslColor rgbToHsl(const RgbFloat& rgb) {
    float maxVal = std::max({rgb.r, rgb.g, rgb.b});
    float minVal = std::min({rgb.r, rgb.g, rgb.b});
    float delta = maxVal - minVal;

    float h = 0.0f;

    if (delta != 0.0f) {
        if (maxVal == rgb.r) {
            h = fmodf(((rgb.g - rgb.b) / delta), 6.0f);
        }
        else if (maxVal == rgb.g) {
            h = ((rgb.b - rgb.r) / delta) + 2.0f;
        }
        else {
            h = ((rgb.r - rgb.g) / delta) + 4.0f;
        }

        h /= 6.0f;

        if (h < 0.0f) {
            h += 1.0f;
        }
    }

    float l = (maxVal + minVal) / 2.0f;
    float s = (delta == 0.0f) ? 0.0f : (delta / (1.0f - fabsf(2.0f * l - 1.0f)));

    return HslColor(h, s, l);
}

// ----------------------------------------------------------------------------------
// --- Rgb888 <-> Rgb565 helpers  ---------------------------------------------------
// ----------------------------------------------------------------------------------

static inline Rgb565 packRgb888To565(const Rgb888& c){
    const uint16_t r5 = static_cast<uint16_t>((static_cast<uint32_t>(c.r) * 31u + 127u) / 255u);
    const uint16_t g6 = static_cast<uint16_t>((static_cast<uint32_t>(c.g) * 63u + 127u) / 255u);
    const uint16_t b5 = static_cast<uint16_t>((static_cast<uint32_t>(c.b) * 31u + 127u) / 255u);
    const uint16_t value = static_cast<uint16_t>((r5 << 11) | (g6 << 5) | b5);
    return Rgb565(value);
}

static inline Rgb888 unpack565ToRgb888(uint16_t v){
    const uint8_t r5 = static_cast<uint8_t>((v >> 11) & 0x1F);
    const uint8_t g6 = static_cast<uint8_t>((v >> 5)  & 0x3F);
    const uint8_t b5 = static_cast<uint8_t>( v        & 0x1F);

    // Expand with bit replication
    const uint8_t r = static_cast<uint8_t>((r5 << 3) | (r5 >> 2));
    const uint8_t g = static_cast<uint8_t>((g6 << 2) | (g6 >> 4));
    const uint8_t b = static_cast<uint8_t>((b5 << 3) | (b5 >> 2));
    return Rgb888(r, g, b);
}

// ----------------------------------------------------------------------------------
// --- RgbFloat <-> Rgb888 helpers  -------------------------------------------------
// ----------------------------------------------------------------------------------

static inline Rgb888 rgbFloatToRgb888(const RgbFloat& c){
    Rgb888 out;
    out.r = clamp8(static_cast<int>(std::round(clamp01(c.r) * 255.0f)));
    out.g = clamp8(static_cast<int>(std::round(clamp01(c.g) * 255.0f)));
    out.b = clamp8(static_cast<int>(std::round(clamp01(c.b) * 255.0f)));
    return out;
}

static inline RgbFloat rgb888ToRgbFloat(const Rgb888& c){
    RgbFloat out;
    out.r = static_cast<float>(c.r) / 255.0f;
    out.g = static_cast<float>(c.g) / 255.0f;
    out.b = static_cast<float>(c.b) / 255.0f;
    return out;
}

// ----------------------------------------------------------------------------------
// --- any => Rgb565  ---------------------------------------------------------------
// ----------------------------------------------------------------------------------

Rgb565 ColorConverter::ToRgb565(const HslColor& src){
    const RgbFloat rf = hslToRgb(src);
    const Rgb888 c888 = rgbFloatToRgb888(rf);
    return packRgb888To565(c888);
}

Rgb565 ColorConverter::ToRgb565(const Rgb888& src){
    return packRgb888To565(src);
}

Rgb565 ColorConverter::ToRgb565(const RgbFloat& src){
    const Rgb888 c888 = rgbFloatToRgb888(src);
    return packRgb888To565(c888);
}

// ----------------------------------------------------------------------------------
// --- any => Rgb888  ---------------------------------------------------------------
// ----------------------------------------------------------------------------------

Rgb888 ColorConverter::ToRgb888(const HslColor& src){
    const RgbFloat rf = hslToRgb(src);
    return rgbFloatToRgb888(rf);
}

Rgb888 ColorConverter::ToRgb888(const Rgb565& src){
    return unpack565ToRgb888(src.value);
}

Rgb888 ColorConverter::ToRgb888(const RgbFloat& src){
    return rgbFloatToRgb888(src);
}

// ----------------------------------------------------------------------------------
// --- any => RgbFloat  -------------------------------------------------------------
// ----------------------------------------------------------------------------------

RgbFloat ColorConverter::ToRgbFloat(const HslColor& src){
    return hslToRgb(src);
}

RgbFloat ColorConverter::ToRgbFloat(const Rgb888& src){
    return rgb888ToRgbFloat(src);
}

RgbFloat ColorConverter::ToRgbFloat(const Rgb565& src){
    const Rgb888 c888 = unpack565ToRgb888(src.value);
    return rgb888ToRgbFloat(c888);
}

// ----------------------------------------------------------------------------------
// --- any => HslColor  -------------------------------------------------------------
// ----------------------------------------------------------------------------------

HslColor ColorConverter::ToHsl(const RgbFloat& src){
    return rgbToHsl(src);
}

HslColor ColorConverter::ToHsl(const Rgb888& src){
    const RgbFloat rf = rgb888ToRgbFloat(src);
    return rgbToHsl(rf);
}

HslColor ColorConverter::ToHsl(const Rgb565& src){
    const Rgb888 c888 = unpack565ToRgb888(src.value);
    const RgbFloat rf = rgb888ToRgbFloat(c888);
    return rgbToHsl(rf);
}

// ----------------------------------------------------------------------------------
// --- LabColor <=> Linear RGB  -----------------------------------------------------
// ----------------------------------------------------------------------------------

LabColor ColorConverter::ToLab(const RgbFloat& src){
    float x, y, z;
    rgbFloatToXyz(src, x, y, z);
    return xyzToLab(x, y, z);
}

RgbFloat ColorConverter::ToRgbFloat(const LabColor& src){
    float x, y, z;
    labToXyz(src, x, y, z);
    return xyzToRgbFloat(x, y, z);
}

// ----------------------------------------------------------------------------------
// --- Linear RGB <=> Standard RGB  -------------------------------------------------
// ----------------------------------------------------------------------------------

RgbFloat ColorConverter::ToLinearRgb(const HslColor& hsl){
    RgbFloat standardRgb = ColorConverter::ToRgbFloat(hsl);
    return ColorConverter::ToLinearRgb(standardRgb);
}

RgbFloat ColorConverter::ToLinearRgb(const LabColor& lab){
    return ColorConverter::ToRgbFloat(lab);
}

RgbFloat ColorConverter::ToLinearRgb(const RgbFloat& rgb){
    static const MathFunctionIfc& gamma = GammaFunctions::GetGamma22();
    return RgbFloat(
        gamma.evaluate(rgb.r),
        gamma.evaluate(rgb.g),
        gamma.evaluate(rgb.b)
    );
}

RgbFloat ColorConverter::ToStandardRgb(const HslColor& hsl){
    return ColorConverter::ToRgbFloat(hsl);
}

RgbFloat ColorConverter::ToStandardRgb(const LabColor& lab){
    RgbFloat linearRgb = ColorConverter::ToRgbFloat(lab);
    return ColorConverter::ToStandardRgb(linearRgb);
}

RgbFloat ColorConverter::ToStandardRgb(const RgbFloat& rgb){
    static const MathFunctionIfc& gammaInverse = GammaFunctions::GetGammaInverse22();
    return RgbFloat(
        gammaInverse.evaluate(rgb.r),
        gammaInverse.evaluate(rgb.g),
        gammaInverse.evaluate(rgb.b)
    );
}

} // namespace
