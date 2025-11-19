#pragma once

#include "core/hardware/i2c/I2c.h"

namespace Garbox {

struct I2cInstances {
public:

    static void Init();

    static I2c& GetI2c();

};

} // namespace
