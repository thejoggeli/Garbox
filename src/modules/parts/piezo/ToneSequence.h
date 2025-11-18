#pragma once
#include <cstddef>
#include "Tone.h"

namespace Garbox {

class ToneSequence {
public:

    // construct sequence from tone array
    constexpr ToneSequence(const Tone* tones, size_t count): 
        mTones(tones), 
        mCount(count){
        // nothing to do
    }

    // construct from fixed-size array (factory-style)
    template <size_t N>
    static constexpr ToneSequence from(const Tone (&tones)[N]){
        return ToneSequence(tones, N);
    }

    // accessors
    constexpr const Tone& getTone(size_t index) const { 
        return mTones[index]; 
    }

    constexpr const Tone* getTones() const { 
        return mTones; 
    }

    constexpr size_t getCount() const { 
        return mCount; 
    }

private:

    const Tone* mTones;
    size_t mCount;

};

} // namespace Garbox
