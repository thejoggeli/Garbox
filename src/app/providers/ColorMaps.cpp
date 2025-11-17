#include "ColorMaps.h"

namespace Garbox {

using ColorMapEntry = ColorMap::Entry;

const ColorMap& ColorMaps::GetRedBlue(){
    static const ColorMapEntry entries[] = {
        RgbFloat(1, 0, 0),
        RgbFloat(0, 0, 1),
    };
    static const Span span(entries);
    static const ColorMap instance(span);
    return instance;   
}

const ColorMap& ColorMaps::GetTestRBR_Uniform(){
    static const ColorMapEntry entries[] = {
        RgbFloat(1, 0, 0),
        RgbFloat(0, 0, 1),
        RgbFloat(1, 0, 0),
    };
    static const Span span(entries);
    static const ColorMap instance(span);
    return instance;   
}

const ColorMap& ColorMaps::GetTestRBR_NonUniform(){
    static const ColorMapEntry entries[] = {
        { 0.0f, RgbFloat(1, 0, 0) },
        { 0.2f, RgbFloat(0, 0, 1) },
        { 1.0f, RgbFloat(1, 0, 0) },
    };
    static const Span span(entries);
    static const ColorMap instance(span);
    return instance;   
}

const ColorMap& ColorMaps::GetViridis(){
    static const ColorMapEntry entries[] = {
        RgbFloat::FromHex(0xfde725),
        RgbFloat::FromHex(0x5ec962),
        RgbFloat::FromHex(0x21918c),
        RgbFloat::FromHex(0x3b528b),
        RgbFloat::FromHex(0x440154),
    };
    static const Span span(entries);
    static const ColorMap instance(span);
    return instance;   
}

} // namespace