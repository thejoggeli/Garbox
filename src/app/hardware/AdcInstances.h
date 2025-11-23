#pragma once

// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/hardware/adc/Adc.h"

namespace Garbox {

class AdcInstances {
public:

    static constexpr int32_t PinHeatpadVoltage = 8; 
    static constexpr int32_t PinHeatpadCurrent = 9; 

    static void Init();

    static Adc& GetHeatpadVoltage();
    static Adc& GetHeatpadCurrent();

private:

    AdcInstances() = delete;
    ~AdcInstances() = delete;
    AdcInstances(const AdcInstances&) = delete;
    AdcInstances& operator=(const AdcInstances&) = delete;
};

} // namespace