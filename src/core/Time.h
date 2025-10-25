#pragma once

#include <cstdint>

namespace Garbox {

class Time {

public:

    Time() = delete;
    ~Time() = delete;

    static uint64_t GetMicros64();
    static uint32_t GetMicros();
    static uint32_t GetMillis();
    static void BeginTick();
    static void EndTick();

    static constexpr float MicrosToSeconds = 1.0e-6f;
    static constexpr float TICK_INTERVAL_MICROS = 10 * 1000; // 10ms

    static uint32_t sTickBeginTime;

private:
    
    static uint32_t sLastMicros64;
    static uint64_t sOffsetMicros64;

};

}