#include "Gpio.h"
#include "assert/Assert.h"
#include "esp_err.h"

namespace Garbox {

Gpio::Gpio() {
    // nothing to do
}

void Gpio::setup(uint32_t pin, Mode mode, bool invert) {
    gpio_config_t cfg = {};

    switch (mode) {
        case Mode::Input:
            cfg.mode = GPIO_MODE_INPUT;
            break;
        case Mode::Output:
            cfg.mode = GPIO_MODE_OUTPUT;
            break;
        case Mode::InputPullup:
            cfg.mode = GPIO_MODE_INPUT;
            cfg.pull_up_en = GPIO_PULLUP_ENABLE;
            break;
        case Mode::InputPulldown:
            cfg.mode = GPIO_MODE_INPUT;
            cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        case Mode::OutputOpenDrain:
            cfg.mode = GPIO_MODE_OUTPUT_OD;
            break;
        case Mode::Uninitialized:
        default:
            AssertDebug(false, "Gpio::setup()", "called with invalid mode");
            return;
    }

    cfg.pin_bit_mask = (1ULL << pin);
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);

    mPin = pin;
    mMode = mode;
    mInvert = invert;
}

void Gpio::setValue(bool value) {
    switch (mMode) {
        case Mode::Output:
        case Mode::OutputOpenDrain:
            mState = value;
            gpio_set_level(static_cast<gpio_num_t>(mPin), (mInvert ? !value : value) ? 1 : 0);
            break;

        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown:
        case Mode::Uninitialized:
        default:
            AssertDebug(false, "Gpio::setValue()", "cannot set value in this mode");
            break;
    }
}

bool Gpio::getValue() const {
    switch (mMode) {
        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown: {
            bool raw = gpio_get_level(static_cast<gpio_num_t>(mPin)) != 0;
            return mInvert ? !raw : raw;
        }

        case Mode::Output:
        case Mode::OutputOpenDrain:
            return mInvert ? !mState : mState;

        case Mode::Uninitialized:
        default:
            AssertDebug(false, "Gpio::getValue()", "called before setup or with invalid mode");
            return false;
    }
}

void Gpio::toggle() {
    switch (mMode) {
        case Mode::Output:
        case Mode::OutputOpenDrain:
            mState = !mState;
            gpio_set_level(static_cast<gpio_num_t>(mPin), (mInvert ? !mState : mState) ? 1 : 0);
            break;

        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown:
        case Mode::Uninitialized:
        default:
            AssertDebug(false, "Gpio::toggle()", "cannot toggle in this mode");
            break;
    }
}

} // namespace Garbox
