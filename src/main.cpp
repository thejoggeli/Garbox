#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/ledc/LedcInstances.h"
#include "global/timer/TimerInstances.h"
#include "parts/debugLeds/DebugLeds.h"

using namespace Garbox;

MainControl gMainControl;

void setup() {

    Log::Init();
    Log::SetLevel(Log::Level::Verbose);
    
    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* context, const char* message){
        DebugLeds::SetLed(DebugLeds::Id::Assert, true);
        LogWarning("AssertHandler", "AssertDebug! %s %s", context, message);
        gMainControl.onAssertDebug(context, message);
    });

    // assert exit handler
    AssertHandler::SetExitHandler([](const char* context, const char* message){
        DebugLeds::SetAllLeds(true);
        gMainControl.onAssertExit(context, message);
        while(true){
            LogError("AssertHandler", "AssertExit! %s %s", context, message);
            Time::DelayMillis(1000);
            DebugLeds::ToggleAllLeds();
        }
    });

    // init debug leds
    DebugLeds::Init();

    // init ledc
    LedcInstances::Init();

    // init timers
    TimerInstances::Init();

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

void loop() {
    static uint32_t lastWake = Time::GetMicros();
    uint32_t now = Time::GetMicros64();

    if (now - lastWake >= AppConfig::TargetTickIntervalMicros) {
        lastWake += AppConfig::TargetTickIntervalMicros;
        gMainControl.tick();
    }
    
    Time::DelayMillis(1);
}