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

    constexpr const Tone* tones() const { return mTones; }
    constexpr size_t count() const { return mCount; }

private:
    const Tone* mTones;
    size_t mCount;
};

} // namespace Garbox
