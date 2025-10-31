#pragma once
#include <cstdint>

namespace Garbox {

class Tone {
public:

    constexpr Tone(uint16_t freqHz, uint32_t durationMicros) : mFreq(freqHz), mDurationMicros(durationMicros) {}

    constexpr uint16_t freq() const { return mFreq; }
    constexpr uint32_t durationMicros() const { return mDurationMicros; }

private:

    uint16_t mFreq;
    uint32_t mDurationMicros;
    
};

} // namespace Garbox
