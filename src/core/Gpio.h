#pragma once
#include <Arduino.h>
#include <cstdint>

namespace Garbox {

class Gpio {
public:

    enum class Mode : uint8_t {
        Input           = INPUT,
        Output          = OUTPUT,
        InputPullup     = INPUT_PULLUP,
        InputPulldown   = INPUT_PULLDOWN,
        OutputOpenDrain = OUTPUT_OPEN_DRAIN
    };

    explicit Gpio(uint32_t pin);

    // setup
    void setMode(Mode mode);

    // digital I/O
    void digitalWrite(bool value);
    bool digitalRead() const;
    void toggle();
    bool getDigitalState() const;

    // analog I/O
    int analogRead() const;
    void analogWrite(uint32_t value);  // value range 0–255 for PWM

private:

    uint32_t mPin;
    bool mState = false; // tracks last written digital state

};

}  // namespace Garbox