#include "ColorMap.h"
#include "ColorConverter.h"
#include "ColorInterpolator.h"
#include <algorithm>
#include <cmath>

namespace Garbox {

ColorMap::ColorMap(std::initializer_list<RgbFloat> colors) : 
    // init members
    rgbMap(colors.begin(), colors.end()), 
    hslMap(){
    // constructor body
    hslMap.reserve(rgbMap.size());
    for (const auto& c : rgbMap) {
        hslMap.push_back(HslColor::From(c));
    }
}

ColorMap::ColorMap(std::initializer_list<HslColor> colors) : 
    // init members
    rgbMap(),
    hslMap(colors.begin(), colors.end()){
    // constructor body
    rgbMap.reserve(hslMap.size());
    for (const auto& c : hslMap) {
        rgbMap.push_back(RgbFloat::From(c));
    }
}

RgbFloat ColorMap::interpolateRgb(float t) const {

    if (rgbMap.empty()) return RgbFloat();
    if (t <= 0.0f) return rgbMap.front();
    if (t >= 1.0f) return rgbMap.back();

    float scaled = t * (rgbMap.size() - 1);
    size_t i = static_cast<size_t>(scaled);
    float frac = scaled - i;

    return ColorInterpolator::interpolateRgb(rgbMap[i], rgbMap[i + 1], frac);
}

HslColor ColorMap::interpolateHsl(float t) const {
    
    if (hslMap.empty()) return HslColor();
    if (t <= 0.0f) return hslMap.front();
    if (t >= 1.0f) return hslMap.back();

    float scaled = t * (hslMap.size() - 1);
    size_t i = static_cast<size_t>(scaled);
    float frac = scaled - i;

    return ColorInterpolator::interpolateHsl(hslMap[i], hslMap[i + 1], frac);
}

} // namespace
