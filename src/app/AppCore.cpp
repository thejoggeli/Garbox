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
    for(ControllerIfc* controller : mControllers){
        controller->setup(mEventFactory, mEventForwarder);
    }

    // init controllers
    for(ControllerIfc* controller : mControllers){
        controller->init();
    }
}

void AppCore::start(){
    // start controllers
    for(ControllerIfc* controller : mControllers){
        controller->start();
    }
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
    mDisplayController.tick();
}

void AppCore::handleForwardedEvent(const Event* event){
    LogDebug("AppCore", "handle event: %s", EventTypeToString(event->type));
    routeEvent(event);
}

void AppCore::routeEvent(const Event* event){
    switch(event->type){
    case EventType::Heartbeat:
        mFanController.onHeartbeat(EventView<EventData::Heartbeat>(event));
        break;
    case EventType::FanCommand:
        mFanController.onFanCommand(EventView<EventData::FanCommand>(event));
        break;
    case EventType::FanStatus:
        break;
    case EventType::HeatpadCommand:
        mHeatpadController.onHeatpadCommand(EventView<EventData::HeatpadCommand>(event));
        break;
    case EventType::HeatpadStatus:
        break;
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("AppCore", "invalid event type");
        break;
    }
}

} // namespace