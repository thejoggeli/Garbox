#include "Time.h"

#include <Arduino.h>

#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global/AppConfig.h"

namespace Garbox {

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

void Time::DelayMillis(uint32_t millis){
    delay(millis);
}

void Time::DelayMicros(uint32_t micros){
    delayMicroseconds(micros);
}

}