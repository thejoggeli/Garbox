#pragma once

#include <cstddef>
#include <cstdint>
#include "Piezo.h"
#include "ToneSequence.h"

namespace Garbox {

class PiezoPlayer {
public:
    explicit PiezoPlayer(Piezo& piezo);

    void playSequence(const ToneSequence& sequence);
    void stop();
    void tick();
    bool isPlaying() const;

private:
    Piezo& mPiezo;
    const ToneSequence* mCurrentSeq = nullptr;

    size_t mIndex = 0;
    uint32_t mLastTime = 0;
    bool mPlaying = false;
};

} // namespace Garbox
