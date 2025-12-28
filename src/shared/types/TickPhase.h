#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class TickPhase : uint16_t {
    Null = 0,
    Heartbeat,
    SensorRead,
    UserInput,
    Logic,
    ActorWrite,
    Logging,
    Render,
    Count,
};

const char* TickPhaseToString(TickPhase phase);

} // namespace Garbox