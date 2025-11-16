#include "AppCore.h"

#include "app/StartupSequence.h"
#include "global/providers/PartsProvider.h"

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

void AppCore::tick(){

    // tick parts
    PartsProvider::Tick();

    // tick controllers
    mHeartbeatController.tick();
    mGarboxController.tick();
    mFanController.tick();
    
}

} // namespace