#include "ColorMaps.h"

namespace Garbox {

const ColorMap& ColorMaps::GetTestLed(){
    static const ColorMap instance = ColorMap({
        RgbFloat(1, 0, 0),
        RgbFloat(0, 0, 1),
        RgbFloat(1, 0, 0),
    });
    return instance;
}

const ColorMap& ColorMaps::GetTestDisplay(){
    static const ColorMap instance = ColorMap({
        RgbFloat(1, 0, 0),
        RgbFloat(0, 0, 1),
        RgbFloat(0, 1, 0),
        RgbFloat(0, 0, 1),
        RgbFloat(1, 0, 0),
    });
    return instance;   
}

} // namespace