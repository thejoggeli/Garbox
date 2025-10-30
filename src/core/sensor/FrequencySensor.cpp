#include "FrequencySensor.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "global/AppConfig.h"

namespace Garbox {

// static so each class instance can share one per-core lock
static portMUX_TYPE sFrequencySensorMux = portMUX_INITIALIZER_UNLOCKED;

FrequencySensor::FrequencySensor(uint32_t pin, timer_group_t timerGroup, timer_idx_t timerId):
    // init members
    mPin(pin),
    mTimerGroup(timerGroup),
    mTimerId(timerId){
    // nothing to do
}

bool FrequencySensor::init(Config const& config) {

    AssertExit(!mInitialized, "FrequencySensor::init()", "already initialized");
    
    // initialize members
    mTimerFrequencyHz = static_cast<float>(config.timerFrequencyHz);

    // convert to timer ticks
    // ticks = time[s] * freq[Hz] = (stopTimeoutMicros * freqHz) / 1e6
    uint64_t ticks64 = (static_cast<uint64_t>(config.stopTimeoutMicros) * config.timerFrequencyHz) / 1'000'000ULL;
    AssertExit(ticks64 <= 0xFFFFFFFFULL, "FrequencySensor::init()", "stopTimeoutMicros invalid value");
    mStopTimeoutTicks = static_cast<uint32_t>(ticks64);

    // Compute divider from desired frequency (APB = 80 MHz)
    AssertExit((AppConfig::ClockFrequency % config.timerFrequencyHz) == 0, "FrequencySensor::init()", "timerFrequencyHz invalid value");
    uint32_t divider = AppConfig::ClockFrequency / config.timerFrequencyHz;
    AssertExit((divider >= 2) && (divider <= 65536), "FrequencySensor::init()", "timerFrequencyHz invalid value");

    // Configure hardware timer for 1 µs ticks
    esp_err_t err;
    timer_config_t timerConf = {
        .alarm_en = TIMER_ALARM_DIS,
        .counter_en = TIMER_START,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_DIS,
        .divider = static_cast<uint16_t>(divider),
        .clk_src = TIMER_SRC_CLK_APB
    };
    err = timer_init(mTimerGroup, mTimerId, &timerConf);
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "timer_init failed");

    err = timer_set_counter_value(mTimerGroup, mTimerId, 0);
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "timer_set_counter_value failed");

    err = timer_start(mTimerGroup, mTimerId);
    AssertExit(err == ESP_OK, "FrequencySensor::init()", "timer_start failed");

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

    err = gpio_config(&ioConf);
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
    auto* self = static_cast<FrequencySensor*>(arg);
    uint64_t nowTicks64;
    timer_get_counter_value(self->mTimerGroup, self->mTimerId, &nowTicks64);
    self->mLastEdgeTicks = self->mCurrentEdgeTicks;
    self->mCurrentEdgeTicks = static_cast<uint32_t>(nowTicks64);
    self->mHasNewEdge = true;
}

void FrequencySensor::tick(){
    if(!mInitialized){
        AssertDebug(false, "FrequencySensor::tick()", "not initialized");
        return;
    }

    if((mState == State::Idle && !mHasNewEdge) || !mEnabled){
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
            mFrequencyHz = 0;
        }
    }
    else if(mState == State::Running){
        uint64_t nowTicks64;
        timer_get_counter_value(mTimerGroup, mTimerId, &nowTicks64);
        if((nowTicks64 - mCurrentEdgeTicks) > mStopTimeoutTicks){
            mFrequencyHz = 0;
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
        mFrequencyHz = mTimerFrequencyHz / static_cast<float>(deltaTicks);
    }
}

void FrequencySensor::setEnabled(bool enabled) {
    if (!mInitialized){
        AssertDebug(false, "FrequencySensor::setEnabled()", "not initialized");
        return;
    }
    if(enabled == mEnabled){
        return;
    }
    mEnabled = enabled;

    portENTER_CRITICAL(&sFrequencySensorMux);
    if (enabled) {
        gpio_intr_enable(static_cast<gpio_num_t>(mPin));
    } else {
        gpio_intr_disable(static_cast<gpio_num_t>(mPin));
        mHasNewEdge = false;
        mFrequencyHz = 0;
        mState = State::Idle;
    }
    portEXIT_CRITICAL(&sFrequencySensorMux);
}

float FrequencySensor::getFrequencyHz() {
    if(!mInitialized){
        AssertDebug(false, "FrequencySensor::getFrequencyHz()", "not initialized");
        return 0;
    }
    return mFrequencyHz;
}

} // namespace Garbox
