#pragma once

#include "core/hardware/bus/SpiDma.h"

namespace Garbox {

struct SpiInstances {

    static void Init();
    
    static SpiDma& GetDisplaySpiDma();

};

} // namespace
