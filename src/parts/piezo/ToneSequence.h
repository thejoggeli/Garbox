#pragma once
#include <cstddef>
#include <initializer_list>
#include "Tone.h"

namespace Garbox {

class ToneSequence {
public:

    constexpr ToneSequence(const Tone* tones, size_t count): 
        // init members
        mTones(tones), 
        mCount(count) {
        // nothing to do
    }

    constexpr const Tone& getTone(size_t index) const { return mTones[index]; }
    constexpr const Tone* getTones() const { return mTones; }
    constexpr size_t getCount() const { return mCount; }

private:
    const Tone* mTones;
    size_t mCount;
};

} // namespace Garbox
