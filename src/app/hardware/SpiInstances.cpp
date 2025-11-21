// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "SpiInstances.h"

#include "core/assert/Assert.h"

// included from config
#include "app/config/AppConfig.h"

namespace Garbox {

static bool sInitialized = false;

void SpiInstances::Init(){
    
    AssertExit(!sInitialized, "SpiInstances", "already initialized");

    GetSpiDma().init({
        .hostDevice = SPI2_HOST,
        .pinMosi = PinMosiSpiDma,
        .pinMiso = PinMisoSpiDma,
        .pinClk = PinClkSpiDma,
        .pinCs = PinCsSpiDma,
        .mode = 3,
        .frequencyHz = AppConfig::SpiDmaFrequencyHz,
        .maxTransferSizeBytes = static_cast<int32_t>(AppConfig::SpiDmaMaxTransferSizeBytes),
        .queueSize = 5,
    });

    sInitialized = true;
}

SpiDma& SpiInstances::GetSpiDma(){
    static SpiDma instance;
    return instance;
}

} // namespace