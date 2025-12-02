#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class ComponentId : uint16_t {

    // null event
    Null = 0,

    // default component ids
    Runtime,
    Replay,

    // generated from config
    DisplayController,
    DevtoolsController,
    FanController,
    HeartbeatController,
    HeatpadController,
    InputController,
    I2cPartsController,
    CalibrationBehaviour,
    FermentationBehaviour,
    MainScreen,
    EventLogScreen,
    DebugScreen,

    // enum size marker
    Count,
};

const char* ComponentIdToString(ComponentId id);

} // namespace Garbox