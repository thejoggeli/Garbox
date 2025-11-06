#include "DimmingLed.h"

#include "global/ledc/LedcInstances.h"

namespace Garbox {

DimmingLed::DimmingLed(LedcChannel& ledcChannel) : mLedcChannel(ledcChannel){
    // constructor body
}

void DimmingLed::transitionTo(float brightness){

}

void DimmingLed::setEasingFunction(const EasingFunction& easingFunction){
    mEasingFunction = &easingFunction;
}

} // namespace