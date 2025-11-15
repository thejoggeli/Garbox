#include "Timer.h"

#include "assert/Assert.h"

namespace Garbox {

Timer::Timer(){
    // nothing to do
}

void Timer::init(InitStruct const& initStruct){

    AssertExit(!mInitialized, "Timer", "already initialized");

    // init members
    mGroup = initStruct.group;
    mIndex = initStruct.index;
    mFrequencyHz = initStruct.frequencyHz;
    mMaxValue = initStruct.maxValue;
    mConfig = initStruct.config;

    // determine clock frequency
    uint32_t clockFrequency;
    switch(mConfig.clk_src){
        case TIMER_SRC_CLK_APB:
            clockFrequency = APB_CLK_FREQ;
            break;
        case TIMER_SRC_CLK_XTAL:
            clockFrequency = XTAL_CLK_FREQ;
            break;
        default:
            clockFrequency = 0;
            AssertExit(!mInitialized, "Timer", "unhandled clock source");
    }

    // compute divider from desired frequency (APB = 80 MHz)
    AssertExit((clockFrequency > mFrequencyHz), "Timer", "timer frequency too large");
    AssertExit((clockFrequency % mFrequencyHz) == 0, "Timer", "not divisible clock frequency");
    uint32_t divider = clockFrequency / mFrequencyHz;
    AssertExit((divider >= 2) && (divider <= 65536), "Timer", "divider out of range");
    mConfig.divider = divider;

    // configure hardware timer for 1 µs ticks
    esp_err_t err;
    err = timer_init(mGroup, mIndex, &mConfig);
    AssertExit(err == ESP_OK, "Timer", "timer_init failed");

    // set initial counter value
    err = timer_set_counter_value(mGroup, mIndex, 0);
    AssertExit(err == ESP_OK, "Timer", "timer_set_counter_value failed");

    // enable alarm
    if(mConfig.alarm_en == TIMER_ALARM_EN){
        AssertExit(mMaxValue != 0, "Timer", "expected maxValue != 0");
        err = timer_set_alarm_value(mGroup, mIndex, mMaxValue);
        AssertExit(err == ESP_OK, "Timer", "timer_set_alarm_value failed");
        err = timer_enable_intr(mGroup, mIndex);
        AssertExit(err == ESP_OK, "Timer", "timer_enable_intr failed");
    }
    else {
        AssertExit(mMaxValue == 0, "Timer", "expected maxValue == 0");
    }

    err = timer_start(mGroup, mIndex);
    AssertExit(err == ESP_OK, "Timer", "timer_start failed");

    // init complete
    mInitialized = true;
    mRunning = true;
}

void Timer::start(){
    if(!mInitialized){
        TriggerDebug("Timer", "not initialized");
        return;
    }
    if(!mRunning){
        esp_err_t err = timer_start(mGroup, mIndex);
        if(err != ESP_OK){
            TriggerDebug("Timer", "resume failed");
            return;
        }
        mRunning = true;
    }
}

void Timer::stop(){
    if(!mInitialized){
        TriggerDebug("Timer", "not initialized");
        return;
    }
    if(mRunning){
        esp_err_t err = timer_pause(mGroup, mIndex);
        if(err != ESP_OK){
            TriggerDebug("Timer", "pause failed");
            return;
        }
        mRunning = false;
    }
}

void Timer::reset(){
    stop();
    setValue(0u);
}

void Timer::setValue(uint32_t value){
    if(!mInitialized){
        TriggerDebug("Timer", "not initialized");
        return;
    }
    esp_err_t err = timer_set_counter_value(mGroup, mIndex, value);
    AssertDebug(err == ESP_OK, "Timer", "setValue failed");
}

void Timer::setValue(uint64_t value){
    if(!mInitialized){
        TriggerDebug("Timer", "not initialized");
        return;
    }
    esp_err_t err = timer_set_counter_value(mGroup, mIndex, value);
    AssertDebug(err == ESP_OK, "Timer", "setValue failed");
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
        TriggerDebug("Timer", "not initialized");
        return 0;
    }
    uint64_t ticks = 0;
    esp_err_t err = timer_get_counter_value(mGroup, mIndex, &ticks);
    AssertDebug(err == ESP_OK, "Timer", "getValue failed");
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
        TriggerDebug("Timer", "not initialized");
        return 0;
    }
    return mFrequencyHz;
}

uint64_t Timer::getMaxValue(){
    if(!mInitialized){
        TriggerDebug("Timer", "not initialized");
        return 0;
    }
    return mMaxValue;
}

}