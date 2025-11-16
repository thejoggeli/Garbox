#include "PiezoPlayer.h"

#include "assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global/config/AppConfig.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

PiezoPlayer::PiezoPlayer(LedcTimer& pwmTimer, LedcChannel& pwmChannel): 
    mPiezo(pwmTimer, pwmChannel){
    // nothing to do
}

PiezoPlayer::~PiezoPlayer(){
    TriggerExit("PiezoPlayer", "deconstructor not implemented");
}

void PiezoPlayer::init(uint32_t defaultSilentTimeMicros){
    AssertExit(!mInitialized, "PiezoPlayer", "already initialized");

    // init members
    mDefaultSilentTimeMicros = defaultSilentTimeMicros;

    // init piezo
    mPiezo.init();

    // create mutex
    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "PiezoPlayer", "mutex creation failed");  

    // initialization complete
    mInitialized = true;
}

void PiezoPlayer::startTask(const char* name, uint32_t frequencyHz, uint32_t stackSize, UBaseType_t priority, BaseType_t core){
    LockGuard lock(mMutex);
    AssertExit(mInitialized, "PiezoPlayer", "not initialized");
    AssertExit(mTaskHandle == nullptr, "PiezoPlayer", "task already running");
    AssertExit(frequencyHz > 0, "PiezoPlayer", "task frequency must be > 0");
    BaseType_t result = xTaskCreatePinnedToCore(
        taskTrampoline,
        name, 
        stackSize,
        this,
        priority,
        &mTaskHandle,
        core
    );
    AssertExit(result == pdPASS, "PiezoPlayer", "task creation failed");
    mTaskFrequencyHz = frequencyHz;
}

void PiezoPlayer::stopTask(){
    LockGuard lock(mMutex);

    // check if task running
    if(mTaskHandle == nullptr){
        return;
    }

    // stop playing
    if(mPlaying){
        stop();
    }

    // stop task
    vTaskDelete(mTaskHandle);
    mTaskHandle = nullptr;
}

bool PiezoPlayer::isTaskRunning(){
    return mTaskHandle != nullptr;
}

TaskHandle_t PiezoPlayer::getTaskHandle(){
    return mTaskHandle;
}

void PiezoPlayer::stop(){
    mPiezo.setEnabled(false);
    mPlaying = false;
    mCurrentSequence = nullptr;
    mCurrentToneIndex = 0;
    clearQueue();
}

void PiezoPlayer::handleTask(){

    // task timing
    const TickType_t periodMillis = static_cast<uint32_t>(1000.0f / static_cast<float>(mTaskFrequencyHz));
    TickType_t lastWake = xTaskGetTickCount();

    while(true){
        if (!isPlaying()){
            // Sleep until playTone() / playSequence() notifies us
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            // Re-sync wake time so the next loop starts fresh
            lastWake = xTaskGetTickCount();
        }

        // Update ongoing playback
        playbackTick();

        // Periodic timing while playing
        vTaskDelayUntil(&lastWake, periodMillis);
    }
}

void PiezoPlayer::taskTrampoline(void* player){
    // get self
    AssertExit(player != nullptr, "PiezoPlayer", "player is nullptr");
    static_cast<PiezoPlayer*>(player)->handleTask();
}

void PiezoPlayer::playbackTick(){
    Garbox::LockGuard lock(mMutex);

    Profiler::Scoped(ProfilerConfig::PiezoPlayerTick);

    if (!mPlaying || !mCurrentSequence){
        return;
    }

    // get current time and  tone
    const uint32_t currentTimeMicros = Time::GetMicros();
    const uint32_t elapsedMicros = currentTimeMicros - mLastTimeMicros;
    const Tone& currentTone = mCurrentSequence->getTone(mCurrentToneIndex);

    // check if tone played its duration
    if (elapsedMicros >= currentTone.getDurationMicros()){
        
        // start playing next tone in sequence
        mCurrentToneIndex++;
        mLastTimeMicros += currentTone.getDurationMicros();

        // end of sequence
        if (mCurrentToneIndex >= mCurrentSequence->getCount()){
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

void PiezoPlayer::setPiezoEnabled(bool enabled){
    if(!mInitialized || mTaskHandle){
        TriggerDebug("PiezoPlayer", "setPiezoEnabled invalid state");
        return;
    }
    mPiezo.setEnabled(enabled);
}

void PiezoPlayer::setPiezoTone(uint32_t frequency, float duty){
    if(!mInitialized || !isTaskRunning()){
        TriggerDebug("PiezoPlayer", "setPiezoTone invalid state");
        return;
    }
    if(frequency == 0 || duty <= 0.0f || duty >= 1.0f){
        mPiezo.setEnabled(false);
    }
    else {
        mPiezo.setEnabled(true);
        mPiezo.setFrequency(frequency);
        mPiezo.setDuty(duty);
    }
}

void PiezoPlayer::setPiezoTone(const Tone& tone){
    setPiezoTone(tone.getFrequencyStart(), tone.getDuty());
}

void PiezoPlayer::playSequence(const ToneSequence& sequence){
    Garbox::LockGuard lock(mMutex);
    playSequence(sequence, mDefaultSilentTimeMicros);
}

void PiezoPlayer::playSequence(const ToneSequence& sequence, uint32_t silentTimeMicros){
    Garbox::LockGuard lock(mMutex);
    if(!mInitialized || !isTaskRunning()){
        TriggerDebug("PiezoPlayer", "invalid state");
        return;
    }
    
    if(sequence.getCount() == 0){
        TriggerDebug("PiezoPlayer", "invalid sequence tone count == 0");
        return;
    }

    // add sequence to queue
    bool result = mQueue.push(QueueItem{
        .type = QueueItemType::ToneSequence,
        .tone = Tone(0, 0),
        .sequence = &sequence,
    });
    if(!result){
        TriggerDebug("PiezoPlayer", "queue sequence failed");
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
            TriggerDebug("PiezoPlayer", "queue silence failed");
            return;
        }
    }

    // begin playing sequence
    if(!mPlaying){
        playNextInQueue();
        xTaskNotifyGive(mTaskHandle);
    }
}

void PiezoPlayer::playTone(const Tone& tone){
    Garbox::LockGuard lock(mMutex);
    playTone(tone, mDefaultSilentTimeMicros);
}

void PiezoPlayer::playTone(const Tone& tone, uint32_t silentTimeMicros){
    Garbox::LockGuard lock(mMutex);
    if(!mInitialized || !isTaskRunning()){
        TriggerDebug("PiezoPlayer", "invalid state");
        return;
    }

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
        TriggerDebug("PiezoPlayer", "queue tone failed");
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
            TriggerDebug("PiezoPlayer", "queue silence failed");
            return;
        }
    }

    // begin playing sequence
    if(!mPlaying){
        playNextInQueue();
        xTaskNotifyGive(mTaskHandle);
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
            TriggerDebug("PiezoPlayer", "invalid queue item type");
            mCurrentSequence = nullptr;
        }

        // safety check
        if(mCurrentSequence == nullptr){
            TriggerDebug("PiezoPlayer", "sequence == nullptr");
            clearQueue();
            stop();
            return;
        }

        // initialize state for sequence playback
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
