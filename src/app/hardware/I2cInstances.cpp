// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "I2cInstances.h"
#include "core/assert/Assert.h"
    

namespace Garbox {

static bool sInitialized = false;

void I2cInstances::Init(){
    
    AssertExit(!sInitialized, "I2cInstances", "already initialized");

    GetI2c().init({
        .port = I2c::Port::Num0,
        .pinSda = PinSdaI2c,
        .pinScl = PinSclI2c,
        .pullSda = I2c::Pull::Disable,
        .pullScl = I2c::Pull::Disable,
        .frequencyHz = 100000,
    });

    sInitialized = true;
}

I2c& I2cInstances::GetI2c(){
    static I2c instance;
    return instance;
}

} // namespace