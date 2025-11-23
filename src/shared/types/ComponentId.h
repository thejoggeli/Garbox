// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentId : uint16_t {

    // null event
    Null = 0,

    // default component ids
    Runtime,

    // generated from config
    DisplayController,
    FanController,
    HeartbeatController,
    HeatpadController,
    InputController,
    I2cPartsController,
    CalibrationBehaviour,
    FermentationBehaviour,

    // enum size marker
    Count,
};

const char* ComponentIdToString(ComponentId id);

} // namespace Garbox