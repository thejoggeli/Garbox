#pragma once

#include <cstddef>
#include <cstdint>
#include "Piezo.h"
#include "ToneSequence.h"

namespace Garbox {

class PiezoPlayer {
public:
    explicit PiezoPlayer(Piezo& piezo);

    void stop();
    void tick();

    void playTone(const Tone& tone);
    void playTone(uint32_t durationMicros, uint16_t frequency);
    void playTone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd);
    void playSequence(const ToneSequence& sequence);
    bool isPlaying() const;

private:

    static uint16_t interpolateFrequency(Tone const& tone, uint32_t elapsedMicros);

    Piezo& mPiezo;

    Tone mSingleTone = Tone(0, 0); 
    ToneSequence mSingleSequence = ToneSequence(&mSingleTone, 1);

    const ToneSequence* mCurrentSequence = nullptr;

    size_t mCurrentToneIndex = 0;
    uint32_t mLastTimeMicros = 0;
    bool mPlaying = false;
};

} // namespace Garbox
