#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // Global
    static constexpr uint32_t NumCpuCores = 2;

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
    static constexpr uint32_t PiezoPlayerTaskDurationMicros = 1'000'000 / PiezoPlayerTaskFrequencyHz;
    static constexpr uint32_t PiezoPlayerTaskDurationMillis = 1'000 / PiezoPlayerTaskFrequencyHz;
    static constexpr uint32_t PiezoPlayerTaskPriority = 15;
    static constexpr uint32_t PiezoPlayerTaskStackSize = 1024*2;
    static constexpr uint32_t PiezoPlayerTaskCore = 1;

    // Led Task
    static constexpr const char* LedAnimationTaskName = "LedAnimationTask";
    static constexpr uint32_t LedAnimationTaskFrequencyHz = 60;
    static constexpr uint32_t LedAnimationTaskDurationMicros = 1'000'000 / LedAnimationTaskFrequencyHz;
    static constexpr uint32_t LedAnimationTaskDurationMillis = 1'000 / LedAnimationTaskFrequencyHz;
    static constexpr uint32_t LedAnimationTaskPriority = 14;
    static constexpr uint32_t LedAnimationTaskStackSize = 1024*2;
    static constexpr uint32_t LedAnimationTaskCore = 1;

    // Display
    static constexpr const char* DisplayTaskName = "DisplayTask";
    static constexpr uint32_t DisplayTaskPriority = 10;
    static constexpr uint32_t DisplayTaskStackSize = 1024*2;
    static constexpr uint32_t DisplayTaskCore = 1;
    static constexpr uint32_t DisplayWidth = 320;
    static constexpr uint32_t DisplayHeight = 240;

    // SPI DMA 
    static constexpr const char* SpiDmaTaskName = "SpiDmaTask";
    static constexpr uint32_t SpiDmaTaskPriority = 10;
    static constexpr uint32_t SpiDmaTaskStackSize = 2048;
    static constexpr uint32_t SpiDmaFrequencyHz = 62'500'000;
    // Max SPI DMA size is limited to 32KB on ESP32-S3
    // Dividing by 8 allows sets to buffer to ~19KB
    static constexpr uint32_t SpiDmaMaxTransferSizeBytes = DisplayWidth * DisplayHeight * 2 / 8; 

};

} // namespace
