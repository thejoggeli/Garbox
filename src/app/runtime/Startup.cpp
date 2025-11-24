
#include "app/runtime/StartupSequence.h"
#include "app/runtime/TaskManager.h"
#include "app/config/AppConfig.h"
#include "app/hardware/HardwareInit.h"
#include "app/providers/PartsProvider.h"
#include "app/runtime/GarboxRuntime.h"
#include "core/assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/rtos/Task.h"
#include "core/time/Time.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"

using namespace Garbox;

static GarboxRuntime gRuntime({
    .eventPoolSizeBytes = AppConfig::RuntimeEventPoolSizeBytes,
    .eventQueueLength = AppConfig::RuntimeEventQueueLength,    
});
static Task gMainTask;

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
    gRuntime.init();

    // init main task
    gMainTask.configure(
        AppConfig::MainTaskName,
        AppConfig::MainTaskStackSize,
        AppConfig::MainTaskPriority,
        AppConfig::MainTaskCore
    );
    gMainTask.setHandler(handleMainTask);

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
    gRuntime.run();
}
