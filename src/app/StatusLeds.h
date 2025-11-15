#pragma once

#include "parts/led/AnimatedLedGroup.h"

namespace Garbox {

enum class StatusLedId : uint8_t {
    Heartbeat = 0, 
    Custom1 = 1,
    Custom2 = 2,
    Error = 3,
};

class StatusLeds : public AnimatedLedGroup {
public:

    StatusLeds(Span<AnimatedLed> leds);

    AnimatedLed& getLed(StatusLedId index);
    void setLed(StatusLedId index, float brightness);
    void toggleLed(StatusLedId Index);

};

} // namespace