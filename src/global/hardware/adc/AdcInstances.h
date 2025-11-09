#pragma once

#include "core/hardware/adc/Adc.h"

namespace Garbox {

class AdcInstances {
public:

    static void Init();

    static Adc& GetVoltageSense();
    static Adc& GetCurrentSense();

private:

    AdcInstances() = delete;
    ~AdcInstances() = delete;
    AdcInstances(const AdcInstances&) = delete;
    AdcInstances& operator=(const AdcInstances&) = delete;
};

} // namespace Garbox
