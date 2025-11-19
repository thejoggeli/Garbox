#pragma once

#include "driver/ledc.h"
#include "esp_err.h"
#include <cstdint>

namespace Garbox {

class LedcTimer;

class LedcChannel {
public:
    enum class Id : uint8_t {
        Channel0,
        Channel1,
        Channel2,
        Channel3,
        Channel4,
        Channel5,
        Channel6,
        Channel7,
    };

    LedcChannel();

    bool init(Id id, LedcTimer* timer, int32_t gpio, bool invert = false);
    bool setDutyRaw(uint32_t duty, bool update = true);
    bool setDutyRelative(float ratio, bool update = true);

    uint32_t getDuty() const;

    // disallow copy and move 
    LedcChannel(const LedcChannel&) = delete;
    LedcChannel& operator=(const LedcChannel&) = delete;
    LedcChannel(LedcChannel&&) = delete;
    LedcChannel& operator=(LedcChannel&&) = delete;

private:
    Id mId;
    LedcTimer* mTimer;
    int32_t mGpio;
    ledc_channel_t mChannelIndex;
    ledc_mode_t mMode;
    uint32_t mDuty;
    bool mInitialized;
};

} // namespace
