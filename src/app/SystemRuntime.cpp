#include "SystemRuntime.h"

#include "app/providers/PartsProvider.h"
#include "core/event/EventView.h"
#include "core/log/Log.h"
#include "modules/parts/heatpad/Heatpad.h"

namespace Garbox {

SystemRuntime::SystemRuntime():
    // init behaviours
    mFermentationBehaviour(ComponentId::FermentationBehaviour),
    // init controllers
    mDisplayController(ComponentId::DisplayController),
    mFanController(ComponentId::FanController),
    mGarboxController(ComponentId::GarboxController),
    mHeatpadController(ComponentId::HeatpadController),
    mHeartbeatController(ComponentId::HeartbeatController){
    // nothing to do
}

void SystemRuntime::onRegisterBehaviours(){
    registerBehaviour(&mFermentationBehaviour);
}

void SystemRuntime::onRegisterControllers(){
    registerController(&mDisplayController);
    registerController(&mFanController);
    registerController(&mGarboxController);
    registerController(&mHeatpadController);
    registerController(&mHeartbeatController);
}

void SystemRuntime::onInit(){
    // behaviours and controllers are already initialized when this function is called
    setQueuedBehaviour(&mFermentationBehaviour);
}

void SystemRuntime::onStart(){
    // behaviours and controllers are already started when this function is called
    // nothing to do
}

void SystemRuntime::onMainTick(){

    // apply next behaviour
    applyQueuedBehaviour();

    // input tick
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    dispatchEvents();

    // regular tick tick
    mHeartbeatController.onTick();
    mGarboxController.onTick();
    dispatchEvents();

    // behaviour logic tick
    AppBehaviourAbs* behaviour = static_cast<AppBehaviourAbs*>(getActiveBehaviour());
    AssertExit(behaviour != nullptr, "SystemRuntime", "no behaviour set");
    behaviour->onLogicTick();

    // output tick
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatchEvents();
}

void SystemRuntime::onDisplayTick(){
    mDisplayController.onTick();
}

void SystemRuntime::onRouteEvent(const Event* event){
    
    // log event
    LogDebug("SystemRuntime", "[Event] %s [id=%" PRIi32 "] %s", 
        EventTypeToString(event->type),
        event->id,
        ComponentIdToString(event->sender.id)
    );

    // get active behaviour
    AppBehaviourAbs* activeBehaviour = static_cast<AppBehaviourAbs*>(getActiveBehaviour());

    // route the event
    switch(event->type){

    // route status updates to behaviour
    case EventType::Heartbeat:          activeBehaviour->onHeartbeat(EventView<EventData::Heartbeat>(event)); break;
    case EventType::FanStatus:          activeBehaviour->onFanStatus(EventView<EventData::FanStatus>(event)); break;
    case EventType::HeatpadStatus:      activeBehaviour->onHeatpadStatus(EventView<EventData::HeatpadStatus>(event)); break;

    // route commands to controllers
    case EventType::FanCommand:         mFanController.onFanCommand(EventView<EventData::FanCommand>(event)); break;
    case EventType::HeatpadCommand:     mHeatpadController.onHeatpadCommand(EventView<EventData::HeatpadCommand>(event)); break;
    
    // special event types
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("SystemRuntime", "invalid event type");
        break;
    }
}

} // namespace
