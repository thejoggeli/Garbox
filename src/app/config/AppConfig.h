#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // Global
    static constexpr uint32_t NumCpuCores = 2;
    static constexpr uint32_t HeartbeatIntervalMicros = 2'000'000;
    static constexpr uint32_t MainTickDurationMillis = 20;
    static constexpr uint32_t DisplayTickDurationMillis = 13;
    static constexpr uint32_t TickFrequencyHz = 1000.0f / (MainTickDurationMillis + DisplayTickDurationMillis);

    // Profiler
    static constexpr bool EnableProfiler = false;

    // Main Task
    static constexpr const char* MainTaskName = "MainTask";
    static constexpr uint32_t MainTaskPriority = 3;
    static constexpr uint32_t MainTaskStackSize = 1024*8;
    static constexpr uint32_t MainTaskCore = 1;

    // Piezo Task
    static constexpr const char* PiezoPlayerTaskName = "PiezoPlayerTask";
    static constexpr uint32_t PiezoPlayerTaskFrequencyHz = 120;
    static constexpr uint32_t PiezoPlayerTaskPriority = 15;
    static constexpr uint32_t PiezoPlayerTaskStackSize = 1024*4;
    static constexpr uint32_t PiezoPlayerTaskCore = 1;

    // Led Task
    static constexpr const char* StatusLedsTaskName = "LedAnimationTask";
    static constexpr uint32_t StatusLedsTaskFrequencyHz = 60;
    static constexpr uint32_t StatusLedsTaskPriority = 14;
    static constexpr uint32_t StatusLedsTaskStackSize = 1024*4;
    static constexpr uint32_t StatusLedsTaskCore = 1;

    // Display Render Task
    static constexpr const char* DisplayRenderTaskName = "DisplayRenderTask";
    static constexpr uint32_t DisplayRenderTaskPriority = 10;
    static constexpr uint32_t DisplayRenderTaskStackSize = 1024*4;
    static constexpr uint32_t DisplayRenderTaskCore = 1;

    // Display Sender Task
    static constexpr const char* DisplaySenderTaskName = "DisplaySenderTask";
    static constexpr uint32_t DisplaySenderTaskPriority = 12;
    static constexpr uint32_t DisplaySenderTaskStackSize = 1024*4;
    static constexpr uint32_t DisplaySenderTaskCore = 1;

    // SPI DMA Task
    static constexpr const char* SpiDmaTaskName = "SpiDmaTask";
    static constexpr uint32_t SpiDmaTaskPriority = 11;
    static constexpr uint32_t SpiDmaTaskStackSize = 1024*4;
    static constexpr uint32_t SpiDmaTaskCore = 1;

    // Display
    static constexpr uint32_t DisplayWidth = 320;
    static constexpr uint32_t DisplayHeight = 240;
    static constexpr uint32_t DisplayPartialFactor = 4;
    static constexpr uint32_t DisplayBytesPerPixel = 2;
    static constexpr uint32_t DisplayBytesPerFrame = DisplayWidth * DisplayHeight * DisplayBytesPerPixel;
    static constexpr uint32_t DisplayBytesPerFlush = DisplayBytesPerFrame / DisplayPartialFactor;
    static constexpr uint32_t DisplayChunkSizeBytes = 1024*2;

    // SPI DMA
    // Max SPI DMA size is limited to 32KB on ESP32-S3
    // Dividing by 8 allows sets to buffer to ~19KB
    static constexpr uint32_t SpiDmaMaxTransferSizeBytes = DisplayBytesPerFlush;
    static constexpr uint32_t SpiDmaFrequencyHz = 62'500'000;

    static_assert(SpiDmaMaxTransferSizeBytes >= DisplayChunkSizeBytes, "spi dma buffer size must be >= display chunk size");

};

} // namespace
