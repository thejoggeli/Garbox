#pragma once

#include <cstddef>
#include <cstdint>
#include "Piezo.h"
#include "ToneSequence.h"
#include "util/array/RingBuffer.h"

namespace Garbox {

class PiezoPlayer {
public:
    explicit PiezoPlayer(Piezo& piezo, uint32_t deadTimeMicros = 50'000);

    void stop();
    void tick();
    void clearQueue();

    void playSequence(const ToneSequence& sequence);
    void playTone(const Tone& tone);
    void playTone(uint32_t durationMicros, uint16_t frequency);
    void playTone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd);
    bool isPlaying() const;

    void addDeadTime(uint32_t deadTimeMicros);

private:

    static uint16_t interpolateFrequency(Tone const& tone, uint32_t elapsedMicros);

    bool checkQueueCapacity();
    void playNextInQueue();

    static constexpr size_t QueueSize = 20; // also includes silent tones for dead time

    enum class QueueItemType : uint8_t {
        SingleTone = 0,
        ToneSequence,
        DeadTime
    };

    struct QueueItem {
        QueueItemType type;
        Tone tone = Tone(0, 0);
        const ToneSequence* sequence = nullptr;
    };

    Piezo& mPiezo;
    uint32_t mDeadTimeMicros;

    Tone mSingleTone = Tone(0, 0); 
    const ToneSequence mSingleSequence = ToneSequence(&mSingleTone, 1);
    const ToneSequence* mCurrentSequence = nullptr;

    RingBuffer<QueueItem, QueueSize> mQueue;

    size_t mCurrentToneIndex = 0;
    uint32_t mLastTimeMicros = 0;
    bool mPlaying = false;
};

} // namespace Garbox
