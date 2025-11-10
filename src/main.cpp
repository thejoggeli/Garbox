#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/hardware/adc/AdcInstances.h"
#include "global/hardware/ledc/LedcInstances.h"
#include "global/hardware/gpio/GpioInstances.h"
#include "global/hardware/spi/SpiInstances.h"
#include "global/hardware/timer/TimerInstances.h"
#include "global/util/FunctionInstances.h"
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
        LogError("AssertHandler", "AssertDebug! %s %s", context, message);
        if(DebugLeds::IsInitialized()){
            DebugLeds::SetLed(DebugLeds::Id::Assert, true);
        }
        gMainControl.onAssertDebug(context, message);
    });

    // assert exit handler
    AssertHandler::SetExitHandler([](const char* context, const char* message){
        LogError("AssertHandler", "AssertExit! %s %s", context, message);
        if(DebugLeds::IsInitialized()){
            DebugLeds::SetLed(DebugLeds::Id::Assert, true);
        }
        gMainControl.onAssertExit(context, message);
        while(true){
            LogError("AssertHandler", "AssertExit! %s %s", context, message);
            if(DebugLeds::IsInitialized()){
                DebugLeds::ToggleAllLeds();
            }
            Time::DelayMillis(1000);
        }
    });

    // install ISR service once globally (safe to call multiple times)
    if (gpio_install_isr_service(0) != ESP_OK){
        TriggerExit("Main", "gpio_install_isr_service failed");
    }

    // init hardware instances
    GpioInstances::Init();
    LedcInstances::Init();
    TimerInstances::Init();
    SpiInstances::Init();
    AdcInstances::Init();

    // init function instances
    FunctionInstances::GetGamma22Sampled();
    FunctionInstances::GetEaseInOutSineSampled();

    // init profiler
    Profiler::Setup(ProfilerConfig::Count);
    Profiler::SetEnabled(ProfilerConfig::EnableProfiler);

    // init debug leds
    DebugLeds::Init();

    // init main app
    gMainControl.init();

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
        Time::BlockMicros(1000); // placeholder delay
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
        
        uint32_t seconds = Time::GetSeconds();
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

