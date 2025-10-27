#include "ColorMap.h"

#include "ColorConverter.h"
#include "ColorInterpolator.h"
#include <algorithm>
#include <cmath>

namespace Garbox {

ColorMap::ColorMap(std::initializer_list<RgbFloat> colors) : entryCount(colors.size()) {
    rgbMap = new RgbFloat[entryCount];
    hslMap = new HslColor[entryCount];

    size_t i = 0;
    for (const auto& c : colors) {
        rgbMap[i] = c;
        hslMap[i] = c.toHsl();
        ++i;
    }
}

ColorMap::ColorMap(std::initializer_list<HslColor> colors) : entryCount(colors.size()) {
    rgbMap = new RgbFloat[entryCount];
    hslMap = new HslColor[entryCount];

    size_t i = 0;
    for (const auto& c : colors) {
        hslMap[i] = c;
        rgbMap[i] = c.toRgb();
        ++i;
    }
}

ColorMap::~ColorMap(){
    delete rgbMap;
    delete hslMap;
}

RgbFloat ColorMap::interpolateRgb(float t) const {
    if (entryCount == 0) {
        return RgbFloat();
    }

    if (t <= 0.0f) {
        return rgbMap[0];
    }

    if (t >= 1.0f) {
        return rgbMap[entryCount - 1];
    }

    float scaled = t * (entryCount - 1);
    size_t i = static_cast<size_t>(scaled);
    float frac = scaled - i;

    return ColorInterpolator::interpolateRgb(rgbMap[i], rgbMap[i + 1], frac);
}

HslColor ColorMap::interpolateHsl(float t) const {
    if (entryCount == 0) {
        return HslColor();
    }

    if (t <= 0.0f) {
        return hslMap[0];
    }

    if (t >= 1.0f) {
        return hslMap[entryCount - 1];
    }

    float scaled = t * (entryCount - 1);
    size_t i = static_cast<size_t>(scaled);
    float frac = scaled - i;

    return ColorInterpolator::interpolateHsl(hslMap[i], hslMap[i + 1], frac);
}

} // namespace