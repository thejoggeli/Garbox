#include "Rgb888.h"

#include "HslColor.h"
#include "RgbFloat.h"
#include "util/color/ColorConverter.h"

namespace Garbox {

Rgb888::Rgb888() : r(0), g(0), b(0){
    // constructor body
}

Rgb888::Rgb888(uint32_t hex): 
    r((hex >>  0) & 0xFF), 
    g((hex >>  8) & 0xFF), 
    b((hex >> 16) & 0xFF){
    // constructor body
}

Rgb888::Rgb888(uint8_t rVal, uint8_t gVal, uint8_t bVal): 
    r(rVal), 
    g(gVal), 
    b(bVal){
    // constructor body
}

Rgb888 Rgb888::From(const Rgb565& rgb){
    return ColorConverter::ToRgb888(rgb);
}

Rgb888 Rgb888::From(const RgbFloat& rgb){
    return ColorConverter::ToRgb888(rgb);
}

Rgb888 Rgb888::From(const HslColor& hsl){
    return ColorConverter::ToRgb888(hsl);
}
    
Rgb888 Rgb888::FromRgbFloat(float r, float g, float b){
    return ColorConverter::ToRgb888(RgbFloat(r, g, b));
}

Rgb888 Rgb888::FromHsl(float h, float s, float l){
    return ColorConverter::ToRgb888(HslColor(h, s, l));
}

Rgb888 Rgb888::FromHex(uint32_t hex){
    return Rgb888(hex);
}

} // namespace