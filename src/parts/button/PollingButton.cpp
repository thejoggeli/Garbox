#include "PollingButton.h"

#include "assert/Assert.h"
#include "driver/gpio.h"
#include "esp_err.h"

namespace Garbox {

#define GarboxDebugInterruptButton 0

PollingButton::PollingButton(Gpio& gpio):
    // initialize members
    mGpio(gpio),
    mButton(){
    // constructor body
}

void PollingButton::init(){
    AssertExit(!mInitialized, "PollingButton", "already initialized");
    AssertExit(mGpio.isInput(), "PollingButton", "gpio must be configured as input");

    // get gpio pin number
    mPin = mGpio.getPinNumber();
    AssertExit(mPin >= 0, "PollingButton", "invalid pin number");

    mInitialized = true;
}

void PollingButton::tick(){
    if(!mInitialized){
        TriggerDebug("PollingButton", "tick() called before init()");
        return;
    }
    
    mButton.setPhysicalButtonState(mGpio.readLevel());
    mButton.tick();
}

bool PollingButton::isPressed() const {
    return mButton.isPressed();
}

bool PollingButton::isReleased() const {
    return mButton.isReleased();
}

bool PollingButton::isLongPressed() const {
    return mButton.isLongPressed();
}

void PollingButton::setStateChangedCallback(StateChangedCallback callback){
    mButton.setStateChangedCallback(callback);
}

void PollingButton::setHoldCallback(HoldCallback callback){
    mButton.setHoldCallback(callback);
}

void PollingButton::setUserData(void* userData){
    mButton.setUserData(userData);
}

void PollingButton::setPressedToReleasedDelayMicros(uint32_t micros){
    mButton.setPressedToReleasedDelayMicros(micros);
}

void PollingButton::setReleasedToPressedDelayMicros(uint32_t micros){
    mButton.setReleasedToPressedDelayMicros(micros);
}

void PollingButton::setPressedHoldTimeMicros(uint32_t micros){
    mButton.setPressedHoldTimeMicros(micros);
}

void PollingButton::setReleasedHoldTimeMicros(uint32_t micros){
    mButton.setReleasedHoldTimeMicros(micros);
}

void PollingButton::setLongPressMicros(uint32_t delayMicros){
    mButton.setLongPressMicros(delayMicros);
}

void PollingButton::setInitialHoldDelayMicros(uint32_t delayMicros){
    mButton.setInitialHoldDelayMicros(delayMicros);
}

void PollingButton::setRepeatHoldDelayMicros(uint32_t delayMicros){
    mButton.setRepeatHoldDelayMicros(delayMicros);
}

} // namespace Garbox
