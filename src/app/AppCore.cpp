#include "AppCore.h"

#include "app/providers/PartsProvider.h"
#include "app/StartupSequence.h"

namespace Garbox {

AppCore::AppCore(){
    // nothing to do
}

void AppCore::init(){

    // run startup
    StartupSequence startup;
    startup.run();

    // init controllers
    mHeartbeatController.init();
    mGarboxController.init();
    mFanController.init();
}

void AppCore::start(){
    // start controllers
    mHeartbeatController.start();
    mGarboxController.start();
    mFanController.start();
}

void AppCore::mainTick(){

    // tick parts
    PartsProvider::Tick();

    // tick controllers
    mHeartbeatController.tick();
    mGarboxController.tick();
    mFanController.tick();
    
}

void AppCore::displayTick(){
    // TODO
    // trigger UI update + notify display
}

} // namespace