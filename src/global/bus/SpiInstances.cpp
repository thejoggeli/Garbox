#include "SpiInstances.h"

#include "assert/Assert.h"
#include "global/PinConfig.h"

namespace Garbox {

static bool sInitialized = false;

void SpiInstances::Init(){
    
    AssertExit(!sInitialized, "SpiInstances::Init()", "already initialized");

    GetDisplaySpiDma().setup({
        .hostDevice = SPI2_HOST,
        .pinMosi = PinConfig::DisplaySda,
        .pinMiso = -1,
        .pinClk = PinConfig::DisplayScl,
        .pinCs = PinConfig::DisplayCs,
        .frequencyHz = 40'000'000,
        .maxTransferSize = 240 * 240 * 2,
        .queueSize = 3,
    });

    sInitialized = true;
}

SpiDma& SpiInstances::GetDisplaySpiDma(){
    static SpiDma instance;
    return instance;
}

} // namespace
