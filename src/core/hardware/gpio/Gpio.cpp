#include "Gpio.h"

#include "assert/Assert.h"
#include "esp_err.h"

namespace Garbox {

Gpio::Gpio() {
    // nothing to do
}

void Gpio::init(int32_t pin, Mode mode, bool invert, bool initialLevel) {

    AssertExit(!mInitialized, "Gpio", "already initialized");
    AssertExit((pin >= 0) && (pin < static_cast<int32_t>(gpio_num_t::GPIO_NUM_MAX)), "Gpio", "invalid pin number");

    mPin = static_cast<gpio_num_t>(pin);
    mMode = mode;
    mInvert = invert;
    mLogicalLevel = initialLevel;


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
        TriggerExit("Gpio", "init unhandled mode");
        return;
    }

    cfg.pin_bit_mask = (1ULL << pin);
    cfg.intr_type = GPIO_INTR_DISABLE;
    if(gpio_config(&cfg) != ESP_OK){
        TriggerExit("Gpio", "gpio_config failed");
    }

    if(isOutput()){
        mLogicalLevel = initialLevel;
        if(gpio_set_level(mPin, logicalToRaw(mLogicalLevel)) != ESP_OK){
            TriggerExit("Gpio", "gpio_set_level failed");
        }
    }
    else if(isInput()){
        mLogicalLevel = rawToLogical(gpio_get_level(mPin));
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
        mLogicalLevel = !mLogicalLevel;
        if(gpio_set_level(mPin, logicalToRaw(mLogicalLevel)) != ESP_OK){
            TriggerDebug("Gpio", "toggle gpio_set_level failed");
        }
        break;
    case Mode::Input:
    case Mode::InputPullup:
    case Mode::InputPulldown:
    default:
        TriggerDebug("Gpio", "toggle unhandled mode");
        break;
    }
}

void Gpio::writeLevel(bool logicalLevel) {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mMode) {
    case Mode::Output:
    case Mode::OutputOpenDrain:
        if(mLogicalLevel == logicalLevel){
            return;
        }
        mLogicalLevel = logicalLevel;
        if(gpio_set_level(mPin, logicalToRaw(mLogicalLevel)) != ESP_OK){
            TriggerDebug("Gpio", "writeLevel gpio_set_level failed");
        }
        break;
    case Mode::Input:
    case Mode::InputPullup:
    case Mode::InputPulldown:
    default:
        TriggerDebug("Gpio", "writeLevel unhandled mode");
        break;
    }
}

void Gpio::writeLevelRaw(bool rawLevel) {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mMode) {
    case Mode::Output:
    case Mode::OutputOpenDrain: {
        bool logicalLevel = rawToLogical(rawLevel);
        if(mLogicalLevel == logicalLevel){
            return;
        }
        mLogicalLevel = logicalLevel;
        if(gpio_set_level(mPin, rawLevel) != ESP_OK){
            TriggerDebug("Gpio", "writeLevelRaw gpio_set_level failed");
        }
        break;
    }
    case Mode::Input:
    case Mode::InputPullup:
    case Mode::InputPulldown:
    default:
        TriggerDebug("Gpio", "writeLevelRaw unhandled mode");
        break;
    }
}

bool Gpio::readLevel() const {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return false;
    }
    switch (mMode) {
    case Mode::Input:
    case Mode::InputPullup:
    case Mode::InputPulldown: 
        return rawToLogical(gpio_get_level(mPin));
    case Mode::Output:
    case Mode::OutputOpenDrain:
        return mLogicalLevel;
    default:
        TriggerDebug("Gpio", "readLevel unhandled mode");
        return false;
    }
}

bool Gpio::readLevelRaw() const {
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return false;
    }
    switch (mMode) {
    case Mode::Input:
    case Mode::InputPullup:
    case Mode::InputPulldown: 
        return gpio_get_level(mPin) != 0;
    case Mode::Output:
    case Mode::OutputOpenDrain:
        return logicalToRaw(mLogicalLevel);
    default:
        TriggerDebug("Gpio", "readLevelRaw unhandled mode");
        return false;
    }
}

int32_t Gpio::getPin() const {
    return static_cast<int32_t>(mPin);
}

bool Gpio::isInverted() const{
    return mInvert;
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

bool Gpio::rawToLogical(int rawLevel) const {
    return (rawLevel != 0) ^ mInvert;
}

uint32_t Gpio::logicalToRaw(bool logicalLevel) const {
    return static_cast<uint32_t>(logicalLevel ^ mInvert);
}

} // namespace Garbox
