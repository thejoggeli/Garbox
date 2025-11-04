#include "PiezoPlayer.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global/AppConfig.h"
#include "parts/debugLeds/DebugLeds.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

PiezoPlayer::PiezoPlayer(Piezo& piezo, uint32_t defaultSilentTimeMicros): 
    mPiezo(piezo),
    mDefaultSilentTimeMicros(defaultSilentTimeMicros){
    // nothing to do
}

PiezoPlayer::~PiezoPlayer(){
    AssertExit(false, "PiezoPlayer", "deconstructor not implemented");
    // TODO stop task
}

void PiezoPlayer::init(){
    AssertExit(!mInitialized, "PiezoPlayer", "already initialized");

    // create mutex
    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "PiezoPlayer", "mutex creation failed");  

    // initialization complete
    mInitialized = true;

    // start task
    BaseType_t taskRes = xTaskCreatePinnedToCore(
        handleTask,
        AppConfig::PiezoTaskName,
        AppConfig::PiezoTaskStackSize,
        this,
        AppConfig::PiezoTaskPriority,
        nullptr,
        AppConfig::PiezoTaskCore
    );
    AssertExit(taskRes == pdPASS, "PiezoPlayer", "start task failed");
}

void PiezoPlayer::stop(){
    DebugLeds::SetLed(DebugLeds::Id::Custom2, false);
    mPiezo.setEnabled(false);
    mPlaying = false;
    mCurrentSequence = nullptr;
    mCurrentToneIndex = 0;
}

void PiezoPlayer::handleTask(void* self){
    const TickType_t cycleTime = pdMS_TO_TICKS(AppConfig::PiezoTaskDurationMillis);
    TickType_t lastWakeTime = xTaskGetTickCount();
    while(true){
        static_cast<PiezoPlayer*>(self)->tick();
        vTaskDelayUntil(&lastWakeTime, cycleTime);
    }
}

void PiezoPlayer::tick(){
    Garbox::LockGuard lock(mMutex);

    if (!mPlaying || !mCurrentSequence){
        return;
    }

    // get current time and  tone
    const uint32_t currentTimeMicros = Time::GetMicros();
    const uint32_t elapsedMicros = currentTimeMicros - mLastTimeMicros;
    const Tone& currentTone = mCurrentSequence->getTone(mCurrentToneIndex);

    // check if tone played its duration
    if (elapsedMicros >= currentTone.getDurationMicros()) {
        
        // start playing next tone in sequence
        mCurrentToneIndex++;
        mLastTimeMicros += currentTone.getDurationMicros();

        // end of sequence
        if (mCurrentToneIndex >= mCurrentSequence->getCount()) {
            playNextInQueue();
            return;
        }

        // start next tone
        const Tone& nextTone = mCurrentSequence->getTone(mCurrentToneIndex);
        if(nextTone.isSilent()){
            mPiezo.setEnabled(false); 
        }
        else if(currentTone.isMonotonic()){
            // start playing monotonic tone
            mPiezo.setFrequency(nextTone.getFrequencyStart());
            mPiezo.setDuty(nextTone.getDuty());
            mPiezo.setEnabled(true);
        }
        else {
            // start playing interpolated tone
            uint32_t nextElapsedMicros = currentTimeMicros - mLastTimeMicros;
            uint16_t frequency = interpolateFrequency(nextTone, nextElapsedMicros);
            mPiezo.setFrequency(frequency);
            mPiezo.setDuty(nextTone.getDuty());
            mPiezo.setEnabled(true);
        }
    }
    else {
        // updated current tone interpolation
        if(!currentTone.isMonotonic()){
            uint16_t frequency = interpolateFrequency(currentTone, elapsedMicros);
            mPiezo.setFrequency(frequency);
        }
    }
}

uint16_t PiezoPlayer::interpolateFrequency(Tone const& tone, uint32_t elapsedMicros){
    // start frequency
    if ((elapsedMicros == 0) || tone.isMonotonic())
        return tone.getFrequencyStart();

    // end frequency 
    if (elapsedMicros >= tone.getDurationMicros())
        return tone.getFrequencyEnd();

    // compute interpolated frequency as float
    const float f1 = static_cast<float>(tone.getFrequencyStart());
    const float f2 = static_cast<float>(tone.getFrequencyEnd());
    const float t = static_cast<float>(elapsedMicros) / static_cast<float>(tone.getDurationMicros());
    const float frequency = f1 + (f2 - f1) * t; 

    // cast back to uint16
    return static_cast<uint16_t>(frequency);
}

void PiezoPlayer::playSequence(const ToneSequence& sequence){
    Garbox::LockGuard lock(mMutex);
    playSequence(sequence, mDefaultSilentTimeMicros);
}

void PiezoPlayer::playSequence(const ToneSequence& sequence, uint32_t silentTimeMicros){
    Garbox::LockGuard lock(mMutex);
    if(sequence.getCount() == 0){
        AssertDebug(false, "PiezoPlayer", "invalid sequence tone count == 0");
        return;
    }

    // add sequence to queue
    bool result = mQueue.push(QueueItem{
        .type = QueueItemType::ToneSequence,
        .tone = Tone(0, 0),
        .sequence = &sequence,
    });
    if(!result){
        AssertDebug(false, "PiezoPlayer", "queue sequence failed");
        return;
    }

    // add silent time
    if(silentTimeMicros > 0){
        result = mQueue.push(QueueItem{
            .type = QueueItemType::SilentTime,
            .tone = Tone(silentTimeMicros),
            .sequence = nullptr,
        });
        if(!result){
            AssertDebug(false, "PiezoPlayer", "queue silence failed");
            return;
        }
    }

    // begin playing sequence
    if(!mPlaying){
        playNextInQueue();
    }
}

void PiezoPlayer::playTone(const Tone& tone){
    Garbox::LockGuard lock(mMutex);
    playTone(tone, mDefaultSilentTimeMicros);
}

void PiezoPlayer::playTone(const Tone& tone, uint32_t silentTimeMicros){
    Garbox::LockGuard lock(mMutex);

    // ensure there is enough space for tone + Silent time
    if(mQueue.available() < 2){
        return;
    }

    // add single tone to queue
    bool result = mQueue.push(QueueItem{
        .type = QueueItemType::SingleTone,
        .tone = tone,
        .sequence = nullptr,
    });
    if(!result){
        AssertDebug(false, "PiezoPlayer", "queue tone failed");
        return;
    }

    // add silent time
    if(silentTimeMicros > 0){
        result = mQueue.push(QueueItem{
            .type = QueueItemType::SilentTime,
            .tone = Tone(silentTimeMicros),
            .sequence = nullptr,
        });
        if(!result){
            AssertDebug(false, "PiezoPlayer", "queue silence failed");
            return;
        }
    }

    // begin playing sequence
    if(!mPlaying){
        playNextInQueue();
    }
}

void PiezoPlayer::playNextInQueue(){
    Garbox::LockGuard lock(mMutex);

    QueueItem* nextItem = mQueue.popPtr();
    if(nextItem != nullptr){

        // handle queue item type
        if(nextItem->type == QueueItemType::ToneSequence){
            mCurrentSequence = nextItem->sequence;
        }
        else if(nextItem->type == QueueItemType::SingleTone){ 
            mSingleTone = nextItem->tone;
            mCurrentSequence = &mSingleSequence;
        }
        else if(nextItem->type == QueueItemType::SilentTime){ 
            mSingleTone = nextItem->tone;
            mCurrentSequence = &mSingleSequence;
        }
        else {
            AssertDebug(false, "PiezoPlayer", "invalid queue item type");
            mCurrentSequence = nullptr;
        }

        // safety check
        if(mCurrentSequence == nullptr){
            AssertDebug(false, "PiezoPlayer", "sequence == nullptr");
            clearQueue();
            stop();
            return;
        }

        // initialize state for sequence playback
        DebugLeds::SetLed(DebugLeds::Id::Custom2, true);
        mCurrentToneIndex = 0;
        mPlaying = true;
        mLastTimeMicros = Time::GetMicros();

        // play initial tone of sequence
        const Tone& firstTone = mCurrentSequence->getTone(0);
        if(firstTone.isSilent()){
            mPiezo.setEnabled(false); 
        }
        else {
            mPiezo.setFrequency(firstTone.getFrequencyStart());
            mPiezo.setDuty(firstTone.getDuty());
            mPiezo.setEnabled(true);
        }
    }
    else {
        stop();
    }
}

bool PiezoPlayer::isPlaying() const {
    Garbox::LockGuard lock(mMutex);
    return mPlaying;
}

void PiezoPlayer::clearQueue(){
    Garbox::LockGuard lock(mMutex);
    mQueue.clear();
}

} // namespace Garbox
