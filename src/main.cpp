#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/log/Log.h"
#include "core/scheduling/TimeSlotScheduler.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/bus/SpiInstances.h"
#include "global/ledc/LedcInstances.h"
#include "global/gpio/GpioInstances.h"
#include "global/timer/TimerInstances.h"
#include "parts/debugLeds/DebugLeds.h"
#include "util/StringUtils.h"

using namespace Garbox;

MainControl gMainControl;

void mainTask(void* parameter);

void setup() {

    Log::Init();
    Log::SetLevel(Log::Level::Verbose);
    
    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* context, const char* message){
        DebugLeds::SetLed(DebugLeds::Id::Assert, true);
        while(true){
        LogError("AssertHandler", "AssertDebug! %s %s", context, message);

        }
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
    const TickType_t cycleTime = pdMS_TO_TICKS(AppConfig::MainTaskDurationMillis);
    TickType_t lastWakeTime = xTaskGetTickCount();
    while(true){

        // FIXED DURATION SECTION
        Time::Tick();
        gMainControl.tick();

        // VARIABLE DURATION SECTION

        // update display first to minimize frame jitter
        // TODO update display 

        // logging
        // TODO log some things         
        
        // sleep until next tick
        vTaskDelayUntil(&lastWakeTime, cycleTime);
    }
}

/*
void loggingTask(){
    // Print diagnostics once per second
    static uint32_t lastPrintMicros = 0;
    if (Time::GetTickMicros() - lastPrintMicros >= 10'000'000) {
        lastPrintMicros = Time::GetTickMicros();

        const TimeSlotScheduler::Diagnostics& dMain = gScheduler.getDiagnostics(SlotMain);
        const TimeSlotScheduler::Diagnostics& dDisp = gScheduler.getDiagnostics(SlotDisplay);
        const TimeSlotScheduler::Diagnostics& dLogg = gScheduler.getDiagnostics(SlotLogging);

        uint32_t const maxMain = dMain.maxTimeMicros;
        uint32_t const maxDisp = dDisp.maxTimeMicros;
        uint32_t const maxLogg = dLogg.maxTimeMicros;

        uint32_t const maxAllMain = dMain.maxTimeAllMicros;
        uint32_t const maxAllDisp = dDisp.maxTimeAllMicros;
        uint32_t const maxAllLogg = dLogg.maxTimeAllMicros;

        float const usageMain = static_cast<float>(dMain.maxTimeAllMicros) / static_cast<float>(dMain.minDurationMicros) * 100.0f;
        float const usageDisp = static_cast<float>(dDisp.maxTimeAllMicros) / static_cast<float>(dDisp.minDurationMicros) * 100.0f;
        float const usageLogg = static_cast<float>(dLogg.maxTimeAllMicros) / static_cast<float>(dLogg.minDurationMicros) * 100.0f;

        uint32_t seconds = Time::GetSecondsSlow();
        static char timeStringBuffer[20];
        StringUtils::FormatDurationDHMS(seconds, timeStringBuffer, sizeof(timeStringBuffer));

        LogInfo("Main", "============== Diagnostics %s =============", timeStringBuffer);
        LogInfo("Main", "| Type | freq(Hz) | max(us) | maxall(us) | usage(%%) |");
        LogInfo("Main", "| High | %8" PRIu32 " | %7" PRIu32 " | %10" PRIu32 " | %8.3f |", dMain.frequencyHz, maxMain, maxAllMain, usageMain);
        LogInfo("Main", "| Disp | %8" PRIu32 " | %7" PRIu32 " | %10" PRIu32 " | %8.3f |", dDisp.frequencyHz, maxDisp, maxAllDisp, usageDisp);
        LogInfo("Main", "| Logg | %8" PRIu32 " | %7" PRIu32 " | %10" PRIu32 " | %8.3f |", dLogg.frequencyHz, maxLogg, maxAllLogg, usageLogg);
        LogInfo("Main", "=====================================================");

        gScheduler.clearMaxTimes();

    }
}
*/
