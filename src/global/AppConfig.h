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

    // Display Task
    static constexpr const char* DisplayTaskName = "DisplayTask";
    static constexpr uint32_t DisplayTaskPriority = 8;
    static constexpr uint32_t DisplayTaskStackSize = 1024;
    static constexpr uint32_t DisplayTaskCore = 1;

    // Piezo Task
    static constexpr const char* PiezoTaskName = "PiezoTask";
    static constexpr uint32_t PiezoTaskFrequencyHz = 200;
    static constexpr uint32_t PiezoTaskDurationMicros = 1'000'000 / PiezoTaskFrequencyHz;
    static constexpr uint32_t PiezoTaskDurationMillis = 1'000 / PiezoTaskFrequencyHz;
    static constexpr uint32_t PiezoTaskPriority = 10;
    static constexpr uint32_t PiezoTaskStackSize = 1024;
    static constexpr uint32_t PiezoTaskCore = 1;
    
    // tick rate for specific parts of the app
    static constexpr uint32_t FanTickFrequencyHz = MainTaskFrequencyHz;

    // Display
    static constexpr uint32_t DisplayWidth = 320;
    static constexpr uint32_t DisplayHeight = 240;

    // SPI DMA buffer size for display
    // Due to memory limitations, the buffer can hold a fraction of the display's pixel data 
    // - Formula: bytes = (width * height) * (2 bytes) * (fraction) 
    // At an SPI clock frequency of 
    // - SPI Clock 62.5 MHz bitrate 
    // - Send rate 80 Hz 
    // - Fraction of 50%
    // the result is
    // - SPI bus utilization 80% of bitrate
    // - Results in 40 FPS on screen
    // A fraction slightly above 50% is chosen to account for possible rounding errors and also because there is enough headroom
    static constexpr uint32_t SpiDmaMaxTransferSizeBytes = static_cast<uint32_t>(
        // static_cast<float>(DisplayWidth) * static_cast<float>(DisplayHeight) * 2.0f * 0.51f
        DisplayWidth * DisplayHeight * 2 / 4
    );

};

} // namespace