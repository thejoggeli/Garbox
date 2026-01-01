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

const ColorMap& ColorMaps::GetCoolWarm(){
    static const ColorMapEntry entries[] = {
        { 0.00f, RgbFloat::FromHex(0x1f31fa) }, // blue,
        { 0.30f, RgbFloat::FromHex(0xaaaaff) }, // light blue
        { 0.40f, RgbFloat::FromHex(0x40FF40) }, // green
        { 0.60f, RgbFloat::FromHex(0x40FF40) }, // green
        { 0.70f, RgbFloat::FromHex(0xffaaaa) }, // light red
        { 1.00f, RgbFloat::FromHex(0xdb2503) }, // red
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