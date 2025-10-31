#include "Time.h"

#include <Arduino.h>

#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global/AppConfig.h"

namespace Garbox {

static uint64_t sTickMicros64 = 0;
static uint32_t sTickMicros = 0;

void Time::Tick(){
    sTickMicros64 = esp_timer_get_time();
    sTickMicros = static_cast<uint32_t>(sTickMicros64);
}

uint64_t Time::GetTickMicros64(){
    return sTickMicros64;
}

uint32_t Time::GetTickMicros(){
    return sTickMicros;
}

uint64_t Time::GetMicros64() {
    return esp_timer_get_time();
}

uint32_t Time::GetMicros() {
    return static_cast<uint32_t>(esp_timer_get_time());
}

uint32_t Time::GetMillisSlow() {
    return static_cast<uint32_t>(esp_timer_get_time() / static_cast<uint64_t>(1000));
}

uint32_t Time::GetSecondsSlow() {
    return static_cast<uint32_t>(esp_timer_get_time() / static_cast<uint64_t>(1000000));
}

/// DO NOT call this fucntion from ISR routine !!! It will mess up the ESP-IDF internal FreeRTOS scheduling
void Time::DelayMillis(uint32_t millis){
    delay(millis);
}

/// This is safe to call from ISR.
void Time::DelayMicros(uint32_t micros){
    delayMicroseconds(micros);
}

}