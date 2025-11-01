#include "PiezoPlayer.h"

#include "core/time/Time.h"

namespace Garbox {

PiezoPlayer::PiezoPlayer(Piezo& piezo) : mPiezo(piezo) {
    // nothing to do
}

void PiezoPlayer::stop(){
    if (mPlaying) {
        mPiezo.setEnabled(false);
    }
    mPlaying = false;
    mCurrentSequence = nullptr;
    mCurrentToneIndex = 0;
}

void PiezoPlayer::tick(){
    if (!mPlaying || !mCurrentSequence){
        return;
    }

    // get current time and  tone
    const uint32_t currentTimeMicros = Time::GetMicros();
    const uint32_t elapsedMicros = currentTimeMicros - mLastTimeMicros;
    const Tone& currentTone = mCurrentSequence->getTones()[mCurrentToneIndex];

    // check if tone played its duration
    if (elapsedMicros >= currentTone.getDurationMicros()) {
        
        mCurrentToneIndex++;
        mLastTimeMicros = currentTimeMicros;

        // end of sequence
        if (mCurrentToneIndex >= mCurrentSequence->getCount()) {
            stop();
            return;
        }

        // start next tone
        const Tone& nextTone = mCurrentSequence->getTones()[mCurrentToneIndex];
        const uint32_t nextFrequency = nextTone.getFrequencyStart();
        if(nextFrequency == 0){
            mPiezo.setEnabled(false);
        }
        else {
            mPiezo.setFrequency(nextTone.getFrequencyStart());
            mPiezo.setEnabled(true);
        }
    }
}

void PiezoPlayer::playTone(const Tone& tone){
    mSingleTone = tone;
    playSequence(mSingleSequence);
}

void PiezoPlayer::playTone(uint32_t durationMicros, uint16_t frequency){
    mSingleTone = Tone(durationMicros, frequency);
    playSequence(mSingleSequence);
}

void PiezoPlayer::playTone(uint32_t durationMicros, uint16_t frequencyStart, uint16_t frequencyEnd){
    mSingleTone = Tone(durationMicros, frequencyStart, frequencyEnd);
    playSequence(mSingleSequence);
}

void PiezoPlayer::playSequence(const ToneSequence& sequence){
    mCurrentSequence = &sequence;
    mCurrentToneIndex = 0;
    mPlaying = true;
    mLastTimeMicros = Time::GetMicros();

    const auto& currentTone = sequence.getTones()[0];
    mPiezo.setFrequency(currentTone.getFrequencyStart());
    mPiezo.setEnabled(true);
}

bool PiezoPlayer::isPlaying() const {
    return mPlaying;
}

} // namespace Garbox
