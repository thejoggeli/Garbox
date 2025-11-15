#pragma once

#include <cstdint>

namespace Garbox {

enum class ButtonState : uint8_t {
    Released,
    Pressed,
    PressedLong,
    Count
};

const char* ButtonStateToString(ButtonState state);

} // namespace Garbox
