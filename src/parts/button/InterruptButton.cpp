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

void InterruptButton::init(){
    AssertExit(!mInitialized, "InterruptButton", "already initialized");
    AssertExit(mGpio.isInput(), "InterruptButton", "gpio must be configured as input");

    // get gpio pin number
    mPin = mGpio.getPinNumber();
    AssertExit(mPin >= 0, "InterruptButton", "invalid pin number");

    // set invert flag
    mInvert = mGpio.isInverted();

    // initial state
    const bool initialLevelRaw = mGpio.readLevelRaw();
    mCurrentLevelRaw = initialLevelRaw; 
    vIsrReferenceLevelRaw = mCurrentLevelRaw;

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
    if(level != self->vIsrReferenceLevelRaw){
        if(!self->vIsrPulseStartDetected){
            self->vIsrPulseStartDetected = true;
            self->vIsrPulseStartCpuCycles = Time::GetCpuCycles();
        }
    }
    // signal changed towards reference
    else {
        self->vIsrPulseEndCpuCycles = Time::GetCpuCycles();
        self->vIsrPulseEndDetected = true;
    }

}

void InterruptButton::tick(){
    if(!mInitialized){
        TriggerDebug("InterruptButton", "tick() called before init()");
        return;
    }
    
    // handle detected edge
    // critical section
    portENTER_CRITICAL(&mMux);
    if(vIsrPulseStartDetected){

        // copy ISR variables
        bool     pulseStartDetected  = vIsrPulseStartDetected;
        bool     pulseEndDetected    = vIsrPulseEndDetected;
        uint32_t pulseStartCpuCycles = vIsrPulseStartCpuCycles;
        uint32_t pulseEndCpuCycles   = vIsrPulseEndCpuCycles;

        // reset ISR pulse state
        vIsrPulseStartDetected = false;
        vIsrPulseEndDetected = false;

        // read gpio level
        bool newLevelRaw = mGpio.readLevelRaw(); 
        vIsrReferenceLevelRaw = newLevelRaw;

        // leave critical section
        portEXIT_CRITICAL(&mMux);

        // detect missed pulse
        const bool missedPulse = pulseStartDetected && pulseEndDetected && (mCurrentLevelRaw == newLevelRaw);
        if(missedPulse){
            // handle missed pulse
            const uint32_t missedPulseDuration = Time::CpuCyclesToMicros(pulseEndCpuCycles - pulseStartCpuCycles);
            mButton.handleMissedPulse(missedPulseDuration);
            #if GarboxDebugInterruptButton
                LogDebug("InterruptButton", "detected missed pulse state=%" PRIu32 ", duration=%" PRIu32 "us", missedPulseState, missedPulseDuration);
            #endif
        }

        // apply new input state
        mCurrentLevelRaw = newLevelRaw;
    }
    else {
        bool newLevelRaw = mGpio.readLevelRaw(); 
        vIsrReferenceLevelRaw = newLevelRaw;
        mCurrentLevelRaw = newLevelRaw;
        portEXIT_CRITICAL(&mMux);
    }

    #if GarboxDebugInterruptButton
        LogDebug("InterruptButton", "state=%" PRIu32, mCurrentRawState);
    #endif
    
    bool pressed = mInvert ? !mCurrentLevelRaw : mCurrentLevelRaw;
    mButton.setPhysicalButtonState(pressed);
    mButton.tick();
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

void InterruptButton::setStateChangedCallback(ButtonIfc::StateChangedCallback callback){
    mButton.setStateChangedCallback(callback);
}

void InterruptButton::setHoldCallback(ButtonIfc::HoldCallback callback){
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
