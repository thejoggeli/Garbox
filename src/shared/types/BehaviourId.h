// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>

namespace Garbox {

enum class BehaviourId : uint16_t {

    // null event
    Null = 0,

    Calibration,
    Fermentation,

    // enum size marker
    Count,
};

const char* BehaviourIdToString(BehaviourId id);

} // namespace Garbox