#include "SpiInstances.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"
#include "global/PinConfig.h"

namespace Garbox {

static bool gInitialized = false;

void SpiInstances::Init(){
    
    AssertExit(!gInitialized, "SpiInstances", "already initialized");

    GetSpiDma().setup({
        .hostDevice = SPI2_HOST,
        .pinMosi = PinConfig::DisplaySda,
        .pinMiso = -1,
        .pinClk = PinConfig::DisplayScl,
        .pinCs = -1,
        .mode = 3,
        .frequencyHz = AppConfig::SpiDmaFrequencyHz,
        .maxTransferSizeBytes = static_cast<int32_t>(AppConfig::SpiDmaMaxTransferSizeBytes),
        .queueSize = 5,
        .txCompleteTaskName = AppConfig::SpiDmaTaskName,
        .txCompleteTaskPriority = AppConfig::SpiDmaTaskPriority,
        .txCompleteTaskStackSize = AppConfig::SpiDmaTaskStackSize,
    });

    gInitialized = true;
}

SpiDma& SpiInstances::GetSpiDma(){
    static SpiDma instance;
    return instance;
}

} // namespace
