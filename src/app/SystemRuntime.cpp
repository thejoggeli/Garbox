#include "SystemRuntime.h"

#include "app/providers/PartsProvider.h"
#include "core/event/EventView.h"
#include "core/log/Log.h"
#include "modules/parts/heatpad/Heatpad.h"

namespace Garbox {

SystemRuntime::SystemRuntime():
    // init members
    mDisplayController(ControllerId::Display),
    mFanController(ControllerId::Fan),
    mGarboxController(ControllerId::Garbox),
    mHeatpadController(ControllerId::Heatpad),
    mHeartbeatController(ControllerId::Heartbeat){
    // nothing to do
}

void SystemRuntime::onRegisterControllers(){
    registerController(&mDisplayController);
    registerController(&mFanController);
    registerController(&mGarboxController);
    registerController(&mHeatpadController);
    registerController(&mHeartbeatController);
}

void SystemRuntime::onInit(){
    // controllers are already initialized when this function is called
}

void SystemRuntime::onStart(){
    // controllers are already started when this function is called
    // nothing to do
}

void SystemRuntime::onMainTick(){

    // input tick
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    dispatchEvents();

    // regular tick tick
    mHeartbeatController.onTick();
    mGarboxController.onTick();
    dispatchEvents();

    // behaviour tick
    // mActiveBehaviour.tick();

    // output tick
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatchEvents();

}

void SystemRuntime::onDisplayTick(){
    mDisplayController.onRenderTick();
}

void SystemRuntime::onRouteEvent(const Event* event){
    LogDebug("SystemRuntime", "Event: %s", EventTypeToString(event->type));
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
        TriggerDebug("SystemRuntime", "invalid event type");
        break;
    }
}

} // namespace