#pragma once

#include "parts/piezo/ToneSequence.h"

namespace Garbox {

class PiezoConfig {
public:

    static const ToneSequence startupSequence;
    static const ToneSequence buttonSequence;
    
};

} // namespace Garbox
