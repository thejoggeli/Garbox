#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // Main Task
    static constexpr const char* MainTaskName = "MainTask";
    static constexpr uint32_t MainTaskFrequencyHz = 40;
    static constexpr uint32_t MainTaskDurationMicros = 1'000'000 / MainTaskFrequencyHz;
    static constexpr uint32_t MainTaskDurationMillis = 1'000 / MainTaskFrequencyHz;
    static constexpr uint32_t MainTaskPriority = 5;
    static constexpr uint32_t MainTaskStackSize = 1024*4;
    static constexpr uint32_t MainTaskCore = 1;
    static constexpr uint32_t MainTaskUpdateDisplayDurationMillis = 5; // ui and display update is triggered this many seconds before each main cycle tick finishes 

    // Piezo Task
    static constexpr const char* PiezoTaskName = "PiezoTask";
    static constexpr uint32_t PiezoTaskFrequencyHz = 200;
    static constexpr uint32_t PiezoTaskDurationMicros = 1'000'000 / PiezoTaskFrequencyHz;
    static constexpr uint32_t PiezoTaskDurationMillis = 1'000 / PiezoTaskFrequencyHz;
    static constexpr uint32_t PiezoTaskPriority = 15;
    static constexpr uint32_t PiezoTaskStackSize = 1024;
    static constexpr uint32_t PiezoTaskCore = 1;

    // Display
    static constexpr const char* DisplayTaskName = "DisplayTask";
    static constexpr uint32_t DisplayTaskPriority = 10;
    static constexpr uint32_t DisplayTaskStackSize = 1024;
    static constexpr uint32_t DisplayTaskCore = 1;
    static constexpr uint32_t DisplayWidth = 320;
    static constexpr uint32_t DisplayHeight = 240;

    // SPI DMA 
    static constexpr const char* SpiDmaTaskName = "SpiDmaTask";
    static constexpr uint32_t SpiDmaTaskPriority = 10;
    static constexpr uint32_t SpiDmaTaskStackSize = 2048;
    // Due to memory limitations, the buffer can hold a fraction of the display's pixel data 
    static constexpr uint32_t SpiDmaMaxTransferSizeBytes = DisplayWidth * DisplayHeight * 2 / 4;
    static constexpr uint32_t SpiDmaFrequencyHz = 62'500'000;

};

} // namespace
