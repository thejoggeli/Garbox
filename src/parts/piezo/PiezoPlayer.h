#pragma once

#include <cstddef>
#include <cstdint>
#include "Piezo.h"
#include "ToneSequence.h"
#include "util/array/RingBuffer.h"

namespace Garbox {

class PiezoPlayer {
public:
    explicit PiezoPlayer(Piezo& piezo, uint32_t defaultSilentTimeMicros = 50'000);

    void stop();
    void tick();
    void clearQueue();

    void playSequence(const ToneSequence& sequence);
    void playSequence(const ToneSequence& sequence, uint32_t SilentTime);
    void playTone(const Tone& tone);
    void playTone(const Tone& tone, uint32_t SilentTime);
    bool isPlaying() const;

private:

    static uint16_t interpolateFrequency(Tone const& tone, uint32_t elapsedMicros);

    void playNextInQueue();

    static constexpr size_t QueueSize = 20; // also includes silent tones for Silent time

    enum class QueueItemType : uint8_t {
        SingleTone = 0,
        ToneSequence,
        SilentTime
    };

    struct QueueItem {
        QueueItemType type;
        Tone tone = Tone(0, 0);
        const ToneSequence* sequence = nullptr;
    };

    Piezo& mPiezo;
    uint32_t mDefaultSilentTimeMicros;

    Tone mSingleTone = Tone(0, 0); 
    const ToneSequence mSingleSequence = ToneSequence(&mSingleTone, 1);
    const ToneSequence* mCurrentSequence = nullptr;

    RingBuffer<QueueItem, QueueSize> mQueue;

    size_t mCurrentToneIndex = 0;
    uint32_t mLastTimeMicros = 0;
    bool mPlaying = false;
};

} // namespace Garbox
