#pragma once

#include <array>
#include <cstddef>
#include "RgbFloat.h"
#include "HslColor.h"

namespace Garbox {

class ColorMap {
public:
    ColorMap(std::initializer_list<RgbFloat> colors);
    ColorMap(std::initializer_list<HslColor> colors);

    ~ColorMap();

    ColorMap(const ColorMap&) = delete;
    ColorMap& operator=(const ColorMap&) = delete;
    ColorMap(ColorMap&&) = delete;
    ColorMap& operator=(ColorMap&&) = delete;

    RgbFloat interpolateRgb(float t) const;
    HslColor interpolateHsl(float t) const;

    size_t size() const { return entryCount; }

private:
    RgbFloat* rgbMap;
    HslColor* hslMap;
    size_t entryCount;
};

} // namespace