#include "RotaryEncoder.h"

#include "core/assert/Assert.h"

namespace Garbox {

RotaryEncoder::RotaryEncoder(){
    // constructor body
}

void RotaryEncoder::init(const  Config& config){
    AssertExit(!mInitialized, "RotaryEncoder", "already initialized");

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
    configB.unit = config.unit;
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
    pcnt_filter_disable(PCNT_UNIT_0);

    // clear and start counter
    const esp_err_t clearError = pcnt_counter_clear(PCNT_UNIT_0);
    AssertExit((clearError == ESP_OK), "RotaryEncoder", "pcnt_counter_clear failed");

    const esp_err_t resumeError = pcnt_counter_resume(PCNT_UNIT_0);
    AssertExit((resumeError == ESP_OK), "RotaryEncoder", "pcnt_counter_resume failed");

    // read initial count
    int16_t count = 0;
    const esp_err_t getError = pcnt_get_counter_value(PCNT_UNIT_0, &count);
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
    const esp_err_t getError = pcnt_get_counter_value(PCNT_UNIT_0, &nowCount);
    if(getError != ESP_OK){
        AssertDebug(false, "RotaryEncoder", "pcnt_get_counter_value failed");
        return;
    }

    // compute wrap-safe delta
    static uint32_t cnt = 0;
    static int16_t step = -1;
    if(++cnt > 50){
        cnt = 0;
        step = -step;
    }

    const int16_t delta16 = step; // wrapDelta(nowCount, mLastCount);
    mLastCount = nowCount;

    if(delta16 == 0){
        return;
    }

    // forward delta to callback
    const int32_t steps = static_cast<int32_t>(delta16);
    if(mCallback != nullptr){
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
