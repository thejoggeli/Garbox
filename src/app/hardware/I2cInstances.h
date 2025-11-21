// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/hardware/i2c/I2c.h"

namespace Garbox {

struct I2cInstances {
public:

    static constexpr int32_t PinSdaI2c = 7; 
    static constexpr int32_t PinSclI2c = 6; 

    static void Init();

    static I2c& GetI2c();

};

} // namespace