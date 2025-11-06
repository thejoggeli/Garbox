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

    void setup(int32_t pin, Mode mode, bool invert = false, bool initivalValue = false);

    void toggle();
    void setValue(bool value);

    bool getValue() const;
    int32_t getPin() const;
    bool isInput() const;
    bool isOutput() const;
    bool hasPullup() const;
    bool hasPulldown() const;

private:
    gpio_num_t mPin = gpio_num_t::GPIO_NUM_NC;
    bool mValue = false;
    bool mInvert = false;
    Mode mMode;
    bool mInitialized = false;

    bool fromPinValue(int value) const;
    uint32_t toPinValue(bool value) const;
};

} // namespace Garbox
