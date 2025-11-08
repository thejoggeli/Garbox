#include "InterruptButton.h"

#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "hal/gpio_hal.h"
#include "soc/gpio_reg.h"

namespace Garbox {

#define GarboxDebugInterruptButton 0

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
    vRawReferenceState = mCurrentRawState;

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
    bool level;
    if(self->mPin < 32){
        level = ((GPIO.in >> self->mPin) & 0x1) != 0;
    }
    else {
        level = ((GPIO.in1.data >> (self->mPin - 32)) & 0x1) != 0;
    }

    // signal changed away from reference
    if(level != self->vRawReferenceState){
        if(!self->vEdgeAwayDetected){
            self->vEdgeAwayDetected = true;
            self->vEdgeAwayMicros = Time::GetMicros();
        }
    }
    // signal changed towards reference
    else {
        self->vEdgeReturnMicros = Time::GetMicros();
        self->vEdgeReturnDetected = true;
    }

    // store new raw state
    self->vNewRawState = level;
}

void InterruptButton::tick(){
    if(!mInitialized){
        TriggerDebug("InterruptButton", "tick() called before init()");
        return;
    }
    
    // handle detected edge
    // critical section
    portENTER_CRITICAL(&mMux);
    if(vEdgeAwayDetected){

        // copy ISR variables
        bool newRawState = vNewRawState;
        bool edgeAwayDetected = vEdgeAwayDetected;
        bool edgeReturnDetected = vEdgeReturnDetected;
        uint32_t edgeAwayMicros = vEdgeAwayMicros;
        uint32_t edgeReturnMicros = vEdgeReturnMicros;

        // set new state for ISR
        vRawReferenceState = newRawState;
        vEdgeAwayDetected = false;
        vEdgeReturnDetected = false;

        // leave critical section
        portEXIT_CRITICAL(&mMux);

        // detect missed pulse
        const bool missedPulse = edgeAwayDetected && edgeReturnDetected && (mCurrentRawState == newRawState);
        if(missedPulse){
            // handle missed pulse
            const uint32_t missedPulseDuration = edgeReturnMicros - edgeAwayMicros;
            const bool missedPulseState = mInvert ? mCurrentRawState : !mCurrentRawState;
            mButton.handleMissedPulse(missedPulseState, missedPulseDuration);
            #if GarboxDebugInterruptButton
                LogDebug("InterruptButton", "detected missed pulse state=%" PRIu32 ", duration=%" PRIu32 "us", missedPulseState, missedPulseDuration);
            #endif
        }

        // apply new input state
        mCurrentRawState = newRawState;
    }
    else {
        portEXIT_CRITICAL(&mMux);
    }

    #if GarboxDebugInterruptButton
        LogDebug("InterruptButton", "state=%" PRIu32, mCurrentRawState);
    #endif
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

void InterruptButton::setPressedToReleasedDelayMicros(uint32_t micros){
    mButton.setPressedToReleasedDelayMicros(micros);
}

void InterruptButton::setReleasedToPressedDelayMicros(uint32_t micros){
    mButton.setReleasedToPressedDelayMicros(micros);
}

void InterruptButton::setPressedHoldTimeMicros(uint32_t micros){
    mButton.setPressedHoldTimeMicros(micros);
}

void InterruptButton::setReleasedHoldTimeMicros(uint32_t micros){
    mButton.setReleasedHoldTimeMicros(micros);
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
