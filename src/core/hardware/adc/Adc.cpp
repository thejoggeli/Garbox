#include "Adc.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "esp_err.h"

namespace Garbox {

Adc::Adc(){
    // nothing to do
}

void Adc::init(const Config& config){
    AssertExit(!mInitialized, "Adc", "already initialized");
    AssertExit((config.pin >= 0), "Adc", "invalid pin number");

    mPin = config.pin;
    mChannel = config.channel;
    mAttenuation = config.attenuation;
    mBitWidth = config.bitWidth;
    mVrefMillivolts = config.vrefMillivolts;

    if(adc1_config_width(mBitWidth) != ESP_OK){
        TriggerExit("Adc", "adc1_config_width failed");
    }

    if(adc1_config_channel_atten(mChannel, mAttenuation) != ESP_OK){
        TriggerExit("Adc", "adc1_config_channel_atten failed");
    }

    // Always characterize calibration data, even if eFuse values are missing
    if(config.enableCalibration){
        esp_adc_cal_value_t calType = esp_adc_cal_characterize(
            ADC_UNIT_1,
            mAttenuation,
            mBitWidth,
            mVrefMillivolts,
            &mAdcChars
        );

        switch(calType){
        case ESP_ADC_CAL_VAL_EFUSE_TP:
        case ESP_ADC_CAL_VAL_EFUSE_VREF:
        case ESP_ADC_CAL_VAL_EFUSE_TP_FIT:
        case ESP_ADC_CAL_VAL_DEFAULT_VREF:
            mCalibrated = true;
            break;
        default:
            mCalibrated = false;
            break;
        }
    }

    mInitialized = true;
}

void Adc::sample(){
    if(!mInitialized){
        TriggerDebug("Adc", "not initialized");
        return;
    }

    int rawValue = adc1_get_raw(mChannel);
    if(rawValue < 0){
        TriggerDebug("Adc", "adc1_get_raw failed");
        return;
    }

    mLastRaw = static_cast<uint16_t>(rawValue);
    mLastVolts = convertRawToVoltage(mLastRaw);
}

uint16_t Adc::getRaw() const {
    if(!mInitialized){
        TriggerDebug("Adc", "not initialized");
        return 0;
    }
    return mLastRaw;
}

float Adc::getVolts() const {
    if(!mInitialized){
        TriggerDebug("Adc", "not initialized");
        return 0.0f;
    }
    return mLastVolts;
}

bool Adc::isCalibrated() const {
    return mCalibrated;
}

float Adc::convertRawToVoltage(uint16_t raw) const {
    if(mCalibrated){
        uint32_t millivolts = esp_adc_cal_raw_to_voltage(raw, &mAdcChars);
        return static_cast<float>(millivolts) / 1000.0f;
    }

    // fallback approximate conversion
    const uint16_t maxValue = (1U << mBitWidth) - 1U;
    float vrefVolts = 3.3f;

    switch(mAttenuation){
    case ADC_ATTEN_DB_0:
        vrefVolts = 0.95f;
        break;
    case ADC_ATTEN_DB_2_5:
        vrefVolts = 1.25f;
        break;
    case ADC_ATTEN_DB_6:
        vrefVolts = 1.75f;
        break;
    case ADC_ATTEN_DB_12:
        vrefVolts = 3.10f;
        break;
    default:
        vrefVolts = 3.3f;
        break;
    }

    return (static_cast<float>(raw) / static_cast<float>(maxValue)) * vrefVolts;
}

} // namespace Garbox
