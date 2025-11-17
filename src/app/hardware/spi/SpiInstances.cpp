#include "SpiInstances.h"

#include "app/config/AppConfig.h"
#include "app/config/PinConfig.h"
#include "assert/Assert.h"

namespace Garbox {

static bool gInitialized = false;

void SpiInstances::Init(){
    
    AssertExit(!gInitialized, "SpiInstances", "already initialized");

    GetSpiDma().init({
        .hostDevice = SPI2_HOST,
        .pinMosi = PinConfig::DisplaySda,
        .pinMiso = -1,
        .pinClk = PinConfig::DisplayScl,
        .pinCs = -1,
        .mode = 3,
        .frequencyHz = AppConfig::SpiDmaFrequencyHz,
        .maxTransferSizeBytes = static_cast<int32_t>(AppConfig::SpiDmaMaxTransferSizeBytes),
        .queueSize = 5,
    });

    gInitialized = true;
}

SpiDma& SpiInstances::GetSpiDma(){
    static SpiDma instance;
    return instance;
}

} // namespace
