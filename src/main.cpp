#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global/ledc/LedcInstances.h"
#include "parts/debugLeds/DebugLeds.h"

using namespace Garbox;

MainControl gMainControl;

void setup() {
    
    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* context, const char* message){
        LogWarning("AssertHandler", "AssertDebug! %s %s", context, message);
        gMainControl.onAssertDebug(context, message);
    });

    // assert exit handler
    AssertHandler::SetExitHandler([](const char* context, const char* message){
        gMainControl.onAssertExit(context, message);
        while(true){
            LogError("AssertHandler", "AssertExit! %s %s", context, message);
            Time::DelayMillis(1000);
        }
    });

    // init ledc
    LedcInstances::Init();

    // init debug leds
    DebugLeds::Init();

    // fade debug leds in
    for(int32_t i = 0; i <= 25; i++){
        float brightness = static_cast<float>(i) * (1.0f/25.0f);
        DebugLeds::SetAllLeds(true, brightness);
        Time::DelayMillis(10);
    }
    Time::DelayMillis(250);

    // fade debug leds out
    for(int32_t i = 25; i >= 0; i--){
        float brightness = static_cast<float>(i) * (1.0f/25.0f);
        DebugLeds::SetAllLeds(true, brightness);
        Time::DelayMillis(10);
    }
    Time::DelayMillis(250);

    // init everything
    gMainControl.init();

    // start everything
    gMainControl.start();
}

extern "C" void app_main(void){

    ESP_LOGI("Main", "System start");
    
    setup();

    ESP_LOGI("Main", "Setup complete");

    const TickType_t period = pdMS_TO_TICKS(10);  // 100 Hz
    TickType_t last_wake = xTaskGetTickCount();

    while (true) {
        // --- Main control cycle ---
        // read_sensors();
        // control_logic();
        // update_outputs();

        vTaskDelayUntil(&last_wake, period);
    }

}
