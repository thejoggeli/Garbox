#pragma once

#include "core/hardware/spi/SpiDma.h"


namespace Garbox {

struct SpiInstances {
public:

    static constexpr int32_t PinMosiSpiDma = 11; 
    static constexpr int32_t PinMisoSpiDma = -1; 
    static constexpr int32_t PinClkSpiDma = 12; 
    static constexpr int32_t PinCsSpiDma = -1; 

    static void Init();

    static SpiDma& GetSpiDma();

};

} // namespace