#pragma once

#include <cstdint>

namespace Garbox {

enum class FermentationState : uint8_t {
    Null = 0,
    Reset,
    Ready, // awainting input
    Regulating,   // PID control active
    InvalidInput, // Not all input data is available => heater forced off
    OverTemperature,  // Temperature too high => heater forced off
    Cooldown,
    Count 
};

const char* FermentationStateToString(FermentationState id);

} // namespace
