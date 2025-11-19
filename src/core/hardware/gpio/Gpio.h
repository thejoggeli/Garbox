#pragma once

#include "driver/gpio.h"
#include <cstdint>

namespace Garbox {

// Conversion table for level without and with inversion: logical <=> physical
// ------------------------------
// | invert | logical | physical| 
// |----------------------------|
// |   0    |    1    |    1    |
// |   0    |    0    |    0    |
// |   1    |    1    |    0    |
// |   1    |    0    |    1    |
// ------------------------------
class Gpio {
public:

    using InterruptHandler = void(*)(void*);

    enum class Mode : uint8_t {
        Disable = 0,
        Input,
        Output,
        OutputOpenDrain,
    };

    enum class Pull : uint8_t {
        Disable = 0,
        Up,
        Down,
    };

    enum class Interrupt : uint8_t {
        Disable = 0,
        PositiveEdge,
        NegativeEdge,
        AnyEdge,
        LowLevel,
        HighLevel,
    };

    struct Config {
        Mode mode = Mode::Disable;
        Pull pull = Pull::Disable;
        Interrupt interrupt = Interrupt::Disable;
        bool invert = false; // if enabled: (logical_level) = !(physical_level)
    };

    Gpio(int32_t pin);

    // Configure the GPIO pin with a specific mode, inversion option, and initial output level (logical) 
    // The initial logical level can be set with initialLevel. 
    // It is written to (or read from) the pin upon initialization.
    void init(const Config& config, bool initialLevel = false);

    // configuration
    void applyConfig(const Config& config);
    Config getCurrentConfig() const;

    // Toggle the output level of the pin (only valid for output modes)
    void toggle();

    // Read and write operations for the pin, with and without inversion
    void writeLevel(bool level);    // write logical level
    void writeLevelRaw(bool level); // write raw level
    bool readLevel() const;         // read logical level (uses cached value if in output mode)
    bool readLevelRaw() const;      // read raw level (uses cached value if in output mode)

    // interrupt handling
    bool addInterruptHandler(InterruptHandler handler, void* user);
    bool setInterruptEnabled(bool enabled);
    
    // Getters for pin number, mode, and configuration flags
    int32_t getPinNumber() const;
    bool isInverted() const;
    bool isInput() const;
    bool isOutput() const;
    bool hasPullup() const;
    bool hasPulldown() const;
    bool isInitialized() const;

    // disallow copy and move 
    Gpio(const Gpio&) = delete;
    Gpio& operator=(const Gpio&) = delete;
    Gpio(Gpio&&) = delete;
    Gpio& operator=(Gpio&&) = delete;

private:

    const gpio_num_t mPin = gpio_num_t::GPIO_NUM_NC;
    bool mLogicalLevel = false; // current (logical) pin level 

    Config mConfig;
    bool mInitialized = false;

    // Convert raw GPIO level to logical value (applies inversion if needed)
    bool rawToLogical(int rawLevel) const;
    
    // Convert logical value to raw GPIO level (applies inversion if needed)
    uint32_t logicalToRaw(bool logicalLevel) const;

};

} // namespace Garbox
