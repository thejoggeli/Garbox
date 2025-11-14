#include "StatusLeds.h"

namespace Garbox {

StatusLeds::StatusLeds(Span<AnimatedLed> leds) : AnimatedLedGroup(leds){
    // nothing to do
} 

AnimatedLed& StatusLeds::getLed(StatusLed index){
    return AnimatedLedGroup::getLed(static_cast<uint8_t>(index));
}

void StatusLeds::setLed(StatusLed index, float brightness){
    AnimatedLedGroup::setLed(static_cast<uint8_t>(index), brightness);
}

void StatusLeds::toggleLed(StatusLed index){
    AnimatedLedGroup::toggleLed(static_cast<uint8_t>(index));
}

} // namespace