#include "Rgb565.h"

#include "Rgb888.h"
#include "RgbFloat.h"
#include "HslColor.h"
#include "core/util/color/ColorConverter.h"

namespace Garbox {

Rgb565::Rgb565() : value(0){}

Rgb565::Rgb565(uint16_t val) : value(val){}

Rgb565::Rgb565(uint8_t r5, uint8_t g6, uint8_t b5){
    value = ((r5 & 0x1F) << 11) | ((g6 & 0x3F) << 5) | (b5 & 0x1F);
}

Rgb565 Rgb565::From(const HslColor& hsl){
    return ColorConverter::ToRgb565(hsl);
}

Rgb565 Rgb565::From(const Rgb888& rgb){
    return ColorConverter::ToRgb565(rgb);
}

Rgb565 Rgb565::From(const RgbFloat& rgb){
    return ColorConverter::ToRgb565(rgb);
}

Rgb565 Rgb565::FromHsl(float h, float s, float l){
    return ColorConverter::ToRgb565(HslColor(h, s, l));
}

Rgb565 Rgb565::FromRgb888(uint8_t r, uint8_t g, uint8_t b){
    return ColorConverter::ToRgb565(Rgb888(r, g, b));
}

Rgb565 Rgb565::FromRgbFloat(float r, float g, float b){
    return ColorConverter::ToRgb565(RgbFloat(r, g, b));
}

} // namespace 
