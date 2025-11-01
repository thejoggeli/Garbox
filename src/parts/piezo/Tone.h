#pragma once
#include <cstdint>

namespace Garbox {

class Tone {
public:

    // silent tone
    constexpr Tone(uint32_t durationMicros): 
        mDurationMicros(durationMicros),
        mFrequencyStart(0), 
        mFrequencyEnd(0),
        mDuty(0.5f){
        // nothing to do
    }

    constexpr Tone(uint32_t durationMicros, uint16_t frequency): 
        mDurationMicros(durationMicros),
        mFrequencyStart(frequency), 
        mFrequencyEnd(frequency),
        mDuty(0.5f){
        // nothing to do
    }

    constexpr Tone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd): 
        mDurationMicros(durationMicros), 
        mFrequencyStart(frequencyStart), 
        mFrequencyEnd(frequencyEnd),
        mDuty(0.5f){
        // nothing to do
    }

    constexpr Tone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd, float duty): 
        mDurationMicros(durationMicros), 
        mFrequencyStart(frequencyStart), 
        mFrequencyEnd(frequencyEnd),
        mDuty(duty){
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

    constexpr float getDuty() const {
        return mDuty;
    }

    constexpr bool isMonotonic() const {
        return (mFrequencyStart == mFrequencyEnd);
    }

    constexpr bool isSilent() const {
        return (mFrequencyStart == 0) || (mFrequencyEnd == 0);
    }

private:

    uint32_t mDurationMicros;
    uint16_t mFrequencyStart;
    uint16_t mFrequencyEnd;
    float mDuty;

};

} // namespace Garbox
