// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentId : uint16_t {

    // null event
    Null = 0,

    FanController,
    HeatpadController,
    HeartbeatController,
    FermentationBehaviour,
    I2cPartsController,
    DisplayController,
    InputController,
    CalibrationBehaviour,

    // enum size marker
    Count,
};

const char* ComponentIdToString(ComponentId id);

} // namespace Garbox