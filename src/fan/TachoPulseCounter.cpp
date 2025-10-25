#include "TachoPulseCounter.h"

#include <Arduino.h> // for gpio config helpers (optional but typical in Arduino env)

namespace Garbox {

TachoPulseCounter::TachoPulseCounter(
    uint32_t pin,
    pcnt_unit_t unit,
    pcnt_channel_t channel,
    int16_t minCount,
    int16_t maxCount):
    // init members
    mPin(pin),
    mUnit(unit),
    mChannel(channel),
    mMinCount(minCount),
    mMaxCount(maxCount){
    // nothing to do
}

bool TachoPulseCounter::init() {
    if (mInitialized) {
        return true;
    }

    // pcnt_config_t describes how this channel behaves
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = static_cast<int>(mPin);      // tach signal pin
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;           // we don't use a control pin
    cfg.unit           = mUnit;
    cfg.channel        = mChannel;

    // Count on one edge only.
    // Typical PC fan tacho: open-collector pulsing low twice/rev.
    // You can pick rising or falling. We'll count FALLING edges.
    //
    // For falling edges, we treat negative edge as "increase":
    cfg.pos_mode   = PCNT_COUNT_DIS;   // on rising edge, don't change
    cfg.neg_mode   = PCNT_COUNT_INC;   // on falling edge, increment

    // Control modes not used (no direction ctrl pin), so KEEP
    cfg.lctrl_mode = PCNT_MODE_KEEP;
    cfg.hctrl_mode = PCNT_MODE_KEEP;

    // Counter limits (hardware saturates at these)
    cfg.counter_l_lim = mMinCount;
    cfg.counter_h_lim = mMaxCount;

    esp_err_t err = pcnt_unit_config(&cfg);
    if (err != ESP_OK) {
        return false;
    }

    // Optional: add input filter to reject very short glitches/noise.
    // filter_val is in APB clock cycles (~80 MHz default on ESP32),
    // max 1023. Let's pick something modest.
    //
    // NOTE: you *must* enable filter after setting its value.
    pcnt_set_filter_value(mUnit, 100);      // ignore pulses shorter than ~100 cycles
    pcnt_filter_enable(mUnit);

    // By default, pcnt_unit_config disables some events; we don't enable interrupts here.
    // We just do raw counting.

    mInitialized = true;
    return true;
}

bool TachoPulseCounter::start() {
    if (!mInitialized) {
        if (!init()) {
            return false;
        }
    }

    // Clear and resume the counter
    if (pcnt_counter_clear(mUnit) != ESP_OK) {
        return false;
    }
    if (pcnt_counter_resume(mUnit) != ESP_OK) {
        return false;
    }

    return true;
}

int TachoPulseCounter::getCount() const {
    if (!mInitialized) {
        return 0;
    }

    int16_t val = 0;
    if (pcnt_get_counter_value(mUnit, &val) != ESP_OK) {
        return 0;
    }
    return static_cast<int>(val);
}

bool TachoPulseCounter::clearCount() {
    if (!mInitialized) {
        return false;
    }

    return (pcnt_counter_clear(mUnit) == ESP_OK);
}

int TachoPulseCounter::getAndClearCount() {
    int current = getCount();
    clearCount();
    return current;
}

} // namespace Garbox
