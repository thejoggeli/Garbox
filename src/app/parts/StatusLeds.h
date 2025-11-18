#pragma once

#include "app/types/StatusLedId.h"
#include "modules/parts/led/AnimatedLedGroup.h"

namespace Garbox {


class StatusLeds : public AnimatedLedGroup {
public:

    StatusLeds(Span<AnimatedLed> leds);

    AnimatedLed& getLed(StatusLedId index);
    void setLed(StatusLedId index, float brightness);
    void toggleLed(StatusLedId Index);

};

} // namespace
