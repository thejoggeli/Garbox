#pragma once

#include <Arduino.h>
#include <cstdint>

namespace Garbox {

/**
 * @brief Hardware PWM driver using ESP32-S3 LEDC peripheral.
 *
 * The ESP32-S3 LEDC controller provides up to 8 PWM channels, each with
 * configurable frequency and resolution. Frequency and resolution are
 * inversely related — higher frequency means fewer available duty steps.
 *
 * The PWM frequency (f_PWM) is derived from the 80 MHz APB clock:
 *
 *     f_PWM = 80 MHz / (2^resolutionBits)
 *
 * Approximate maximum frequencies for each duty resolution:
 *   Resolution | Max Frequency
 *   -----------|---------------
 *       8-bit  | 312 kHz
 *       9-bit  | 156 kHz
 *      10-bit  | 78 kHz
 *      11-bit  | 39 kHz
 *      12-bit  | 19 kHz
 *      13-bit  | 9.8 kHz
 *      14-bit  | 4.9 kHz
 *      15-bit  | 2.4 kHz
 *
 * Example:
 *   - For a 25 kHz fan PWM, use 8–10 bit resolution.
 *   - For smooth LED dimming, use lower frequency (~2–5 kHz) and 10–12 bits.
 * 
 * Official documentation: “LED Control (LEDC) – ESP32-S3” at
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/ledc.html
 */

class LedcPwm {
public:

    enum class Channel : uint8_t {
        Channel0 = 0,
        Channel1,
        Channel2,
        Channel3,
        Channel4,
        Channel5,
        Channel6,
        Channel7
    };

    explicit LedcPwm(
        uint32_t pin,
        Channel channel,
        uint32_t frequency = 25000,
        uint8_t resolutionBits = 8
    );

    // setup
    void init();

    // control
    void setDutyRaw(uint32_t duty);
    void setDutyNormalized(float percent);
    void setFrequency(uint32_t frequency, uint8_t mResolution);

    // query
    uint32_t getDuty() const;
    uint32_t getFrequency() const;
    uint32_t getMaxDuty() const;
    uint8_t getResolution() const;

private:
    uint32_t mPin;
    Channel  mChannel;
    uint32_t mFrequency;
    uint8_t  mResolution;
    uint8_t  mDuty = 0;
    uint32_t mMaxDuty = 0;
    float    mMaxDutyFloat = 0.0f;
};

}  // namespace Garbox
