#pragma once

// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include <cstdint>

namespace Garbox {

enum class BehaviourId : uint16_t {

    // null event
    Null = 0,

    // generated from config
    Calibration,
    Fermentation,

    // enum size marker
    Count,
};

const char* BehaviourIdToString(BehaviourId id);

} // namespace Garbox