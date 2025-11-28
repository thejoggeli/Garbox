#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class ScreenId : uint16_t {

    // null event
    Null = 0,

    // generated from config
    Main,
    Debug,

    // enum size marker
    Count,
};

const char* ScreenIdToString(ScreenId id);

} // namespace Garbox