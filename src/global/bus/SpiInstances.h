#pragma once

#include "core/hardware/bus/SpiDma.h"

namespace Garbox {

struct SpiInstances {
public:

    static void Init();

    static SpiDma& GetSpiDma();

};

} // namespace
