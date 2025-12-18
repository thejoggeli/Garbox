#include "TaskManager.h"

#include "app/config/AppConfig.h"
#include "app/generated/hardware/SpiInstances.h"
#include "app/providers/PartsProvider.h"
#include "core/assert/Assert.h"
#include "modules/parts/display/Display.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

static constexpr size_t MaxStopHandlersCount = 16; 
static std::array<TaskManager::StopHandler, MaxStopHandlersCount> sStopHandlers;
static uint32_t sStopHandlersCount = 0;

void TaskManager::RegisterStopHandler(StopHandler handler){
    AssertExit(sStopHandlersCount < MaxStopHandlersCount, "TaskManager", "max tasks count exceeded");
    sStopHandlers[sStopHandlersCount++] = handler;
}

void TaskManager::StartAll(){

    // start status leds task
    AnimatedLedGroup& statusLeds = PartsProvider::GetStatusLeds();
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

    // start display render task (won't do anything until render trigger notify)
    Display& display = PartsProvider::GetDisplay();
    display.startRenderTask(
        AppConfig::DisplayRenderTaskName,
        AppConfig::DisplayRenderTaskStackSize,
        AppConfig::DisplayRenderTaskPriority,
        AppConfig::DisplayRenderTaskCore
    );

    // start display sender task
    display.startSenderTask(
        AppConfig::DisplaySenderTaskName,
        AppConfig::DisplaySenderTaskStackSize,
        AppConfig::DisplaySenderTaskPriority,
        AppConfig::DisplaySenderTaskCore
    );

    // start spi dma task
    SpiDma& spiDma = SpiInstances::GetSpiDma();
    spiDma.startTask(
        AppConfig::SpiDmaTaskName,
        AppConfig::SpiDmaTaskStackSize,
        AppConfig::SpiDmaTaskPriority,
        AppConfig::SpiDmaTaskCore
    );

    // register status leds stop handler
    RegisterStopHandler([](){
        PartsProvider::GetStatusLeds().stopTask();
    });

    // register piezo player stop handler
    RegisterStopHandler([](){
        PartsProvider::GetPiezoPlayer().stopTask();
    });

    // register display stop handler
    RegisterStopHandler([](){
        PartsProvider::GetDisplay().stopTasks();
    });

    // register spi dma stop handler
    RegisterStopHandler([](){
        SpiInstances().GetSpiDma().stopTask();
    });
}

void TaskManager::StopAll(){
    for(size_t i = 0; i < sStopHandlersCount; i++){
        StopHandler& handler = sStopHandlers[i];
        handler();
    }
}

} // namespace
