
#include "app/config/AppConfig.h"
#include "app/providers/PartsProvider.h"
#include "app/system/SafeOutputManager.h"
#include "app/system/StartupSequence.h"
#include "app/system/TaskManager.h"

#include "core/assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/lvgl/LvglProvider.h"
#include "core/rtos/Task.h"
#include "core/time/Time.h"

#include "generated/hardware/HardwareInit.h"
#include "generated/runtime/Runtime.h"

#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"

using namespace Garbox;

void handleMainTask(const TaskStopToken& stopToken);

Runtime& getRuntime(){
    static Runtime instance;
    return instance;
}

Task& getMainTask(){
    static Task instance;
    return instance;
}

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

    // Latch hazardous outputs safe before any potentially blocking shutdown.
    SafeOutputManager::Apply();

    // Stop all managed tasks cooperatively. The current task becomes the
    // fatal-reporting context and is only requested to stop.
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

    // Install the per-pin GPIO ISR service exactly once, before parts attach
    // their instance handlers.
    if (gpio_install_isr_service(0) != ESP_OK){
        TriggerExit("Main", "gpio_install_isr_service failed");
    }

    // init hardware instances
    HardwareInit::Init();

    // init all parts
    PartsProvider::Init();

    // register the minimal, allocation-free fatal output path
    SafeOutputManager::Init();

    // init lvgl
    LvglProvider::Init(AppConfig::DisplayWidth, AppConfig::DisplayHeight);

    // Register MainTask first so fatal shutdown prevents new application
    // output commands before waiting for auxiliary workers.
    TaskManager::RegisterStopHandler([](){
        getMainTask().stop();
    });
    TaskManager::StartAll();

    // run startup sequence
    StartupSequence startup;
    startup.run();

    // init main app
    getRuntime().init();

    // init main task
    getMainTask().configure(
        AppConfig::MainTaskName,
        AppConfig::MainTaskStackSize,
        AppConfig::MainTaskPriority,
        AppConfig::MainTaskCore
    );
    getMainTask().setHandler(handleMainTask);

    // start main task
    getMainTask().start();
}

void loop(){
    Time::DelayMillis(1000);
}

void handleMainTask(const TaskStopToken& stopToken){
    LogDebug("Main", "main task started");
    getRuntime().start();
    getRuntime().run(stopToken);
}
