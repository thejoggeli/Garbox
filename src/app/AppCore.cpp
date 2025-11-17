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

    // init event factory
    const size_t eventFactorySize = 1024;
    mEventFactory.init(eventFactorySize);

    // setup controllers
    mHeartbeatController.setup(ControllerId::Heartbeat, mEventFactory);
    mGarboxController.setup(ControllerId::Garbox, mEventFactory);
    mFanController.setup(ControllerId::Fan, mEventFactory);

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
    mGarboxController.onDisplayTick();
}

} // namespace