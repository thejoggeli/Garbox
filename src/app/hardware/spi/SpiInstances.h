#pragma once

#include "core/hardware/spi/SpiDma.h"

namespace Garbox {

struct SpiInstances {
public:

    static void Init();

    static SpiDma& GetSpiDma();

};

} // namespace
