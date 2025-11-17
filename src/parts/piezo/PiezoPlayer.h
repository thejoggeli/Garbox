#pragma once


#include <cstddef>
#include <cstdint>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "core/rtos/Task.h"
#include "parts/piezo/Piezo.h"
#include "parts/piezo/ToneSequence.h"
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

    // periodc task
    void startTask(const char* taskName, uint32_t frequencyHz, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTask();
    TaskHandle_t getTaskHandle();

    // directly piezo control, bypassing the player logic. should only be used when the task is not running 
    void setPiezoEnabled(bool enabled);
    void setPiezoTone(uint32_t frequency, float duty); 
    void setPiezoTone(const Tone& tone); 

    // playback
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
    bool mInitialized = false;

    // tone sequence
    Tone mSingleTone = Tone(0, 0); 
    const ToneSequence mSingleSequence = ToneSequence(&mSingleTone, 1);
    const ToneSequence* mCurrentSequence = nullptr;

    // playback state
    RingBuffer<QueueItem, QueueSize> mQueue;
    size_t mCurrentToneIndex = 0;
    uint32_t mLastTimeMicros = 0;
    bool mPlaying = false;

    // mutex
    SemaphoreHandle_t mMutex = nullptr;

    // playback task
    Task mTask;
    uint32_t mTaskFrequencyHz = 0;

    // playback
    void playbackTick();
    void playNextInQueue();
    static uint16_t interpolateFrequency(Tone const& tone, uint32_t elapsedMicros);

    // playback task
    void handleTask();
    static void taskTrampoline(void* self);
};

} // namespace Garbox
