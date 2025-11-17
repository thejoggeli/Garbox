#include "Time.h"

#include <Arduino.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"

namespace Garbox {

static constexpr uint32_t CpuFreqMhz = static_cast<uint32_t>(F_CPU / 1000000UL);

static volatile uint32_t gTickMicros = 0;
static volatile uint32_t gTickMillis = 0;
static volatile uint32_t gTickSeconds = 0;

// Global spinlock for atomic access to tick snapshot
static portMUX_TYPE gTimeMux = portMUX_INITIALIZER_UNLOCKED;

static inline uint32_t IRAM_ATTR readCycleCount(){
    uint32_t ccount;
    __asm__ __volatile__("rsr %0, ccount" : "=a"(ccount));
    return ccount;
}

void Time::Tick(){
    taskENTER_CRITICAL(&gTimeMux);
    gTickMicros = GetMicros();
    gTickMillis = GetMillis();
    gTickSeconds = GetSeconds();
    taskEXIT_CRITICAL(&gTimeMux);
}

uint32_t Time::GetMicros(){
    return static_cast<uint32_t>(esp_timer_get_time());
}

uint32_t Time::GetMillis(){
    return static_cast<uint32_t>(esp_timer_get_time() / 1000ULL);
}

uint32_t Time::GetSeconds(){
    return static_cast<uint32_t>(esp_timer_get_time() / 1000000ULL);
}

uint32_t Time::GetCpuCycles(){
    return static_cast<uint32_t>(readCycleCount());
}

uint32_t Time::CpuCyclesToMicros(uint32_t cycles){
    return static_cast<uint32_t>(cycles / CpuFreqMhz);
}

uint32_t Time::GetTickMicros()  { 
    return gTickMicros; 
}

uint32_t Time::GetTickMillis(){ 
    return gTickMillis; 
}

uint32_t Time::GetTickSeconds(){ 
    return gTickSeconds; 
}

void Time::DelayMillis(uint32_t millis){ 
    delay(millis); 
}

void Time::BlockMillis(uint32_t millis){
    const uint32_t startMicros = GetMicros();
    const uint32_t waitMicros  = millis * 1000U;
    while ((uint32_t)(GetMicros() - startMicros) < waitMicros){
        // spin
    }
}

void Time::BlockMicros(uint32_t micros){
    const uint32_t startMicros = GetMicros();
    while ((uint32_t)(GetMicros() - startMicros) < micros){
        // spin
    }
}

} // namespace Garbox
