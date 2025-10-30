#pragma once

#include <cstdint>
#include "driver/pcnt.h"

namespace Garbox {

class PulseCounter {
public:

    enum class PinMode : uint8_t {
        Pullup,   // internal pull-up enabled
        Pulldown, // internal pull-down enabled
        Floating, // no internal pull
    };

    struct Config {
        PinMode pinMode = PinMode::Floating;
        int16_t minCount = -32768;
        int16_t maxCount = 32767;
        uint32_t filterCycles = 100;
    };

    explicit PulseCounter(pcnt_unit_t unit, uint32_t pin);

    bool init(Config const& config);
    bool start();
    
    int32_t getCount() const;
    int32_t getAndClearCount();
    void clearCount();

private:

    bool mInitialized = false;
    
    pcnt_unit_t mUnit;
    uint32_t mPin;

};

} // namespace Garbox
