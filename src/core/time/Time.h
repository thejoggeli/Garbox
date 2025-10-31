#pragma once

#include <cstdint>
#include "TimeLiterals.h"

namespace Garbox {

class Time {

public:

    Time() = delete;
    ~Time() = delete;

    static void Tick();
    static uint64_t GetTickMicros64();
    static uint32_t GetTickMicros();

    /// get native time in microseconds (64-bit)
    /// wraps after ~584942 years
    static uint64_t GetMicros64();
    
    /// get native time in microseconds
    /// wraps after ~71 minutes
    static uint32_t GetMicros();

    /// get native time in milliseconds
    /// slow because of 64-bit division
    /// wraps after ~49 days
    static uint32_t GetMillisSlow();

    /// get native time in seconds
    /// slow because of 64-bit division
    /// wraps after ~136 years
    static uint32_t GetSecondsSlow();

    /// wait for n milliseconds and block FreeRTOS scheduler
    static void DelayMillis(uint32_t millis);

    /// wait for n microseconds and block FreeRTOS scheduler
    static void DelayMicros(uint32_t micros);

};

}