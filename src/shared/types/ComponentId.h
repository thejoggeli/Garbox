#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentId : uint8_t {
    // special zero type
    Null = 0,

    // behaviour ids
    CalibrationBehaviour,
    FermentationBehaviour,

    // controller ids
    FanController,
    HeartbeatController,
    InputController,
    HeatpadController,
    DisplayController,
    I2cPartsController,

    // special count type
    Count
};

const char* ComponentIdToString(ComponentId id);

} // namespace