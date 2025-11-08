#include "InterruptButton.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "hal/gpio_hal.h"
#include "soc/gpio_reg.h"

namespace Garbox {

InterruptButton::InterruptButton(Gpio& gpio):
    // initialize members
    mGpio(gpio),
    mButton(){
    // constructor body
}

InterruptButton::~InterruptButton(){
    TriggerExit("InterruptButton", "heap using classes must not be deconstructed");
}

void InterruptButton::init(){
    AssertExit(!mInitialized, "InterruptButton", "already initialized");
    AssertExit(mGpio.isInput(), "InterruptButton", "gpio must be configured as input");

    // get gpio pin number
    mPin = mGpio.getPin();
    AssertExit(mPin >= 0, "InterruptButton", "invalid pin number");

    // set invert flag
    mInvert = mGpio.isInverted();

    // initial state
    mCurrentRawState = mGpio.getRawValue();
    vNewRawState = mCurrentRawState;
    vEdgeDetected = false;

    // initialize internal Button
    mButton.init();

    // configure GPIO interrupt
    gpio_set_intr_type(static_cast<gpio_num_t>(mPin), GPIO_INTR_ANYEDGE);

    // attach ISR handler
    if(gpio_isr_handler_add(static_cast<gpio_num_t>(mPin), isrHandler, this) != ESP_OK){
        TriggerDebug("InterruptButton", "gpio_isr_handler_add failed");
    }

    mInitialized = true;
}

void IRAM_ATTR InterruptButton::isrHandler(void* arg){
    InterruptButton* self = static_cast<InterruptButton*>(arg);
    uint32_t level = 0;
    if(self->mPin < 32){
        level = (GPIO.in >> self->mPin) & 0x1;
    }
    else {
        level = (GPIO.in1.data >> (self->mPin - 32)) & 0x1;
    }
    self->vNewRawState = (level != 0);
    self->vEdgeDetected = true;
    self->vLastEdgeTimeMicros = self->vCurrentEdgeTimeMicros;
    self->vCurrentEdgeTimeMicros = Time::GetMicros();
}

void InterruptButton::tick(){
    if(!mInitialized){
        TriggerDebug("InterruptButton", "tick() called before init()");
        return;
    }
    
    // read ISR variables
    if(vEdgeDetected){

        // variables to be copied
        bool newRawState;
        bool edgeDetected;
        uint32_t lastEdgeTimeMicros;
        uint32_t currentEdgeTimeMicros;

        // copy variables
        portENTER_CRITICAL(&mMux);
        newRawState = vNewRawState;
        edgeDetected = vEdgeDetected;
        lastEdgeTimeMicros = vLastEdgeTimeMicros;
        currentEdgeTimeMicros = vCurrentEdgeTimeMicros;
        vEdgeDetected = false;
        portEXIT_CRITICAL(&mMux);

        // detect missed pulse
        const bool inputStateSame = (mCurrentRawState == newRawState);
        const bool missedPulse = edgeDetected && inputStateSame;
        if(missedPulse){
            // handle missed pulse
            const uint32_t missedPulseDuration = currentEdgeTimeMicros - lastEdgeTimeMicros;
            const bool missedPulseState = mInvert ? mCurrentRawState : !mCurrentRawState;
            mButton.handleMissedPulse(missedPulseState, missedPulseDuration);
        }

        // apply new input state
        mCurrentRawState = newRawState;
    }
    
    mButton.tick(mInvert ? !mCurrentRawState : mCurrentRawState);
}

bool InterruptButton::isPressed() const {
    return mButton.isPressed();
}

bool InterruptButton::isReleased() const {
    return mButton.isReleased();
}

bool InterruptButton::isLongPressed() const {
    return mButton.isLongPressed();
}

void InterruptButton::setStateChangedCallback(Button::StateChangedCallback callback){
    mButton.setStateChangedCallback(callback);
}

void InterruptButton::setHoldCallback(Button::HoldCallback callback){
    mButton.setHoldCallback(callback);
}

void InterruptButton::setUserData(void* userData){
    mButton.setUserData(userData);
}

void InterruptButton::setPressDebounceMicros(uint32_t debounceMicros){
    mButton.setPressDebounceMicros(debounceMicros);
}

void InterruptButton::setReleaseDebounceMicros(uint32_t debounceMicros){
    mButton.setReleaseDebounceMicros(debounceMicros);
}

void InterruptButton::setLongPressMicros(uint32_t delayMicros){
    mButton.setLongPressMicros(delayMicros);
}

void InterruptButton::setInitialHoldDelayMicros(uint32_t delayMicros){
    mButton.setInitialHoldDelayMicros(delayMicros);
}

void InterruptButton::setRepeatHoldDelayMicros(uint32_t delayMicros){
    mButton.setRepeatHoldDelayMicros(delayMicros);
}

} // namespace Garbox
