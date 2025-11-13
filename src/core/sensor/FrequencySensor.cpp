#include "FrequencySensor.h"

#include "assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/log/Log.h"

namespace Garbox {

FrequencySensor::FrequencySensor(Gpio& gpio, Timer& timer):
    // init members
    mGpio(gpio),
    mTimer(timer){
    // nothing to do
}

void FrequencySensor::init(Config const& config) {

    AssertExit(!mInitialized, "FrequencySensor", "already initialized");
    
    // initialize members
    mPin = mGpio.getPinNumber();
    mTimerFrequencyHz = static_cast<float>(mTimer.getFrequencyHz());
    AssertExit(mTimerFrequencyHz > 0, "FrequencySensor", "invalid timer frequency");

    // initialize stop timeout
    if(config.stopTimeoutMicros == 0){
        mStopTimeoutTicks = 0; // no timeout
    }
    else {
        // ticks = time[s] * freq[Hz] = (stopTimeoutMicros/1e6) * timerFrequencyHz
        float ticksFloat = static_cast<float>(config.stopTimeoutMicros) * 1e-6f * mTimerFrequencyHz;
        if(ticksFloat < 0 || ticksFloat > static_cast<float>(1'000'000'000)){
            TriggerExit("FrequencySensor", "stopTimeoutMicros invalid value");
        }
        mStopTimeoutTicks = static_cast<uint32_t>(ticksFloat);
    }

    // configure gpio
    Gpio::Config gpioConfig = mGpio.getCurrentConfig();
    gpioConfig.mode = Gpio::Mode::Input;
    gpioConfig.interrupt = Gpio::Interrupt::PositiveEdge;
    mGpio.applyConfig(gpioConfig);

    // add ISR handler to gpio
    mGpio.addInterruptHandler(isrHandler, this);
    
    // Register per-instance ISR (context pointer passed to handler)
    portENTER_CRITICAL(&mFrequencySensorMux);
    if(!mGpio.addInterruptHandler(isrHandler, this)){
        TriggerExit("FrequencySensor", "add interrupt handler failed");
    }

    // Disable interrupt by default
    if(!mGpio.setInterruptEnabled(false)){
        TriggerExit("FrequencySensor", "disable interrupt failed");
    }
    portEXIT_CRITICAL(&mFrequencySensorMux);

    // finish initialization
    mInitialized = true;
}

void IRAM_ATTR FrequencySensor::isrHandler(void* arg) {
    FrequencySensor* self = static_cast<FrequencySensor*>(arg);
    uint64_t nowTicks64 = self->mTimer.getValueFromIsr();
    self->vLastEdgeTicks = self->vCurrentEdgeTicks;
    self->vCurrentEdgeTicks = static_cast<uint32_t>(nowTicks64);
    self->vHasNewEdge = true;
}

void FrequencySensor::tick(){
    if(!mInitialized){
        TriggerDebug("FrequencySensor", "not initialized");
        return;
    }

    if((mState == State::Disabled) || (mState == State::Idle && !vHasNewEdge)){
        return;
    }

    bool updateFrequency = false;
    uint32_t deltaTicks = 0;

    // disable edge detection briefly
    portENTER_CRITICAL(&mFrequencySensorMux);

    // update state
    if(mState == State::Idle){
        if(vHasNewEdge){
            mState = State::Running;
            vHasNewEdge = false;
            vLastEdgeTicks = vCurrentEdgeTicks;
            mMeasuredFrequencyHz = 0;
        }
    }
    else if(mState == State::Running){
        uint32_t nowTicks = static_cast<uint32_t>(mTimer.getValue());
        if((mStopTimeoutTicks > 0) && ((nowTicks - vCurrentEdgeTicks) > mStopTimeoutTicks)){
            mMeasuredFrequencyHz = 0;
            mState = State::Idle;
        }
        else if(vHasNewEdge){
            deltaTicks = vCurrentEdgeTicks - vLastEdgeTicks;
            updateFrequency = true;
            vHasNewEdge = false;
        }
    }

    // re-enable edge detection 
    portEXIT_CRITICAL(&mFrequencySensorMux);

    // update frequency
    if(updateFrequency && (deltaTicks > 0)){
        mMeasuredFrequencyHz = mTimerFrequencyHz / static_cast<float>(deltaTicks);
    }
}

void FrequencySensor::setEnabled(bool enabled) {
    if (!mInitialized){
        TriggerDebug("FrequencySensor", "not initialized");
        return;
    }

    if(enabled == isEnabled()){
        return;
    }

    portENTER_CRITICAL(&mFrequencySensorMux);
    if (enabled) {
        mGpio.setInterruptEnabled(true);
        mState = State::Idle;
    } else {
        mGpio.setInterruptEnabled(false);
        vHasNewEdge = false;
        mMeasuredFrequencyHz = 0;
        mState = State::Disabled;
    }
    portEXIT_CRITICAL(&mFrequencySensorMux);
}

bool FrequencySensor::isEnabled(){
    if(mState == State::Disabled){
        return false;
    }
    return true;
}

float FrequencySensor::getFrequencyHz() {
    if(!mInitialized){
        TriggerDebug("FrequencySensor", "not initialized");
        return 0;
    }
    return mMeasuredFrequencyHz;
}

} // namespace Garbox
