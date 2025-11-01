#pragma once

#include "parts/piezo/ToneSequence.h"

namespace Garbox {

class PiezoConfig {
public:

    static const ToneSequence StartupSequence;
    static const ToneSequence ButtonSequence;
    static const ToneSequence InterpolatedSequence;
    static const ToneSequence InterpolatedSequence2;
    
};

} // namespace Garbox
