
#include "app/AppCore.h"
#include "app/parts/StatusLeds.h"
#include "app/SystemTasks.h"
#include "assert/AssertHandler.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/config/AppConfig.h"
#include "global/hardware/adc/AdcInstances.h"
#include "global/hardware/ledc/LedcInstances.h"
#include "global/hardware/gpio/GpioInstances.h"
#include "global/hardware/spi/SpiInstances.h"
#include "global/hardware/timer/TimerInstances.h"
#include "global/providers/PartsProvider.h"
#include "parts/piezo/PiezoPlayer.h"
#include "util/StringUtils.h"
#include "util/threading/LockGuard.h"

using namespace Garbox;

static AppCore gAppCore;
static TaskHandle_t gMainTaskHandle = nullptr;

void mainTask(void* parameter);
void logProfiler();

void handleAssertDebug(const char* context, const char* message, int32_t arg){

    // print error to log
    LogError("AssertHandler", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);

    // turn on error led
    StatusLeds& statusLeds = PartsProvider::GetStatusLeds();
    if(statusLeds.isInitialized()){
        AnimatedLed& errorLed = statusLeds.getLed(StatusLedId::Error);
        errorLed.setBrightness(1.0f);
    }
}

void handleAssertExit(const char* context, const char* message, int32_t arg){
    
    // print error to log
    LogError("AssertHandler", "AssertExit! %s %s (arg=%" PRIi32 "|0x%X)", context, message, arg, arg);

    // stop main task if this was called from another task
    TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
    if(currentTask != gMainTaskHandle){
        vTaskDelete(gMainTaskHandle);
    }

    // stop all system tasks (exclude current task)
    SystemTasks::StopAll(currentTask);

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
        Time::DelayMillis(1000);
    }
}

void setup(){

    Log::Init();
    Log::SetLevel(Log::Level::Verbose);
    
    // assert handlers
    AssertHandler::SetDebugHandler(handleAssertDebug);
    AssertHandler::SetExitHandler(handleAssertExit);

    // install ISR service once globally (safe to call multiple times)
    if (gpio_install_isr_service(0) != ESP_OK){
        TriggerExit("Main", "gpio_install_isr_service failed");
    }

    // init profiler
    Profiler::Setup(ProfilerConfig::Count);
    Profiler::SetEnabled(ProfilerConfig::EnableProfiler);

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

    // init main app
    gAppCore.init();

    // start main app
    gAppCore.start();

    // start profiler
    Profiler::Start();

    // start main task
    xTaskCreatePinnedToCore(
        mainTask,
        AppConfig::MainTaskName,
        AppConfig::MainTaskStackSize,
        nullptr, // parameter
        AppConfig::MainTaskPriority,
        &gMainTaskHandle,
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
        gAppCore.tick();
        Profiler::End(ProfilerConfig::MainTick);

        // logging
        Profiler::Begin(ProfilerConfig::LogTick);
        logProfiler();
        Profiler::End(ProfilerConfig::LogTick);

        // wait until updateDisplayDuration millis are remaining before current cycle is finished
        // this is done to ensure that the display is always updated at a fixed interval and to 
        // give it enough time to transfer the previous ui state via SPI
        vTaskDelayUntil(&lastWakeTime, cycleDuration - updateDisplayDuration);
        
        // end main task
        // sleep until next tick
        vTaskDelayUntil(&lastWakeTime, updateDisplayDuration);
        Profiler::End(ProfilerConfig::MainTask);
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
        for (uint8_t i = 0; i < ProfilerConfig::Count; ++i){
            const Profiler::Record& r = Profiler::GetRecord(i);
            const char* idStr = ProfilerConfig::IdToString(i);
            LogInfo("Main", " | %-18s | %5" PRIu32 " | %8.3f | %7" PRIu32 " | %7.0f | %7" PRIu32 " |", idStr, r.countLast, r.frequency, r.minDurationLast, r.avgDuration, r.maxDurationLast);
        }
        LogInfo("Main", "=========================================================================");
    }
}

