#include "PiezoPlayer.h"

#include "core/time/Time.h"

namespace Garbox {

PiezoPlayer::PiezoPlayer(Piezo& piezo) : mPiezo(piezo){
    // nothing to do
}

void PiezoPlayer::playSequence(const ToneSequence& sequence){
    stop();

    mCurrentSeq = &sequence;
    mIndex = 0;
    mPlaying = true;
    mLastTime = Time::GetMicros();

    const auto& tone = sequence.tones()[0];
    mPiezo.setFrequency(tone.freq());
    mPiezo.setEnabled(true);
}

void PiezoPlayer::stop(){
    if (mPlaying) {
        mPiezo.setEnabled(false);
    }
    mPlaying = false;
    mCurrentSeq = nullptr;
    mIndex = 0;
}

void PiezoPlayer::tick(){
    if (!mPlaying || !mCurrentSeq){
        return;
    }

    const uint32_t now = Time::GetMicros();
    const uint32_t elapsed = now - mLastTime;

    const auto& tone = mCurrentSeq->tones()[mIndex];
    if (elapsed >= tone.durationMicros()) {
        mIndex++;
        mLastTime = now;

        if (mIndex >= mCurrentSeq->count()) {
            stop();
            return;
        }

        const auto& next = mCurrentSeq->tones()[mIndex];
        mPiezo.setFrequency(next.freq());
        mPiezo.setEnabled(next.freq() > 0);
    }
}

bool PiezoPlayer::isPlaying() const {
    return mPlaying;
}

} // namespace Garbox
