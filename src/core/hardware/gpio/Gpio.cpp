#include "Gpio.h"

#include "core/assert/Assert.h"
#include "esp_err.h"

namespace Garbox {

Gpio::Gpio(){
    // constructor body
}

void Gpio::init(int32_t pin, bool initialLevel, const Config& config){
    AssertExit(!mInitialized, "Gpio", "already initialized");
    AssertExit((pin >= 0) && (pin < static_cast<int32_t>(gpio_num_t::GPIO_NUM_MAX)), "Gpio", "invalid pin number");
    mPin = static_cast<gpio_num_t>(pin);
    mLogicalLevel = initialLevel;
    mInitialized = true;
    applyConfig(config);
}

void Gpio::applyConfig(const Config& config){
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    
    // store config 
    mConfig = config;

    // create gpio config 
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << static_cast<int32_t>(mPin)),
        .mode = GPIO_MODE_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    // configure mode
    switch (mConfig.mode){
    case Mode::Disable:
        cfg.mode = GPIO_MODE_DISABLE;
        break;
    case Mode::Input:
        cfg.mode = GPIO_MODE_INPUT;
        break;
    case Mode::Output:
        cfg.mode = GPIO_MODE_OUTPUT;
        break;
    case Mode::OutputOpenDrain:
        cfg.mode = GPIO_MODE_OUTPUT_OD;
        break;
    default:
        TriggerExit("Gpio", "init unhandled mode");
    }

    // configure pullup and pulldown
    switch(mConfig.pull){
    case Pull::Up:
        cfg.pull_up_en = GPIO_PULLUP_ENABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        break;
    case Pull::Down:
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
        break;
    case Pull::Disable:
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        break;
    default:
        TriggerExit("Gpio", "init unhandled pull mode");
    }

    // configure interrupt type
    switch(mConfig.interrupt){
    case Interrupt::Disable:
        cfg.intr_type = GPIO_INTR_DISABLE;
        break;
    case Interrupt::PositiveEdge:
        cfg.intr_type = GPIO_INTR_POSEDGE;
        break;
    case Interrupt::NegativeEdge:
        cfg.intr_type = GPIO_INTR_NEGEDGE;
        break;
    case Interrupt::AnyEdge:
        cfg.intr_type = GPIO_INTR_ANYEDGE;
        break;
    case Interrupt::HighLevel:
        cfg.intr_type = GPIO_INTR_HIGH_LEVEL;
        break;
    case Interrupt::LowLevel:
        cfg.intr_type = GPIO_INTR_LOW_LEVEL;
        break;
    default:
        TriggerExit("Gpio", "init unhandled interrupt mode");
    }

    // apply gpio config
    if(gpio_config(&cfg) != ESP_OK){
        TriggerExit("Gpio", "gpio_config failed");
    }
    
    // apply level
    if(isOutput()){
        if(gpio_set_level(mPin, logicalToRaw(mLogicalLevel)) != ESP_OK){
            TriggerExit("Gpio", "gpio_set_level failed");
        }
    }
    else if(isInput()){
        mLogicalLevel = rawToLogical(gpio_get_level(mPin));
    }
}

Gpio::Config Gpio::getCurrentConfig() const {
    return mConfig;
}

void Gpio::toggle(){
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mConfig.mode){
    case Mode::Output:
    case Mode::OutputOpenDrain:
        mLogicalLevel = !mLogicalLevel;
        if(gpio_set_level(mPin, logicalToRaw(mLogicalLevel)) != ESP_OK){
            TriggerDebug("Gpio", "toggle gpio_set_level failed");
        }
        break;
    default:
        TriggerDebug("Gpio", "toggle unhandled mode");
        break;
    }
}

void Gpio::writeLevel(bool logicalLevel){
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mConfig.mode){
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
    default:
        TriggerDebug("Gpio", "writeLevel unhandled mode");
        break;
    }
}

void Gpio::writeLevelRaw(bool rawLevel){
    if(!mInitialized){
        TriggerDebug("Gpio", "not initialized"); 
        return;
    }
    switch (mConfig.mode){
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
    switch (mConfig.mode){
    case Mode::Input:
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
    switch (mConfig.mode){
    case Mode::Input:
        return gpio_get_level(mPin) != 0;
    case Mode::Output:
    case Mode::OutputOpenDrain:
        return logicalToRaw(mLogicalLevel);
    default:
        TriggerDebug("Gpio", "readLevelRaw unhandled mode");
        return false;
    }
}

bool Gpio::addInterruptHandler(InterruptHandler handler, void* user){
    esp_err_t err = gpio_isr_handler_add(mPin, handler, user);
    if(err != ESP_OK){
        TriggerDebug("Gpio", "gpio_isr_handler_add failed", err);
        return false;
    } 
    return true;
}

bool Gpio::setInterruptEnabled(bool enabled){
    if(enabled){
        esp_err_t err = gpio_intr_enable(mPin);
        if(err != ESP_OK){
            TriggerDebug("Gpio", "gpio_intr_enable failed", err);
            return false;
        } 
    }
    else {
        esp_err_t err = gpio_intr_disable(mPin);
        if(err != ESP_OK){
            TriggerDebug("Gpio", "gpio_intr_disable failed", err);
            return false;
        } 
    }
    return true;
}

int32_t Gpio::getPinNumber() const {
    return static_cast<int32_t>(mPin);
}

bool Gpio::isInverted() const {
    return mConfig.invert;
}

bool Gpio::isInput() const {
    return (mConfig.mode == Mode::Input);
}

bool Gpio::isOutput() const {
    return (mConfig.mode == Mode::Output) || (mConfig.mode == Mode::OutputOpenDrain);
}

bool Gpio::hasPullup() const {
    return (mConfig.pull == Pull::Up);
}

bool Gpio::hasPulldown() const {
    return (mConfig.pull == Pull::Down);
}

bool Gpio::isInitialized() const {
    return mInitialized;
}

bool Gpio::rawToLogical(int rawLevel) const {
    return (rawLevel != 0) ^ mConfig.invert;
}

uint32_t Gpio::logicalToRaw(bool logicalLevel) const {
    return static_cast<uint32_t>(logicalLevel ^ mConfig.invert);
}

} // namespace Garbox
