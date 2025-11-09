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

    // Configure the GPIO pin with a specific mode, inversion option, and initial output level (logical) 
    void init(int32_t pin, Mode mode, bool invert = false, bool initialLevel = false);

    // Toggle the output level of the pin (only valid for output modes)
    void toggle();

    // Read and write operations for the pin, with and without inversion
    void writeLevel(bool level);    // write logical level
    void writeLevelRaw(bool level); // write raw level
    bool readLevel() const;         // read logical level (uses cached value if in output mode)
    bool readLevelRaw() const;      // read raw level (uses cached value if in output mode)
    
    // Getters for pin number, mode, and configuration flags
    int32_t getPin() const;
    bool isInverted() const;
    bool isInput() const;
    bool isOutput() const;
    bool hasPullup() const;
    bool hasPulldown() const;

    // Disallow copy and move 
    Gpio(const Gpio&) = delete;
    Gpio& operator=(const Gpio&) = delete;
    Gpio(Gpio&&) = delete;
    Gpio& operator=(Gpio&&) = delete;

private:
    gpio_num_t mPin = gpio_num_t::GPIO_NUM_NC;
    bool mLogicalLevel = false; // current (logical) pin level 
    bool mInvert = false;
    Mode mMode;
    bool mInitialized = false;

    // Convert raw GPIO level to logical value (applies inversion if needed)
    bool rawToLogical(int rawLevel) const;
    
    // Convert logical value to raw GPIO level (applies inversion if needed)
    uint32_t logicalToRaw(bool logicalLevel) const;
};

} // namespace Garbox
