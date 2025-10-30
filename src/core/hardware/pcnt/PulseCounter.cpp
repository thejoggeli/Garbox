#include "PulseCounter.h"

#include "assert/Assert.h"
#include "driver/gpio.h"
#include "esp_err.h"

namespace Garbox {

PulseCounter::PulseCounter(uint32_t pin) : mPin(pin){
    // nothing to do
}

bool PulseCounter::init(Config const& config) {
    if (mInitialized) {
        AssertExit(false, "PulseCounter::init()", "already initialized");
        return ESP_FAIL;
    }

    esp_err_t err;

    // Configure the input pin
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = 1ULL << mPin;
    io_conf.mode = GPIO_MODE_INPUT;
    switch(config.pinMode){
        case PinMode::Floating:
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Pullup:
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Pulldown:
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        default:
            AssertExit(false, "PulseCounter::init()", "invalid PinMode");
            return false;
    }
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // 1. Create PCNT unit
    pcnt_unit_config_t unit_cfg = {
        .low_limit = config.minCount,
        .high_limit = config.maxCount,
        .intr_priority = 0,
        .flags = {
            .accum_count = false
        }
    };
    err = pcnt_new_unit(&unit_cfg, &mUnitHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_new_unit failed");
        return false;
    }

    // 2. Create channel
    pcnt_chan_config_t chan_cfg = {
        .edge_gpio_num = static_cast<int>(mPin),
        .level_gpio_num = -1, // unused
        .flags = {}
    };
    err = pcnt_new_channel(mUnitHandle, &chan_cfg, &mChannelHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_new_channel failed");
        return false;
    }

    // 3. Configure counting behavior: count on falling edge
    err = pcnt_channel_set_edge_action(
        mChannelHandle,
        PCNT_CHANNEL_EDGE_ACTION_HOLD,      // rising: no change
        PCNT_CHANNEL_EDGE_ACTION_INCREASE   // falling: increment
    );
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_channel_set_edge_action failed");
        return false;
    }

    err = pcnt_channel_set_level_action(
        mChannelHandle,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,     // high: keep
        PCNT_CHANNEL_LEVEL_ACTION_KEEP      // low: keep
    );
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_channel_set_level_action failed");
        return false;
    }

    // 4. Configure glitch filter
    if (config.glitchFilterNanos > 0) {
        pcnt_glitch_filter_config_t filt_cfg = {
            .max_glitch_ns = config.glitchFilterNanos
        };
        err = pcnt_unit_set_glitch_filter(mUnitHandle, &filt_cfg);
        if (err != ESP_OK) {
            AssertExit(false, "PulseCounter::init()", "pcnt_unit_set_glitch_filter failed");
            return false;
        }
    }

    // 5. Enable and clear
    err = pcnt_unit_enable(mUnitHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_unit_enable failed");
        return false;
    }

    err = pcnt_unit_clear_count(mUnitHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::init()", "pcnt_unit_clear_count failed");
        return false;
    }

    mInitialized = true;
    return true;
}

bool PulseCounter::start() {
    if (!mInitialized) {
        AssertExit(false, "PulseCounter::start()", "not initialized");
        return false;
    }

    esp_err_t err = pcnt_unit_clear_count(mUnitHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::start()", "pcnt_unit_clear_count failed");
        return false;
    }

    err = pcnt_unit_start(mUnitHandle);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter::start()", "pcnt_unit_start failed");
        return false;
    }

    return true;
}

int32_t PulseCounter::getCount() const {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter::getCount()", "not initialized");
        return 0;
    }

    int count = 0;
    esp_err_t err = pcnt_unit_get_count(mUnitHandle, &count);
    if (err != ESP_OK) {
        AssertDebug(false, "PulseCounter::getCount()", "pcnt_unit_get_count failed");
        return 0;
    }

    return static_cast<int32_t>(count);
}

int32_t PulseCounter::getAndClearCount() {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter::getAndClearCount()", "not initialized");
        return 0;
    }

    int32_t count = getCount();
    clearCount();
    return count;
}

void PulseCounter::clearCount() {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter::clearCount()", "not initialized");
        return;
    }

    esp_err_t err = pcnt_unit_clear_count(mUnitHandle);
    if (err != ESP_OK) {
        AssertDebug(false, "PulseCounter::clearCount()", "pcnt_unit_clear_count failed");
    }
}

} // namespace Garbox
