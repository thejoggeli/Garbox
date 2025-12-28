#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class ProfilerId : uint16_t {

    // null event
    Null = 0,

    // system
    MainPeriod,
    MainBusy,

    // tick phases
    HeartbeatTick,
    SensorReadTick,
    UserInputTick,
    LogicTick,
    ActorWriteTick,
    LoggingTick,
    RenderTick,

    // custom
    LedAnimation,
    PiezoPlayer,
    DisplayRender,

    // enum size marker
    Count,
};

const char* ProfilerIdToString(ProfilerId id);

} // namespace Garbox