#include "Gpio.h"
#include "assert/Assert.h"
#include "esp_err.h"

namespace Garbox {

Gpio::Gpio() {
    // nothing to do
}

void Gpio::setup(int32_t pin, Mode mode, bool invert, bool initivalValue) {

    AssertExit(!mInitialized, "Gpio", "already initialized");

    mPin = static_cast<gpio_num_t>(pin);
    mMode = mode;
    mInvert = invert;
    mValue = initivalValue;

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
        default:
            TriggerDebug("Gpio", "called with unhandled mode");
            return;
    }

    cfg.pin_bit_mask = (1ULL << pin);
    cfg.intr_type = GPIO_INTR_DISABLE;
    if(gpio_config(&cfg) != ESP_OK){
        TriggerDebug("Gpio", "gpio_config failed");
    }

    mValue = initivalValue;
    if((mMode == Mode::Output) || (mMode == Mode::OutputOpenDrain)){
        if(gpio_set_level(mPin, toPinValue(mValue)) != ESP_OK){
            TriggerDebug("Gpio", "gpio_set_level failed");
        }
    }

    mInitialized = true;
}

void Gpio::toggle() {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mMode) {
        case Mode::Output:
        case Mode::OutputOpenDrain:
            mValue = !mValue;
            gpio_set_level(mPin, toPinValue(mValue));
            break;
        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown:
        default:
            TriggerDebug("Gpio", "unhandled mode");
            break;
    }
}

void Gpio::setValue(bool value) {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mMode) {
        case Mode::Output:
        case Mode::OutputOpenDrain:
            if(mValue == value){
                return;
            }
            mValue = value;
            gpio_set_level(mPin, toPinValue(mValue));
            break;
        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown:
        default:
            TriggerDebug("Gpio", "unhandled mode");
            break;
    }
}

bool Gpio::getValue() const {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return false;
    }
    switch (mMode) {
        case Mode::Input:
        case Mode::InputPullup:
        case Mode::InputPulldown: 
            return fromPinValue(gpio_get_level(mPin));
        case Mode::Output:
        case Mode::OutputOpenDrain:
            return mValue;
        default:
            TriggerDebug("Gpio", "unhandled mode");
            return false;
    }
}

int32_t Gpio::getPin() const {
    return static_cast<int32_t>(mPin);
}

bool Gpio::isInput() const{
    return (mMode == Mode::Input) || (mMode == Mode::InputPulldown) || (mMode == Mode::InputPullup);
}

bool Gpio::isOutput() const{
    return (mMode == Mode::Output) || (mMode == Mode::OutputOpenDrain);
}

bool Gpio::hasPullup() const{
    return (mMode == Mode::InputPullup);
}

bool Gpio::hasPulldown() const{
    return (mMode == Mode::InputPulldown);
}

bool Gpio::fromPinValue(int value) const {
    if(mInvert){
        return value == 0;
    }
    return value != 0; 
}

uint32_t Gpio::toPinValue(bool value) const {
    if(mInvert){
        return value ? 0 : 1;
    }
    return value ? 1 : 0;
}

} // namespace Garbox
