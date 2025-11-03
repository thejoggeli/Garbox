#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // target durations for full cycle + all individual tasks
    struct TaskDurationMicros {
        static constexpr uint32_t Main = 10'000; // 10 ms 
        static constexpr uint32_t DisplayTx = 2'500; // 2.5 ms 
        static constexpr uint32_t FullCycle = TaskDurationMicros::Main + TaskDurationMicros::DisplayTx; // 12.5 ms
    };

    // target frequencies for full cycle + all individual tasks
    struct TaskFrequencyHz {
        static constexpr uint32_t FullCycle = 1'000'000 / TaskDurationMicros::FullCycle; // 80 Hz
        static constexpr uint32_t Main = FullCycle; // 80 Hz
        static constexpr uint32_t DisplayTx = FullCycle; // 80 Hz
    };

    // tick rate for specific parts of the app
    struct TickFrequencyHz {
        static constexpr uint32_t Fan = TaskFrequencyHz::Main;
    };

    // FreeRTOS task priorities
    static constexpr uint32_t SpiDmaTaskPriority = 5; 

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
        DisplayWidth * DisplayHeight * 2
    );

};

} // namespace