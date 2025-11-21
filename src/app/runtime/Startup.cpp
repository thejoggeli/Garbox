
#include "app/runtime/StartupSequence.h"
#include "app/runtime/TaskManager.h"
#include "app/config/AppConfig.h"
#include "app/hardware/HardwareInit.h"
#include "app/providers/PartsProvider.h"
#include "app/runtime/GarboxRuntime.h"
#include "core/assert/AssertHandler.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/rtos/Task.h"
#include "core/time/Time.h"
#include "core/util/StringUtils.h"
#include "core/rtos/LockGuard.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"

using namespace Garbox;

static GarboxRuntime gRuntime;
static Task gMainTask;
static bool gResetProfiler = false;

void handleMainTask();
void logProfiler();

void handleAssertDebug(const char* context, const char* message, int32_t arg){

    // print error to log
    LogError("Main", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);

    // turn on error led
    AnimatedLedGroup& statusLeds = PartsProvider::GetStatusLeds();
    if(statusLeds.isInitialized()){
        AnimatedLed& errorLed = statusLeds.getLed(static_cast<uint8_t>(StatusLedId::Error));
        errorLed.setBrightness(1.0f);
    }
}

void handleAssertExit(const char* context, const char* message, int32_t arg){
    
    // print error to log
    LogError("Main", "AssertExit! %s %s (arg=%" PRIi32 "|0x%X)", context, message, arg, arg);

    // stop all tasks (automatically excludes the current task running this handler)
    TaskManager::StopAll();

    // get status leds and piezo
    AnimatedLedGroup& statusLeds = PartsProvider::GetStatusLeds();
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
        
    // turn off all status leds
    statusLeds.setAllLeds(0.0f);

    // play short blink + beep
    for(uint32_t i = 0; i < 3; i++){
        // leds + tone on
        statusLeds.setAllLeds(1.0f);
        piezoPlayer.setPiezoTone(3500, 0.66f);
        Time::BlockMillis(400);

        // leds + tone off
        statusLeds.setAllLeds(0.0f);
        piezoPlayer.setPiezoEnabled(false);
        Time::BlockMillis(400);
    }

    // turn leds on again
    statusLeds.setAllLeds(1.0f);

    // enter endless loop and periodically print error to log
    while(true){
        LogError("AssertHandler", "AssertExit! %s %s (arg=%" PRIi32 "|0x%X)", context, message, arg, arg);
        Time::BlockMillis(2500);
    }
}

void setup(){

    Log::Init();
    Log::SetLevel(LogLevel::Verbose);
    LogDebug("Main", "log setup complete");
    
    // assert handlers
    AssertHandler::SetDebugHandler(handleAssertDebug);
    AssertHandler::SetExitHandler(handleAssertExit);

    // install ISR service once globally (safe to call multiple times)
    if (gpio_install_isr_service(0) != ESP_OK){
        TriggerExit("Main", "gpio_install_isr_service failed");
    }

    // init profiler
    Profiler::Setup();
    Profiler::SetEnabled(AppConfig::EnableProfiler);

    // init hardware instances
    HardwareInit::Init();

    // init all parts
    PartsProvider::Init();

    // start system tasks
    TaskManager::StartAll();
    TaskManager::RegisterStopHandler([](){
        gMainTask.stop();
    });

    // run startup sequence
    StartupSequence startup;
    startup.run();

    // init main app
    gRuntime.init({
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 128,
    });

    // init main task
    gMainTask.configure(
        AppConfig::MainTaskName,
        AppConfig::MainTaskStackSize,
        AppConfig::MainTaskPriority,
        AppConfig::MainTaskCore
    );
    gMainTask.setHandler(handleMainTask);

    // start profiler
    Profiler::Start();
    gResetProfiler = true;

    // start main app
    gRuntime.start();

    // start main task
    gMainTask.start();

    // setup complete
    LogDebug("Main", "setup complete");
}

void loop(){
    Time::DelayMillis(1000);
}

void handleMainTask(){
    const TickType_t mainTickMillis = pdMS_TO_TICKS(AppConfig::MainTickDurationMillis);
    const TickType_t displayTickMillis = pdMS_TO_TICKS(AppConfig::DisplayTickDurationMillis);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while(true){

        // reset profiler
        if(gResetProfiler){
            Profiler::Start();
            gResetProfiler = false;
        }

        // begin main task
        ProfilerScoped mainTaskProfilerScoped = ProfilerScoped(ProfilerId::MainTask);

        // advance time
        Time::Tick();

        gRuntime.beginTickSequence();

        // main tick
        {
            ProfilerScoped mainTickProfilerScoped = ProfilerScoped(ProfilerId::MainTick);
            gRuntime.onHeartbeatTick();
            gRuntime.onInputTick();
            gRuntime.onLogicTick();
            gRuntime.onOutputTick();
        }

        // logging
        {
            ProfilerScoped logTickProfilerScoped = ProfilerScoped(ProfilerId::LogTick);
            logProfiler();
        }

        // wait until updateDisplayDuration millis are remaining before current cycle is finished
        // this is done to ensure that the display is always updated at a fixed interval and to 
        // give it enough time to transfer the previous ui state via SPI
        {
            vTaskDelayUntil(&lastWakeTime, mainTickMillis);
            ProfilerScoped displayTickProfilerScoped = ProfilerScoped(ProfilerId::DisplayTick);
            gRuntime.onRenderTick();
        }
        
        // end main task
        // sleep until next tick
        vTaskDelayUntil(&lastWakeTime, displayTickMillis);
    }
}

void logProfiler(){
    static uint32_t lastPrint = 0;
    uint32_t now = Time::GetMicros();
    if (now - lastPrint > 30'000'000){
        Profiler::UpdateAll();
        lastPrint = now;
        
        uint32_t seconds = Time::GetSeconds();
        static char timeStringBuffer[20];
        StringUtils::FormatDurationDHMS(seconds, timeStringBuffer, sizeof(timeStringBuffer));
        LogInfo("Main", "===================== Diagnostics %s =====================", timeStringBuffer);
        LogInfo("Main", " | ProfilerId    | Count | freq(Hz) | min(us) | avg(us) | max(us) |");
        for (uint32_t i = 0; i < static_cast<uint32_t>(ProfilerId::Count); ++i){
            const ProfilerId id = static_cast<ProfilerId>(i);
            const Profiler::Record& r = Profiler::GetRecord(id);
            const char* idStr = ProfilerIdToString(id);
            LogInfo("Main", " | %-13s | %5" PRIu32 " | %8.3f | %7" PRIu32 " | %7.0f | %7" PRIu32 " |", idStr, r.countLast, r.frequency, r.minDurationLast, r.avgDuration, r.maxDurationLast);
        }
        LogInfo("Main", "====================================================================");
        gResetProfiler = true;
    }
}

