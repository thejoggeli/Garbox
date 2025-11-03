#pragma once

#include "driver/gpio.h"
#include <cstdint>

namespace Garbox {

class Gpio {
public:

    enum class Mode : uint8_t {
        Input,
        Output,
        InputPullup,
        InputPulldown,
        OutputOpenDrain
    };

    Gpio();

    void setup(uint32_t pin, Mode mode, bool initivalValue = false, bool invert = false);
    void setValue(bool value);
    bool getValue() const;
    void toggle();

private:
    gpio_num_t mPin = gpio_num_t::GPIO_NUM_NC;
    bool mValue = false;
    bool mInvert = false;
    Mode mMode;
    bool mInitialized = false;

    bool fromPinValue(int value) const;
    int toPinValue(bool value) const;
};

} // namespace Garbox
