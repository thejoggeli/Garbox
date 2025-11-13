#pragma once

#include <cstddef>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "Piezo.h"
#include "ToneSequence.h"
#include "util/container/RingBuffer.h"

namespace Garbox {

class LedcTimer;
class LedcChannel;

class PiezoPlayer {
public:
    explicit PiezoPlayer(LedcTimer& timer, LedcChannel& channel);
    ~PiezoPlayer();

    void init(uint32_t defaultSilentTimeMicros = 50'000);
    void stop();
    void clearQueue();

    void playSequence(const ToneSequence& sequence);
    void playSequence(const ToneSequence& sequence, uint32_t SilentTime);
    void playTone(const Tone& tone);
    void playTone(const Tone& tone, uint32_t SilentTime);
    bool isPlaying() const;

    // Disallow copy and move 
    PiezoPlayer(const PiezoPlayer&) = delete;
    PiezoPlayer& operator=(const PiezoPlayer&) = delete;
    PiezoPlayer(PiezoPlayer&&) = delete;
    PiezoPlayer& operator=(PiezoPlayer&&) = delete;

private:

    static uint16_t interpolateFrequency(Tone const& tone, uint32_t elapsedMicros);

    void playNextInQueue();

    static void handleTask(void* player);
    void tick();

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

    Piezo mPiezo;
    uint32_t mDefaultSilentTimeMicros = 0;

    Tone mSingleTone = Tone(0, 0); 
    const ToneSequence mSingleSequence = ToneSequence(&mSingleTone, 1);
    const ToneSequence* mCurrentSequence = nullptr;

    RingBuffer<QueueItem, QueueSize> mQueue;

    size_t mCurrentToneIndex = 0;
    uint32_t mLastTimeMicros = 0;
    bool mPlaying = false;
    bool mInitialized = false;
    SemaphoreHandle_t mMutex = nullptr;
    TaskHandle_t mTaskHandle = nullptr;
};

} // namespace Garbox
