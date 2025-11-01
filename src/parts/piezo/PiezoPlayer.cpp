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
        
        // start playing next tone in sequence
        mCurrentToneIndex++;
        mLastTimeMicros += currentTone.getDurationMicros();

        // end of sequence
        if (mCurrentToneIndex >= mCurrentSequence->getCount()) {
            stop();
            return;
        }

        // start next tone
        const Tone& nextTone = mCurrentSequence->getTones()[mCurrentToneIndex];
        const bool isFrequencyValid = (nextTone.getFrequencyStart() > 0) && (nextTone.getFrequencyEnd() > 0);
        if(!isFrequencyValid){
            mPiezo.setEnabled(false); 
        }
        else if(currentTone.isMonotonic()){
            // start playing monotonic tone
            mPiezo.setFrequency(nextTone.getFrequencyStart());
            mPiezo.setEnabled(true);
        }
        else {
            // start playing interpolated tone
            uint32_t nextElapsedMicros = currentTimeMicros - mLastTimeMicros;
            uint16_t frequency = interpolateFrequency(nextTone, nextElapsedMicros);
            mPiezo.setFrequency(frequency);
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

uint16_t PiezoPlayer::interpolateFrequency(Tone const& tone, uint32_t elapsedMicros){

    // start frequency
    if (tone.getDurationMicros() == 0 || tone.isMonotonic())
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

} // namespace Garbox
