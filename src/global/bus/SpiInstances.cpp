#include "SpiInstances.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"
#include "global/PinConfig.h"

namespace Garbox {

static bool sInitialized = false;

void SpiInstances::Init(){
    
    AssertExit(!sInitialized, "SpiInstances::Init()", "already initialized");

    GetSpiDma().setup({
        .hostDevice = SPI2_HOST,
        .pinMosi = PinConfig::DisplaySda,
        .pinMiso = -1,
        .pinClk = PinConfig::DisplayScl,
        .pinCs = PinConfig::DisplayCs,
        .frequencyHz = 40'000'000,
        .maxTransferSize = static_cast<int32_t>(AppConfig::SpiDmaMaxTransferSize),
        .queueSize = 5,
        .txCompleteTaskPriority = 4,
        .txCompleteTaskStackSize = 4096,
    });

    sInitialized = true;
}

SpiDma& SpiInstances::GetSpiDma(){
    static SpiDma instance;
    return instance;
}

} // namespace
