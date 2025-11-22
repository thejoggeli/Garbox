// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentId : uint16_t {

    // null event
    Null = 0,

    CalibrationBehaviour,
    DisplayController,
    FanController,
    FermentationBehaviour,
    HeartbeatController,
    HeatpadController,
    I2cPartsController,
    InputController,

    // enum size marker
    Count,
};

const char* ComponentIdToString(ComponentId id);

} // namespace Garbox