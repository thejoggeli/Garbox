#pragma once

#include <cstdint>

namespace Garbox {

enum class SystemState : uint8_t {
    Reset = 0,
    StartupSequence,
    Garbox,
    Calibration
};

}