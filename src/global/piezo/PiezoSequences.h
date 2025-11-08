#pragma once

#include "parts/piezo/ToneSequence.h"

namespace Garbox {

class PiezoSequences {
public:
    static const ToneSequence& GetStartup();
    static const ToneSequence& GetButton();
    static const ToneSequence& GetHelixUp();
    static const ToneSequence& GetHelixDown();
    static const ToneSequence& GetInterpolated1();
    static const ToneSequence& GetInterpolated2();
    static const ToneSequence& GetFanStalled();
    static const ToneSequence& GetFanEnabled();
    static const ToneSequence& GetFanDisabled();
};

} // namespace Garbox
