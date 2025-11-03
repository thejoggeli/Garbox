#include "PulseCounter.h"

#include "assert/Assert.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_private/esp_clk.h"

namespace Garbox {

PulseCounter::PulseCounter(pcnt_unit_t unit, uint32_t pin) : 
    // init members
    mUnit(unit),
    mPin(pin){
    // nothing to do
}

bool PulseCounter::init(Config const& config) {

    if (mInitialized) {
        AssertExit(false, "PulseCounter", "already initialized");
        return false;
    }

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
            AssertExit(false, "PulseCounter", "invalid PinMode");
            return false;
    }
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // pcnt_config_t describes how this channel behaves
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = static_cast<int>(mPin); // tach signal pin
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED; // we don't use a control pin
    cfg.unit           = mUnit;
    cfg.channel        = PCNT_CHANNEL_0;

    // Count on one edge only.
    // Typical PC fan tacho: open-collector pulsing low twice/rev.
    // You can pick rising or falling. We'll count FALLING edges.
    //
    // For falling edges, we treat negative edge as "increase":
    cfg.pos_mode   = PCNT_COUNT_DIS; // on rising edge, don't change
    cfg.neg_mode   = PCNT_COUNT_INC; // on falling edge, increment

    // Control modes not used (no direction ctrl pin), so KEEP
    cfg.lctrl_mode = PCNT_MODE_KEEP;
    cfg.hctrl_mode = PCNT_MODE_KEEP;

    // Counter limits (hardware saturates at these)
    cfg.counter_l_lim = config.minCount;
    cfg.counter_h_lim = config.maxCount;

    esp_err_t err = pcnt_unit_config(&cfg);
    if(err != ESP_OK){
        AssertExit(false, "TachoPulseCounter", "pcnt_unit_config has error");
        return false;
    }

    // setup filter
    pcnt_set_filter_value(mUnit, config.filterCycles);
    pcnt_filter_enable(mUnit);

    // setup complete
    mInitialized = true;
    return true;
}

bool PulseCounter::start() {
    if (!mInitialized) {
        AssertExit(false, "PulseCounter", "not initialized");
        return false;
    }

    esp_err_t err = pcnt_counter_clear(mUnit);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter", "pcnt_unit_clear_count failed");
        return false;
    }

    err = pcnt_counter_resume(mUnit);
    if (err != ESP_OK) {
        AssertExit(false, "PulseCounter", "pcnt_unit_start failed");
        return false;
    }

    return true;
}

int32_t PulseCounter::getCount() const {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter", "not initialized");
        return 0;
    }

    int16_t count = 0;
    esp_err_t err = pcnt_get_counter_value(mUnit, &count);
    if (err != ESP_OK) {
        AssertDebug(false, "PulseCounter", "pcnt_unit_get_count failed");
        return 0;
    }

    return static_cast<int32_t>(count);
}

int32_t PulseCounter::getAndClearCount() {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter", "not initialized");
        return 0;
    }

    int32_t count = getCount();
    clearCount();
    return count;
}

void PulseCounter::clearCount() {
    if (!mInitialized) {
        AssertDebug(false, "PulseCounter", "not initialized");
        return;
    }

    esp_err_t err = pcnt_counter_clear(mUnit);
    if (err != ESP_OK) {
        AssertDebug(false, "PulseCounter", "pcnt_unit_clear_count failed");
    }
}

} // namespace Garbox
