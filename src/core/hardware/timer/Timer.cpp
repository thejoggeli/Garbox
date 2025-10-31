#include "Timer.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"

namespace Garbox {

Timer::Timer(timer_group_t group, timer_idx_t index, uint32_t frequencyHz):
    // init members
    mGroup(group),
    mIndex(index),
    mFrequencyHz(frequencyHz){
    // nothing to do
}

void Timer::init(timer_config_t const& config){

    AssertExit(!mInitialized, "Timer::init()", "already initialized");

    // Compute divider from desired frequency (APB = 80 MHz)
    AssertExit((AppConfig::ClockFrequency % mFrequencyHz) == 0, "Timer::init()", "not divisible clock frequency");
    uint32_t divider = AppConfig::ClockFrequency / mFrequencyHz;
    AssertExit((divider >= 2) && (divider <= 65536), "Timer::init()", "divider out of range");

    // Create config
    mConfig = config;
    mConfig.divider = divider;

    // Configure hardware timer for 1 µs ticks
    esp_err_t err;
    err = timer_init(mGroup, mIndex, &mConfig);
    AssertExit(err == ESP_OK, "Timer::init()", "timer_init failed");

    err = timer_set_counter_value(mGroup, mIndex, 0);
    AssertExit(err == ESP_OK, "Timer::init()", "timer_set_counter_value failed");

    err = timer_start(mGroup, mIndex);
    AssertExit(err == ESP_OK, "Timer::init()", "timer_start failed");

    // init complete
    mInitialized = true;
    mRunning = true;
}

void Timer::resume(){
    if(!mInitialized){
        AssertDebug(false, "Timer::resume()", "not initialized");
        return;
    }
    if(!mRunning){
        esp_err_t err = timer_start(mGroup, mIndex);
        if(err != ESP_OK){
            AssertDebug(false, "Timer::resume()", "resume failed");
            return;
        }
        mRunning = true;
    }
}

void Timer::pause(){
    if(!mInitialized){
        AssertDebug(false, "Timer::pause()", "not initialized");
        return;
    }
    if(mRunning){
        esp_err_t err = timer_pause(mGroup, mIndex);
        if(err != ESP_OK){
            AssertDebug(false, "Timer::pause()", "pause failed");
            return;
        }
        mRunning = false;
    }
}

void Timer::setValue(uint32_t value){
    if(!mInitialized){
        AssertDebug(false, "Timer::setValue()", "not initialized");
        return;
    }
    esp_err_t err = timer_set_counter_value(mGroup, mIndex, value);
    AssertDebug(err == ESP_OK, "Timer::setValue()", "setValue failed");
}

void Timer::setValue(uint64_t value){
    if(!mInitialized){
        AssertDebug(false, "Timer::setValue()", "not initialized");
        return;
    }
    esp_err_t err = timer_set_counter_value(mGroup, mIndex, value);
    AssertDebug(err == ESP_OK, "Timer::setValue()", "setValue failed");
}

void Timer::setValueFromIsr(uint32_t value){
    if(!mInitialized){
        return;
    }
    timer_group_set_alarm_value_in_isr(mGroup, mIndex, value);
}

void Timer::setValueFromIsr(uint64_t value){
    if(!mInitialized){
        return;
    }
    timer_group_set_alarm_value_in_isr(mGroup, mIndex, value);
}

uint64_t Timer::getValue(){
    if(!mInitialized){
        AssertDebug(false, "Timer::getValue()", "not initialized");
        return 0;
    }
    uint64_t ticks = 0;
    esp_err_t err = timer_get_counter_value(mGroup, mIndex, &ticks);
    AssertDebug(err == ESP_OK, "Timer::getValue()", "getValue failed");
    return ticks;
}

uint64_t IRAM_ATTR Timer::getValueFromIsr(){
    if(!mInitialized) return 0;
    return timer_group_get_counter_value_in_isr(mGroup, mIndex);
}

timer_group_t Timer::getGroup(){
    return mGroup;
}

timer_idx_t Timer::getIndex(){
    return mIndex;
}

uint32_t Timer::getFrequencyHz(){
    if(!mInitialized){
        AssertDebug(false, "Timer::getFrequencyHz()", "not initialized");
        return 0;
    }
    return mFrequencyHz;
}

}