#pragma once

#include "parts/piezo/ToneSequence.h"

namespace Garbox {

class PiezoSequences {
public:

    static const ToneSequence Startup;
    static const ToneSequence Button;
    static const ToneSequence Helix;
    static const ToneSequence Interpolated1;
    static const ToneSequence Interpolated2;
    static const ToneSequence FanStalled;

};

} // namespace Garbox
