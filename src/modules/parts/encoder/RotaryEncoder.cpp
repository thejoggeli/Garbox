#include "RotaryEncoder.h"

#include "core/assert/Assert.h"

namespace Garbox {

RotaryEncoder::RotaryEncoder(){
    // constructor body
}

void RotaryEncoder::init(const  Config& config){
    AssertExit(!mInitialized, "RotaryEncoder", "already initialized");
    AssertExit(config.countsPerStep > 0, "RotaryEncoder", "counts per step must be positive");

    mUnit = config.unit;
    mCountsPerStep = config.countsPerStep;
    mInvertDirection = config.invertDirection;

    // configure PCNT channel 0
    pcnt_config_t configA = {};
    configA.pulse_gpio_num = config.pinA;
    configA.ctrl_gpio_num = config.pinB;
    configA.unit = config.unit;
    configA.channel = PCNT_CHANNEL_0;
    configA.pos_mode = PCNT_COUNT_INC;
    configA.neg_mode = PCNT_COUNT_DEC;
    configA.lctrl_mode = PCNT_MODE_REVERSE;
    configA.hctrl_mode = PCNT_MODE_KEEP;
    configA.counter_h_lim = INT16_MAX;
    configA.counter_l_lim = INT16_MIN;

    const esp_err_t configAError = pcnt_unit_config(&configA);
    AssertExit((configAError == ESP_OK), "RotaryEncoder", "pcnt_unit_config A failed");

    // configure PCNT channel 1
    pcnt_config_t configB = {};
    configB.pulse_gpio_num = config.pinB;
    configB.ctrl_gpio_num = config.pinA;
    configB.unit = mUnit;
    configB.channel = PCNT_CHANNEL_1;
    configB.pos_mode = PCNT_COUNT_DEC;
    configB.neg_mode = PCNT_COUNT_INC;
    configB.lctrl_mode = PCNT_MODE_REVERSE;
    configB.hctrl_mode = PCNT_MODE_KEEP;
    configB.counter_h_lim = INT16_MAX;
    configB.counter_l_lim = INT16_MIN;

    const esp_err_t configBError = pcnt_unit_config(&configB);
    AssertExit((configBError == ESP_OK), "RotaryEncoder", "pcnt_unit_config B failed");

    // optional glitch filter disabled by default
    pcnt_filter_disable(mUnit);

    // clear and start counter
    const esp_err_t clearError = pcnt_counter_clear(mUnit);
    AssertExit((clearError == ESP_OK), "RotaryEncoder", "pcnt_counter_clear failed");

    const esp_err_t resumeError = pcnt_counter_resume(mUnit);
    AssertExit((resumeError == ESP_OK), "RotaryEncoder", "pcnt_counter_resume failed");

    // read initial count
    int16_t count = 0;
    const esp_err_t getError = pcnt_get_counter_value(mUnit, &count);
    AssertExit((getError == ESP_OK), "RotaryEncoder", "pcnt_get_counter_value failed");

    mLastCount = count;
    mInitialized = true;
}

void RotaryEncoder::tick(){
    if(!mInitialized){
        TriggerDebug("RotaryEncoder", "not initialized");
        return;
    }

    // read current hardware counter
    int16_t nowCount = 0;
    const esp_err_t getError = pcnt_get_counter_value(mUnit, &nowCount);
    if(getError != ESP_OK){
        AssertDebug(false, "RotaryEncoder", "pcnt_get_counter_value failed");
        return;
    }

    // compute wrap-safe delta
    const int16_t delta16 = wrapDelta(nowCount, mLastCount);
    mLastCount = nowCount;

    if(delta16 == 0){
        return;
    }

    // Convert PCNT edges into physical detents. Keeping the remainder on the
    // instance preserves partial turns and direction reversals correctly.
    mPendingCounts += static_cast<int32_t>(delta16);
    int32_t steps = mPendingCounts / static_cast<int32_t>(mCountsPerStep);
    mPendingCounts %= static_cast<int32_t>(mCountsPerStep);
    if(mInvertDirection){
        steps = -steps;
    }

    if(steps != 0 && mCallback != nullptr){
        mCallback(mCallbackCtx, steps);
    }
}

void RotaryEncoder::setCallback(void* ctx, Callback callback){
    mCallback = callback;
    mCallbackCtx = ctx;
}

int16_t RotaryEncoder::wrapDelta(int16_t now, int16_t last){
    const uint16_t uNow = static_cast<uint16_t>(now);
    const uint16_t uLast = static_cast<uint16_t>(last);
    const uint16_t uDelta = static_cast<uint16_t>(uNow - uLast);

    return static_cast<int16_t>(uDelta);
}

} // namespace Garbox
