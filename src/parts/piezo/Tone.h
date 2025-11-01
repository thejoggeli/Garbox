#pragma once
#include <cstdint>

namespace Garbox {

class Tone {
public:

    constexpr Tone(uint32_t durationMicros, uint16_t frequency) : 
        mFrequencyStart(frequency), 
        mFrequencyEnd(frequency), 
        mDurationMicros(durationMicros),
        mMonotonic(true){
        // nothing to do
    }

    constexpr Tone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd) : 
        mFrequencyStart(frequencyStart), 
        mFrequencyEnd(frequencyEnd), 
        mDurationMicros(durationMicros), 
        mMonotonic(false){
        // nothing to do
    }

    constexpr uint16_t getFrequencyStart() const { 
        return mFrequencyStart; 
    }

    constexpr uint16_t getFrequencyEnd() const { 
        return mFrequencyEnd; 
    }

    constexpr uint32_t getDurationMicros() const { 
        return mDurationMicros; 
    }

    constexpr bool isMonotonic() const {
        return mMonotonic;
    }

private:

    uint16_t mFrequencyStart;
    uint16_t mFrequencyEnd;
    uint32_t mDurationMicros;
    bool mMonotonic;

};

} // namespace Garbox
