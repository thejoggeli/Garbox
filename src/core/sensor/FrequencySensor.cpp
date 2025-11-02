#include "FrequencySensor.h"

#include "assert/Assert.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "core/log/Log.h"

namespace Garbox {

// static so each class instance can share one per-core lock
static portMUX_TYPE sFrequencySensorMux = portMUX_INITIALIZER_UNLOCKED;

FrequencySensor::FrequencySensor(uint32_t pin, Timer& timer):
    // init members
    mPin(pin),
    mTimer(timer){
    // nothing to do
}

bool FrequencySensor::init(Config const& config) {

    AssertExit(!mInitialized, "FrequencySensor::init()", "already initialized");
    
    // initialize members
    mTimerFrequencyHz = static_cast<float>(mTimer.getFrequencyHz());
    AssertExit(mTimerFrequencyHz > 0, "FrequencySensor:init()", "invalid timer frequency");

    // initialize stop timeout
    if(config.stopTimeoutMicros == 0){
        mStopTimeoutTicks = 0; // no timeout
    }
    else {
        // ticks = time[s] * freq[Hz] = (stopTimeoutMicros/1e6) * timerFrequencyHz
        float ticksFloat = static_cast<float>(config.stopTimeoutMicros) * 1e-6f * mTimerFrequencyHz;
        if(ticksFloat < 0 || ticksFloat > static_cast<float>(1'000'000'000)){
            AssertExit(false, "FrequencySensor::init()", "stopTimeoutMicros invalid value");
        }
        mStopTimeoutTicks = static_cast<uint32_t>(ticksFloat);
    }

    // Configure GPIO input
    gpio_config_t ioConf = {};
    ioConf.intr_type = GPIO_INTR_POSEDGE;
    ioConf.mode = GPIO_MODE_INPUT;
    ioConf.pin_bit_mask = 1ULL << mPin;

    switch(config.pinMode){
        case PinMode::Floating:
            ioConf.pull_up_en = GPIO_PULLUP_DISABLE;
            ioConf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Pullup:
            ioConf.pull_up_en = GPIO_PULLUP_ENABLE;
            ioConf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Pulldown:
            ioConf.pull_up_en = GPIO_PULLUP_DISABLE;
            ioConf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        default:
            AssertExit(false, "FrequencySensor::init()", "invalid pinMode");
            return false;
    }

    esp_err_t err = gpio_config(&ioConf);
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "gpio_config failed");

    // Install ISR service once globally (safe to call multiple times)
    static bool isrServiceInstalled = false;
    if (!isrServiceInstalled) {
        err = gpio_install_isr_service(0);
        AssertExit(err == ESP_OK, "FrequencySensor::init()", "gpio_install_isr_service failed");
        isrServiceInstalled = true;
    }

    // Register per-instance ISR (context pointer passed to handler)
    portENTER_CRITICAL(&sFrequencySensorMux);
    err = gpio_isr_handler_add(static_cast<gpio_num_t>(mPin), isrHandler, this);
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "gpio_isr_handler_add failed");

    // Disable interrupt by default
    err = gpio_intr_disable(static_cast<gpio_num_t>(mPin));
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "gpio_intr_disable failed");
    portEXIT_CRITICAL(&sFrequencySensorMux);

    // finish initialization
    mInitialized = true;
    return true;
}

void IRAM_ATTR FrequencySensor::isrHandler(void* arg) {
    FrequencySensor* self = static_cast<FrequencySensor*>(arg);
    uint64_t nowTicks64 = self->mTimer.getValueFromIsr();
    self->mLastEdgeTicks = self->mCurrentEdgeTicks;
    self->mCurrentEdgeTicks = static_cast<uint32_t>(nowTicks64);
    self->mHasNewEdge = true;
}

void FrequencySensor::tick(){
    if(!mInitialized){
        AssertDebug(false, "FrequencySensor::tick()", "not initialized");
        return;
    }

    if((mState == State::Disabled) || (mState == State::Idle && !mHasNewEdge)){
        return;
    }

    bool updateFrequency = false;
    uint32_t deltaTicks = 0;

    // disable edge detection briefly
    portENTER_CRITICAL(&sFrequencySensorMux);

    // update state
    if(mState == State::Idle){
        if(mHasNewEdge){
            mState = State::Running;
            mHasNewEdge = false;
            mLastEdgeTicks = mCurrentEdgeTicks;
            mMeasuredFrequencyHz = 0;
        }
    }
    else if(mState == State::Running){
        uint32_t nowTicks = static_cast<uint32_t>(mTimer.getValue());
        if((mStopTimeoutTicks > 0) && ((nowTicks - mCurrentEdgeTicks) > mStopTimeoutTicks)){
            mMeasuredFrequencyHz = 0;
            mState = State::Idle;
        }
        else if(mHasNewEdge){
            deltaTicks = mCurrentEdgeTicks - mLastEdgeTicks;
            updateFrequency = true;
            mHasNewEdge = false;
        }
    }

    // re-enable edge detection 
    portEXIT_CRITICAL(&sFrequencySensorMux);

    // update frequency
    if(updateFrequency && (deltaTicks > 0)){
        mMeasuredFrequencyHz = mTimerFrequencyHz / static_cast<float>(deltaTicks);
    }
}

void FrequencySensor::setEnabled(bool enabled) {
    if (!mInitialized){
        AssertDebug(false, "FrequencySensor::setEnabled()", "not initialized");
        return;
    }

    if(enabled == isEnabled()){
        return;
    }

    portENTER_CRITICAL(&sFrequencySensorMux);
    if (enabled) {
        gpio_intr_enable(static_cast<gpio_num_t>(mPin));
        mState = State::Idle;
    } else {
        gpio_intr_disable(static_cast<gpio_num_t>(mPin));
        mHasNewEdge = false;
        mMeasuredFrequencyHz = 0;
        mState = State::Disabled;
    }
    portEXIT_CRITICAL(&sFrequencySensorMux);
}

bool FrequencySensor::isEnabled(){
    if(mState == State::Disabled){
        return false;
    }
    return true;
}

float FrequencySensor::getFrequencyHz() {
    if(!mInitialized){
        AssertDebug(false, "FrequencySensor::getFrequencyHz()", "not initialized");
        return 0;
    }
    return mMeasuredFrequencyHz;
}

} // namespace Garbox
