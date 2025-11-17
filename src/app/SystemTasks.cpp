#include "SystemTasks.h"

#include "app/config/AppConfig.h"
#include "app/hardware/spi/SpiInstances.h"
#include "app/parts/StatusLeds.h"
#include "app/providers/PartsProvider.h"
#include "parts/display/Display.h"
#include "parts/piezo/PiezoPlayer.h"

namespace Garbox {

void SystemTasks::StartAll(){

    // start status leds task
    StatusLeds& statusLeds = PartsProvider::GetStatusLeds();
    statusLeds.startTask(
        AppConfig::StatusLedsTaskName,
        AppConfig::StatusLedsTaskFrequencyHz,
        AppConfig::StatusLedsTaskStackSize,
        AppConfig::StatusLedsTaskPriority,
        AppConfig::StatusLedsTaskCore
    );

    // start piezo player task
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.startTask(
        AppConfig::PiezoPlayerTaskName,
        AppConfig::PiezoPlayerTaskFrequencyHz,
        AppConfig::PiezoPlayerTaskStackSize,
        AppConfig::PiezoPlayerTaskPriority,
        AppConfig::PiezoPlayerTaskCore
    );

    // start render task (won't do anything until render trigger notify)
    Display& display = PartsProvider::GetDisplay();
    display.startTask(
        AppConfig::DisplayTaskName,
        AppConfig::DisplayTaskStackSize,
        AppConfig::DisplayTaskPriority,
        AppConfig::DisplayTaskCore
    );

    // start spi dma task
    SpiDma& spiDma = SpiInstances::GetSpiDma();
    spiDma.startTask(
        AppConfig::SpiDmaTaskName,
        AppConfig::SpiDmaTaskStackSize,
        AppConfig::SpiDmaTaskPriority,
        AppConfig::SpiDmaTaskCore
    );
}

void SystemTasks::StopAll(){
    
    // stop status leds task
    StatusLeds& statusLeds = PartsProvider::GetStatusLeds();
    statusLeds.stopTask();

    // stop piezo task
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.stopTask();

    // stop display task
    Display& display = PartsProvider::GetDisplay();
    display.stopTask();

    // stop spi dma task
    SpiDma& spiDma = SpiInstances::GetSpiDma();
    spiDma.stopTask();    

}

} // namespace
