#pragma once

#include <cstdint>
#include "driver/ledc.h"
#include "esp_err.h"

namespace Garbox {

class LedcTimer {

public:

    enum class Id : uint8_t {
        Timer0 = 0, 
        Timer1,
        Timer2,
        Timer3
    };

    explicit LedcTimer();

    bool setup(Id id, uint32_t frequencyHz, uint8_t resolutionBits);
    bool setFrequency(uint32_t frequencyHz) const;

    Id getId() const;
    ledc_mode_t getMode() const;
    ledc_timer_t getIndex() const;
    uint8_t getResolutionBits() const;
    uint32_t getMaxDuty() const;

    // Disallow copy and move
    LedcTimer(const LedcTimer&) = delete;
    LedcTimer& operator=(const LedcTimer&) = delete;
    LedcTimer(LedcTimer&&) = delete;
    LedcTimer& operator=(LedcTimer&&) = delete;

private:

    Id mId;
    ledc_mode_t mMode;
    ledc_timer_t mIndex;
    uint8_t mResolutionBits;
    uint32_t mMaxDuty;
    bool mInitialized;

};

} // namespace
