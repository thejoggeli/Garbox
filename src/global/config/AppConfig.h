#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // Global
    static constexpr uint32_t NumCpuCores = 2;
    static constexpr uint32_t HeartbeatIntervalMicros = 2'000'000;

    // Main Task
    static constexpr const char* MainTaskName = "MainTask";
    static constexpr uint32_t MainTaskFrequencyHz = 30;
    static constexpr uint32_t MainTaskDurationMicros = 1'000'000 / MainTaskFrequencyHz; // 33'333 us
    static constexpr uint32_t MainTaskDurationMillis = 1'000 / MainTaskFrequencyHz; // 33 ms
    // ui and display update is triggered this many seconds before each main cycle tick finishes 
    static constexpr uint32_t MainTaskUpdateDisplayDurationMillis = 13;
    static constexpr uint32_t MainTaskPriority = 5;
    static constexpr uint32_t MainTaskStackSize = 1024*8;
    static constexpr uint32_t MainTaskCore = 1;

    // Piezo Task
    static constexpr const char* PiezoPlayerTaskName = "PiezoPlayerTask";
    static constexpr uint32_t PiezoPlayerTaskFrequencyHz = 120;
    static constexpr uint32_t PiezoPlayerTaskPriority = 15;
    static constexpr uint32_t PiezoPlayerTaskStackSize = 1024*2;
    static constexpr uint32_t PiezoPlayerTaskCore = 1;

    // Led Task
    static constexpr const char* StatusLedsTaskName = "LedAnimationTask";
    static constexpr uint32_t StatusLedsTaskFrequencyHz = 60;
    static constexpr uint32_t StatusLedsTaskPriority = 14;
    static constexpr uint32_t StatusLedsTaskStackSize = 1024*2;
    static constexpr uint32_t StatusLedsTaskCore = 1;

    // Display Task
    static constexpr const char* DisplayTaskName = "DisplayTask";
    static constexpr uint32_t DisplayTaskPriority = 10;
    static constexpr uint32_t DisplayTaskStackSize = 1024*2;
    static constexpr uint32_t DisplayTaskCore = 1;

    // SPI DMA Task
    static constexpr const char* SpiDmaTaskName = "SpiDmaTask";
    static constexpr uint32_t SpiDmaTaskPriority = 10;
    static constexpr uint32_t SpiDmaTaskStackSize = 2048;
    static constexpr uint32_t SpiDmaTaskCore = 1;

    // Display
    static constexpr uint32_t DisplayWidth = 320;
    static constexpr uint32_t DisplayHeight = 240;
    static constexpr uint32_t DisplayPartialFactor = 8;
    static constexpr uint32_t DisplayBytesPerPixel = 2;
    static constexpr uint32_t DisplayBytesPerFrame = DisplayWidth * DisplayHeight * DisplayBytesPerPixel;
    static constexpr uint32_t DisplayBytesPerFlush = DisplayBytesPerFrame / DisplayPartialFactor;

    // SPI DMA
    // Max SPI DMA size is limited to 32KB on ESP32-S3
    // Dividing by 8 allows sets to buffer to ~19KB
    static constexpr uint32_t SpiDmaMaxTransferSizeBytes = DisplayBytesPerFlush;
    static constexpr uint32_t SpiDmaFrequencyHz = 62'500'000;

};

} // namespace
