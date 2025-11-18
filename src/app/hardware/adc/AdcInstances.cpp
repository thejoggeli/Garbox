#include "AdcInstances.h"

#include "app/config/PinConfig.h"
#include "core/assert/Assert.h"

namespace Garbox {

template<int32_t GpioNum>
static constexpr adc1_channel_t GpioToAdc1Channel(){
    switch(GpioNum){
    case 1:  return ADC1_CHANNEL_0;
    case 2:  return ADC1_CHANNEL_1;
    case 3:  return ADC1_CHANNEL_2;
    case 4:  return ADC1_CHANNEL_3;
    case 5:  return ADC1_CHANNEL_4;
    case 6:  return ADC1_CHANNEL_5;
    case 7:  return ADC1_CHANNEL_6;
    case 8:  return ADC1_CHANNEL_7;
    case 9:  return ADC1_CHANNEL_8;
    case 10: return ADC1_CHANNEL_9;
    }
    static_assert((GpioNum >= 1) && (GpioNum <= 10), "Invalid GPIO number for ADC1 channel on ESP32-S3");
}

static bool gInitialized = false;

void AdcInstances::Init(){
    
    AssertExit(!gInitialized, "SpiInstances", "already initialized");

    GetHeatpadVoltage().init(Adc::Config {
        .pin = PinConfig::HeatpadVoltageSense,
        .channel = GpioToAdc1Channel<PinConfig::HeatpadVoltageSense>(), 
        .attenuation = ADC_ATTEN_DB_6,
        .bitWidth = ADC_WIDTH_BIT_12,
        .enableCalibration = true,
    });

    GetHeatpadCurrent().init(Adc::Config {
        .pin = PinConfig::HeatpadCurrentSense,
        .channel = GpioToAdc1Channel<PinConfig::HeatpadCurrentSense>(),
        .attenuation = ADC_ATTEN_DB_12,
        .bitWidth = ADC_WIDTH_BIT_12,
        .enableCalibration = true,
    });

    gInitialized = true;
}

Adc& AdcInstances::GetHeatpadVoltage(){
    static Adc instance;
    return instance;
}

Adc& AdcInstances::GetHeatpadCurrent(){
    static Adc instance;
    return instance;
}

} // namespace Garbox
