#pragma once

#include <cstdint>
#include "driver/pcnt.h"
#include "esp_err.h"

namespace Garbox {

class TachoPulseCounter {
public:
    explicit TachoPulseCounter(
        uint32_t pin,
        pcnt_unit_t unit = PCNT_UNIT_0,
        pcnt_channel_t channel = PCNT_CHANNEL_0,
        int16_t minCount = -32768,
        int16_t maxCount =  32767
    );

    // configure PCNT for this pin (must be called before use)
    bool init();

    // clear counter and start counting
    bool start();

    // read current count (signed 16-bit in HW, we widen to int)
    int getCount() const;

    // clear counter to 0
    bool clearCount();

    // convenience: read then clear
    int getAndClearCount();

private:
    uint32_t        mPin;
    pcnt_unit_t     mUnit;
    pcnt_channel_t  mChannel;
    int16_t         mMinCount;
    int16_t         mMaxCount;
    bool            mInitialized = false;
};

} // namespace Garbox
