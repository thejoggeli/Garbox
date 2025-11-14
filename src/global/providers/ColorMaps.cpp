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

} // namespace