#include "LedcChannel.h"

#include "assert/Assert.h"
#include "LedcTimer.h"

namespace Garbox {

LedcChannel::LedcChannel() : 
    // init members
    mId(Id::Channel0), 
    mTimer(nullptr), 
    mGpio(0),
    mDuty(0),
    mInitialized(false){
    // nothing to do
}

bool LedcChannel::setup(Id id, LedcTimer* timer, uint32_t gpio, bool invert) {

    // check if setup
    if(mInitialized){
        AssertDebug(false, "LedChannel::setup()", "already initialized");
        return false;
    }

    // timer must exist
    if(timer == nullptr){
        AssertDebug(false, "LedChannel::setup()", "timer is nullptr");
        return false;
    }

    // set gpio
    mId = id;
    mTimer = timer;
    mGpio = gpio;
    mChannelIndex = static_cast<ledc_channel_t>(id);
    mMode = timer->getMode();
    mDuty = invert ? mTimer->getMaxDuty() : 0;

    // set initial duty such that signal equals 0V
    ledc_set_duty(mMode, mChannelIndex, mDuty);
    ledc_update_duty(mMode, mChannelIndex);

    // create channel config
    ledc_channel_config_t ch = {};
    ch.speed_mode = mMode;
    ch.channel    = mChannelIndex;
    ch.timer_sel  = mTimer->getIndex();
    ch.duty       = 0;
    ch.hpoint     = 0;
    ch.gpio_num   = mGpio;
    ch.flags.output_invert = invert;

    // set channel config
    esp_err_t result = ledc_channel_config(&ch);

    // check result
    if(result != ESP_OK){
        AssertExit(false, "LedChannel::setup()", "attach failed");
        return false;
    }

    mInitialized = true;
    return true;
}

bool LedcChannel::setDutyRaw(uint32_t duty, bool update) {

    // check if setup
    if(!mInitialized){
        AssertDebug(false, "LedChannel::setDutyRaw()", "not initialized");
        return false;
    }

    // set duty
    esp_err_t err = ledc_set_duty(mMode, mChannelIndex, duty);

    // check result
    if (err != ESP_OK){
        AssertDebug(false, "LedChannel::setDutyRaw()", "setDutyRaw failed");
        return false;
    }
    else {
        mDuty = duty;
    }

    // done if no update requested
    if(!update){
        return true;
    }

    // update duty
    err = ledc_update_duty(mMode, mChannelIndex);

    // check result
    if (err != ESP_OK){
        AssertDebug(false, "LedChannel::setDutyRaw()", "updateDuty failed");
        return false;
    }
    return true;
}

bool LedcChannel::setDutyRelative(float ratio, bool update){

    // check if setup
    if(!mInitialized){
        AssertDebug(false, "LedChannel::setDutyRelative()", "channel not initialized");
        return false;
    }

    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    uint32_t const duty = static_cast<uint32_t>(ratio * static_cast<float>(mTimer->getMaxDuty()) + 0.5f);
    
    return setDutyRaw(duty, update);
}

uint32_t LedcChannel::getDuty() const {
    return mDuty;
}

} // namespace
