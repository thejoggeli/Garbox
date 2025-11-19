#include "I2cInstances.h"

#include "app/config/PinConfig.h"
#include "core/assert/Assert.h"

namespace Garbox {

static bool gInitialized = false;

void I2cInstances::Init(){
    
    AssertExit(!gInitialized, "I2cInstances", "already initialized");

    GetI2c().init({
        .port = I2c::Port::Num0,
        .sdaPin = PinConfig::I2cSda,
        .sclPin = PinConfig::I2cScl,
        .sdaPull = I2c::Pull::Disable,
        .sclPull = I2c::Pull::Disable,
        .frequencyHz = 100'000,
    });

    gInitialized = true;
}

I2c& I2cInstances::GetI2c(){
    static I2c instance;
    return instance;
}

} // namespace
