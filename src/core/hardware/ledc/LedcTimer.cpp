#include "LedcTimer.h"

#include "assert/Assert.h"

namespace Garbox {

LedcTimer::LedcTimer() : 
    // init members
    mResolutionBits(0),
    mMaxDuty(0),
    mInitialized(false){
    // nothing to do
}

bool LedcTimer::setup(Id id, uint32_t frequencyHz, uint8_t resolutionBits) {

    // check if setup
    if(mInitialized){
        FailExit("LedcTimer", "already initialized");
        return false;
    }
    
    // resolution
    mId = id;
    mMode = LEDC_LOW_SPEED_MODE;
    mIndex = static_cast<ledc_timer_t>(static_cast<uint8_t>(mId));
    mResolutionBits = resolutionBits;
    mMaxDuty = (1u << resolutionBits) - 1u;

    // create timer configuration
    ledc_timer_config_t cfg = {};
    cfg.speed_mode      = mMode;
    cfg.timer_num       = mIndex;
    cfg.duty_resolution = static_cast<ledc_timer_bit_t>(resolutionBits);
    cfg.freq_hz         = frequencyHz;
    cfg.clk_cfg         = LEDC_AUTO_CLK;

    // set timer configuration
    esp_err_t result = ledc_timer_config(&cfg);

    // check result
    if(result != ESP_OK){
        FailExit("LedcTimer", "setup failed");
        return false;
    }

    mInitialized = true;
    return true;
}

bool LedcTimer::setFrequency(uint32_t frequencyHz) const {

    // check if setup
    if(!mInitialized){
        FailDebug("LedcTimer", "not initialized");
        return false;
    }

    // zero not allowed
    if(frequencyHz == 0){
        FailDebug("LedcTimer", "frequencyHz == 0 not allowed");
        return false;
    }

    // set frequency
    esp_err_t result = ledc_set_freq(mMode, mIndex, frequencyHz);

    // check result
    if(result != ESP_OK){
        FailDebug("LedcTimer", "frequency set failed");
        return false;
    }
    return true;
}

LedcTimer::Id LedcTimer::getId() const{
    return mId;
}

ledc_mode_t LedcTimer::getMode() const{
    return mMode;
}

ledc_timer_t LedcTimer::getIndex() const{
    return mIndex;
}

uint8_t LedcTimer::getResolutionBits() const{
    return mResolutionBits;
}

uint32_t LedcTimer::getMaxDuty() const{
    return mMaxDuty;
}

} // namespace