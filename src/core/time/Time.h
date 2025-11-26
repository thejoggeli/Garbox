#pragma once

#include <stdint.h>

namespace Garbox {

class Time {
public:

    static void Start();
    static void Tick();

    // microsecond timer
    // not safe to call in ISR 
    // wraps every ~71.6 minutes (32-bit)
    static uint32_t GetMicros();

    // millisecond timer
    // not safe to call in ISR 
    // wraps every ~49.7 days (32-bit)
    static uint32_t GetMillis();

    // second timer
    // not safe to call in ISR 
    // wraps every ~136 years (32-bit)
    static uint32_t GetSeconds();

    // NOTE: The cpu cycles use a separate hardware timebase (CPU cycle counter)
    // and are not synchronized with the esp_timer-based (non-ISR) variants.
    // wraps every ~17.89 seconds (at 240MHz CPU clock)
    // this function is safe to call in ISR
    static uint32_t GetCpuCycles(); 

    // converts cpu cycles to the duration in microseconds
    // works only up to cycles corresponding to ~17.89 seconds (at 240MHz CPU clock)
    static uint32_t CpuCyclesToMicros(uint32_t cycles);

    // last tick time snapshot accessors
    static uint32_t GetTickMicros();
    static uint32_t GetTickMillis();
    static uint32_t GetTickSeconds();

    // last tick delta snapshot accessors
    static uint32_t GetTickDeltaMicros();
    static float GetTickDeltaSeconds();

    // delays
    static void DelayMillis(uint32_t millis);  // yields to FreeRTOS scheduler
    static void BlockMillis(uint32_t millis);  // busy-wait, no scheduler yield
    static void BlockMicros(uint32_t micros);  // busy-wait, no scheduler yield
};

} // namespace Garbox
