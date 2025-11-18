#pragma once

#include "core/util/color/ColorMap.h"

namespace Garbox {

class ColorMaps {
public:

    static const ColorMap& GetRedBlue();
    static const ColorMap& GetTestRBR_Uniform();
    static const ColorMap& GetTestRBR_NonUniform();
    static const ColorMap& GetViridis();

};

} // namespace