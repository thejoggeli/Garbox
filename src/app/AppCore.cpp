#include "AppCore.h"

#include "app/providers/PartsProvider.h"
#include "app/StartupSequence.h"
#include "core/event/EventView.h"
#include "core/log/Log.h"
#include "parts/heatpad/Heatpad.h"

namespace Garbox {

AppCore::AppCore(){
    // nothing to do
}

void AppCore::init(){

    // run startup
    StartupSequence startup;
    startup.run();

    // setup event forwarder
    mEventForwarder.setHandler([this](const Event* event){
        handleForwardedEvent(event);
    });

    // init event factory
    const size_t eventFactorySize = 1024;
    mEventFactory.init(eventFactorySize);

    // setup controllers
    mHeartbeatController.setup(mEventFactory, mEventForwarder);
    mGarboxController.setup(mEventFactory, mEventForwarder);
    mFanController.setup(mEventFactory, mEventForwarder);

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
    // clear data pool
    mEventFactory.clearDataPool();

    // tick controllers
    mHeartbeatController.tick();
    mGarboxController.tick();
    mFanController.tick();
    mHeatpadController.tick();
}

void AppCore::displayTick(){
    // TODO trigger UI update
    mDisplayController.tick();
}

void AppCore::handleForwardedEvent(const Event* event){
    LogDebug("AppCore", "handle event: %s", EventTypeToString(event->type));
    routeEvent(event);
}

void AppCore::routeEvent(const Event* event){
    switch(event->type){
    case EventType::Fan:
    case EventType::Button:
        break;
    case EventType::Heartbeat:
        mFanController.onHeartbeatEvent(EventView<HeartbeatEventData>(event));
        break;
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("AppCore", "invalid event type");
        break;
    }
}

} // namespace