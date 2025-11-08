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

    // constant frequency tone
    constexpr Tone(uint32_t durationMicros, uint16_t frequency): 
        mDurationMicros(durationMicros),
        mFrequencyStart(frequency), 
        mFrequencyEnd(frequency),
        mDuty(0.5f){
        // nothing to do
    }

    // frequency sweep tone
    constexpr Tone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd): 
        mDurationMicros(durationMicros), 
        mFrequencyStart(frequencyStart), 
        mFrequencyEnd(frequencyEnd),
        mDuty(0.5f){
        // nothing to do
    }

    // sweep tone with duty control
    constexpr Tone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd, float duty): 
        mDurationMicros(durationMicros), 
        mFrequencyStart(frequencyStart), 
        mFrequencyEnd(frequencyEnd),
        mDuty(duty){
        // nothing to do
    }

    // chainable frequency modifier (const and non-const)
    constexpr Tone frequency(uint16_t frequencyStart) const {
        Tone t = *this;
        t.mFrequencyStart = frequencyStart;
        t.mFrequencyEnd   = frequencyStart;
        return t;
    }

    constexpr Tone& frequency(uint16_t frequencyStart) {
        mFrequencyStart = frequencyStart;
        mFrequencyEnd   = frequencyStart;
        return *this;
    }

    // chainable sweep modifier (const and non-const)
    constexpr Tone sweep(uint16_t frequencyStart, uint16_t frequencyEnd) const {
        Tone t = *this;
        t.mFrequencyStart = frequencyStart;
        t.mFrequencyEnd = frequencyEnd;
        return t;
    }

    constexpr Tone& sweep(uint16_t frequencyStart, uint16_t frequencyEnd) {
        mFrequencyStart = frequencyStart;
        mFrequencyEnd = frequencyEnd;
        return *this;
    }

    // chainable duty modifier (const and non-const)
    constexpr Tone duty(float duty) const {
        Tone t = *this;
        t.mDuty = duty;
        return t;
    }

    constexpr Tone& duty(float duty) {
        mDuty = duty;
        return *this;
    }

    // chainable silent modifier (const and non-const)
    constexpr Tone silent() const {
        Tone t = *this;
        t.mFrequencyStart = 0;
        t.mFrequencyEnd   = 0;
        return t;
    }

    constexpr Tone& silent() {
        mFrequencyStart = 0;
        mFrequencyEnd   = 0;
        return *this;
    }

    // accessors
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
