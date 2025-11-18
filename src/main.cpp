
#include "app/StartupSequence.h"
#include "app/SystemRuntime.h"
#include "app/SystemTasks.h"
#include "app/config/AppConfig.h"
#include "app/hardware/adc/AdcInstances.h"
#include "app/hardware/ledc/LedcInstances.h"
#include "app/hardware/gpio/GpioInstances.h"
#include "app/hardware/spi/SpiInstances.h"
#include "app/hardware/timer/TimerInstances.h"
#include "app/parts/StatusLeds.h"
#include "app/providers/PartsProvider.h"
#include "core/assert/AssertHandler.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/rtos/Task.h"
#include "core/time/Time.h"
#include "core/util/StringUtils.h"
#include "core/util/threading/LockGuard.h"
#include "modules/parts/piezo/PiezoPlayer.h"

using namespace Garbox;

static SystemRuntime gAppCore;
static Task gMainTask;

void handleMainTask();
void logProfiler();

void handleAssertDebug(const char* context, const char* message, int32_t arg){

    // print error to log
    LogError("Main", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);

    // turn on error led
    StatusLeds& statusLeds = PartsProvider::GetStatusLeds();
    if(statusLeds.isInitialized()){
        AnimatedLed& errorLed = statusLeds.getLed(StatusLedId::Error);
        errorLed.setBrightness(1.0f);
    }
}

void handleAssertExit(const char* context, const char* message, int32_t arg){
    
    // print error to log
    LogError("Main", "AssertExit! %s %s (arg=%" PRIi32 "|0x%X)", context, message, arg, arg);

    // stop all tasks (automatically excludes the current task running this handler)
    gMainTask.stop();
    SystemTasks::StopAll();

    // get status leds and piezo
    StatusLeds& statusLeds = PartsProvider::GetStatusLeds();
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
    Log::SetLevel(Log::Level::Verbose);
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
    GpioInstances::Init();
    LedcInstances::Init();
    TimerInstances::Init();
    SpiInstances::Init();
    AdcInstances::Init();

    // init all parts
    PartsProvider::Init();

    // start system tasks
    SystemTasks::StartAll();

    // run startup sequence
    StartupSequence startup;
    startup.run();

    // init main app
    gAppCore.init({
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

    // start main app
    gAppCore.start();

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
        // begin main task
        Profiler::Begin(ProfilerId::MainTask);

        // main tick
        Profiler::Begin(ProfilerId::MainTick);
        Time::Tick();
        gAppCore.onMainTick();
        Profiler::End(ProfilerId::MainTick);

        // logging
        Profiler::Begin(ProfilerId::LogTick);
        logProfiler();
        Profiler::End(ProfilerId::LogTick);

        // wait until updateDisplayDuration millis are remaining before current cycle is finished
        // this is done to ensure that the display is always updated at a fixed interval and to 
        // give it enough time to transfer the previous ui state via SPI
        vTaskDelayUntil(&lastWakeTime, mainTickMillis);
        gAppCore.onDisplayTick();
        
        // end main task
        // sleep until next tick
        vTaskDelayUntil(&lastWakeTime, displayTickMillis);
        Profiler::End(ProfilerId::MainTask);
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
        LogInfo("Main", "======================== Diagnostics %s =======================", timeStringBuffer);
        LogInfo("Main", " | ProfilerId         | Count | freq(Hz) | min(us) | avg(us) | max(us) |");
        for (uint32_t i = 0; i < static_cast<uint32_t>(ProfilerId::Count); ++i){
            const ProfilerId id = static_cast<ProfilerId>(i);
            const Profiler::Record& r = Profiler::GetRecord(id);
            const char* idStr = ProfilerIdToString(id);
            LogInfo("Main", " | %-18s | %5" PRIu32 " | %8.3f | %7" PRIu32 " | %7.0f | %7" PRIu32 " |", idStr, r.countLast, r.frequency, r.minDurationLast, r.avgDuration, r.maxDurationLast);
        }
        LogInfo("Main", "=========================================================================");
    }
}

