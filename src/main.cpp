#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/bus/SpiInstances.h"
#include "global/function/FunctionInstances.h"
#include "global/ledc/LedcInstances.h"
#include "global/gpio/GpioInstances.h"
#include "global/timer/TimerInstances.h"
#include "parts/led/DebugLeds.h"
#include "util/StringUtils.h"

using namespace Garbox;

MainControl gMainControl;

void mainTask(void* parameter);
void logProfiler();

void setup() {

    Log::Init();
    Log::SetLevel(Log::Level::Verbose);
    
    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* context, const char* message){
        DebugLeds::SetLed(DebugLeds::Id::Assert, true);
        LogError("AssertHandler", "AssertDebug! %s %s", context, message);
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

    // init statics
    GpioInstances::Init();
    DebugLeds::Init();
    LedcInstances::Init();
    TimerInstances::Init();
    SpiInstances::Init();

    // init functions
    FunctionInstances::GetGamma22Sampled();
    FunctionInstances::GetEaseInOutSineSampled();

    // init profiler
    Profiler::Setup(ProfilerConfig::Count);
    Profiler::SetEnabled(ProfilerConfig::EnableProfiler);

    // fade debug leds in
    for(AnimatedLed& led : DebugLeds::GetAllLeds()){
        led.setBrightness(0);
        led.setAnimation(FunctionInstances::GetEaseInOutSineSampled(), 1, 250_ms, 0.0f, 1.0f);
    }
    Time::DelayMillis(250);

    // init main app
    // leds must not be used in main app init() functions 
    gMainControl.init();

    // fade debug leds out
    for(AnimatedLed& led : DebugLeds::GetAllLeds()){
        led.setBrightness(0);
        led.setAnimation(FunctionInstances::GetEaseInOutSineSampled(), 1, 250_ms, 1.0f, 0.0f);
    }
    Time::DelayMillis(250);

    // start main app
    gMainControl.start();

    // start main task
    xTaskCreatePinnedToCore(
        mainTask,
        AppConfig::MainTaskName,
        AppConfig::MainTaskStackSize,
        NULL, // parameter
        AppConfig::MainTaskPriority,
        NULL, // handle
        AppConfig::MainTaskCore
    );
}

void loop(){
    Time::DelayMillis(1000);
}

void mainTask(void* parameter){
    const TickType_t cycleDuration = pdMS_TO_TICKS(AppConfig::MainTaskDurationMillis);
    const TickType_t updateDisplayDuration = pdMS_TO_TICKS(AppConfig::MainTaskUpdateDisplayDurationMillis);
    TickType_t lastWakeTime = xTaskGetTickCount();
    while(true){
        // begin main task
        Profiler::Begin(ProfilerConfig::MainTask);

        // main tick
        Profiler::Begin(ProfilerConfig::MainTick);
        Time::Tick();
        gMainControl.tick();
        Profiler::End(ProfilerConfig::MainTick);

        // logging
        Profiler::Begin(ProfilerConfig::LogTick);
        logProfiler();
        Profiler::End(ProfilerConfig::LogTick);

        // wait until updateDisplayDuration millis are remaining before current cycle is finished
        // this is done to ensure that the display is always updated at a fixed interval and to 
        // give it enough time to transfer the previous ui state via SPI
        vTaskDelayUntil(&lastWakeTime, cycleDuration - updateDisplayDuration);

        Profiler::Begin(ProfilerConfig::UiTick);
        // if display is not busy
        // TODO update ui state
        // TODO notify display to render new ui state
        Time::DelayMicros(500); // placeholder delay
        Profiler::End(ProfilerConfig::UiTick);
        
        // end main task
        // sleep until next tick
        vTaskDelayUntil(&lastWakeTime, updateDisplayDuration);
        Profiler::End(ProfilerConfig::MainTask);
    }
}

void logProfiler(){
    static uint32_t lastPrint = 0;
    uint32_t now = Time::GetMicros();
    if (now - lastPrint > 30'000'000) {
        Profiler::UpdateAll();
        lastPrint = now;
        
        uint32_t seconds = Time::GetSecondsSlow();
        static char timeStringBuffer[20];
        StringUtils::FormatDurationDHMS(seconds, timeStringBuffer, sizeof(timeStringBuffer));
        LogInfo("Main", "======================== Diagnostics %s =======================", timeStringBuffer);
        LogInfo("Main", " | ProfilerId         | Count | freq(Hz) | min(us) | avg(us) | max(us) |");
        for (uint8_t i = 0; i < ProfilerConfig::Count; ++i) {
            const Profiler::Record& r = Profiler::GetRecord(i);
            const char* idStr = ProfilerConfig::IdToString(i);
            LogInfo("Main", " | %-18s | %5" PRIu32 " | %8.3f | %7" PRIu32 " | %7.0f | %7" PRIu32 " |", idStr, r.countLast, r.frequency, r.minDurationLast, r.avgDuration, r.maxDurationLast);
        }
        LogInfo("Main", "=========================================================================");
    }
}

