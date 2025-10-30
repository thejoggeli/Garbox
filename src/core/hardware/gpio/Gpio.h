#pragma once

#include "driver/gpio.h"
#include <cstdint>

namespace Garbox {

class Gpio {
public:
    enum class Mode : uint8_t {
        Uninitialized,
        Input,
        Output,
        InputPullup,
        InputPulldown,
        OutputOpenDrain
    };

    Gpio();

    void setup(uint32_t pin, Mode mode, bool invert = false);
    void setValue(bool value);
    bool getValue() const;
    void toggle();

private:
    uint32_t mPin = 0;
    bool mState = false;
    bool mInvert = false;
    Mode mMode = Mode::Uninitialized;
};

} // namespace Garbox
