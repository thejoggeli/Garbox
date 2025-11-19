#pragma once

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <cstdint>

namespace Garbox {

// -----------------------------------------------------------------------------
// ADC attenuation overview (ESP32-S3 typical input ranges)
//
// Attenuation      Measurable input voltage range
// -----------------------------------------------
// ADC_ATTEN_DB_0   : 0 mV  ~  950 mV
// ADC_ATTEN_DB_2_5 : 0 mV  ~ 1250 mV
// ADC_ATTEN_DB_6   : 0 mV  ~ 1750 mV
// ADC_ATTEN_DB_12  : 0 mV  ~ 3100 mV
//
// Note: Choose attenuation according to expected input voltage range.
// Higher attenuation allows higher measurable voltages but reduces precision.
// -----------------------------------------------------------------------------

class Adc {
public:
    struct Config {
        int32_t pin;
        adc1_channel_t channel;
        adc_atten_t attenuation = ADC_ATTEN_DB_12;
        adc_bits_width_t bitWidth = ADC_WIDTH_BIT_12;
        bool enableCalibration = true;        // calibration is safe and recommended
        uint32_t vrefMillivolts = 1100;       // used if eFuse data not found
    };

    Adc();

    void init(const Config& config);
    void sample();

    uint16_t getRaw() const;
    float getVolts() const;
    bool isCalibrated() const;

    // disallow copy and move 
    Adc(const Adc&) = delete;
    Adc& operator=(const Adc&) = delete;
    Adc(Adc&&) = delete;
    Adc& operator=(Adc&&) = delete;

private:
    int32_t mPin = -1;
    adc1_channel_t mChannel = ADC1_CHANNEL_0;
    adc_atten_t mAttenuation = ADC_ATTEN_DB_12;
    adc_bits_width_t mBitWidth = ADC_WIDTH_BIT_12;
    uint32_t mVrefMillivolts = 1100;

    bool mInitialized = false;
    bool mCalibrated = false;

    uint16_t mLastRaw = 0;
    float mLastVolts = 0.0f;

    esp_adc_cal_characteristics_t mAdcChars = {};

    float convertRawToVoltage(uint16_t raw) const;
};

} // namespace Garbox
