#pragma once

#include <vector>
#include <cstddef>
#include "RgbFloat.h"
#include "HslColor.h"

namespace Garbox {

/*
 * WARNING:
 * This class performs dynamic heap allocation.
 * It is intended for use only as a static or global object in embedded environments.
 * Do NOT instantiate it on the stack or in frequently called functions.
 */
class ColorMap {
public:
    ColorMap(std::initializer_list<RgbFloat> colors);
    ColorMap(std::initializer_list<HslColor> colors);

    ~ColorMap() = default;

    ColorMap(const ColorMap&) = delete;
    ColorMap& operator=(const ColorMap&) = delete;
    ColorMap(ColorMap&&) = delete;
    ColorMap& operator=(ColorMap&&) = delete;

    RgbFloat interpolateRgb(float t) const;
    HslColor interpolateHsl(float t) const;

    size_t size() const { return rgbMap.size(); }

private:
    std::vector<RgbFloat> rgbMap;
    std::vector<HslColor> hslMap;
};

} // namespace Garbox
