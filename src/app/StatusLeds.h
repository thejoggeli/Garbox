#pragma once

#include "parts/led/AnimatedLedGroup.h"
#include "global/config/StatusLedsConfig.h"

namespace Garbox {

class StatusLeds : public AnimatedLedGroup {
public:

    StatusLeds(Span<AnimatedLed> leds);

    AnimatedLed& getLed(StatusLed index);
    void setLed(StatusLed index, float brightness);
    void toggleLed(StatusLed Index);

};

} // namespace