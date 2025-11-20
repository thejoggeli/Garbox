#include "GarboxRuntime.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "modules/parts/heatpad/Heatpad.h"

#define GarboxDebugRuntime 0

namespace Garbox {

GarboxRuntime::GarboxRuntime():
    // init behaviours
    mFermentationBehaviour(ComponentId::FermentationBehaviour, getContext()),
    // init controllers
    mDisplayController(ComponentId::DisplayController, getContext()),
    mFanController(ComponentId::FanController, getContext()),
    mInputController(ComponentId::InputController, getContext()),
    mHeatpadController(ComponentId::HeatpadController, getContext()),
    mHeartbeatController(ComponentId::HeartbeatController, getContext()),
    mI2cPartsController(ComponentId::I2cPartsController, getContext()){
    // nothing to do
}

void GarboxRuntime::onRegisterBehaviours(){
    registerBehaviour(&mFermentationBehaviour);
}

void GarboxRuntime::onRegisterControllers(){
    registerController(&mDisplayController);
    registerController(&mFanController);
    registerController(&mInputController);
    registerController(&mHeatpadController);
    registerController(&mHeartbeatController);
    registerController(&mI2cPartsController);
}

void GarboxRuntime::onInit(){
    // behaviours and controllers are already initialized when this function is called
    setQueuedBehaviour(&mFermentationBehaviour);
}

void GarboxRuntime::onStart(){
    // behaviours and controllers are already started when this function is called
    // nothing to do
}

void GarboxRuntime::onMainTick(){

    // apply queued behaviour
    applyQueuedBehaviour();

    // get active behaviour 
    AppBehaviourAbs* behaviour = static_cast<AppBehaviourAbs*>(getActiveBehaviour());
    AssertExit(behaviour != nullptr, "GarboxRuntime", "no behaviour set");

    // input tick
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mI2cPartsController.onInputTick();
    dispatchEvents();

    // regular tick tick
    mHeartbeatController.onTick();
    mInputController.onTick();
    dispatchEvents();

    // behaviour logic tick
    behaviour->onLogicTick();
    dispatchEvents();

    // output tick
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    mI2cPartsController.onOutputTick();
    dispatchEvents();
}

void GarboxRuntime::onDisplayTick(){
    mDisplayController.onTick();
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){
    
#if GarboxDebugRuntime
    // log event meta data
    LogDebug("GarboxRuntime", "[Event] %s [id=%" PRIi32 "] %s", 
        EventTypeToString(header->type),
        header->id,
        ComponentIdToString(header->sender.id)
    );
#endif

    // get active behaviour
    AppBehaviourAbs* activeBehaviour = static_cast<AppBehaviourAbs*>(getActiveBehaviour());

    // route the event
    switch(header->type){

    // route status updates to behaviour
    case EventType::Heartbeat:          activeBehaviour->onHeartbeat(EventRead<EventData::Heartbeat>(header)); break;
    case EventType::FanStatus:          activeBehaviour->onFanStatus(EventRead<EventData::FanStatus>(header)); break;
    case EventType::HeatpadStatus:      activeBehaviour->onHeatpadStatus(EventRead<EventData::HeatpadStatus>(header)); break;
    case EventType::TemperatureStatus:  activeBehaviour->onTemperatureStatus(EventRead<EventData::TemperatureStatus>(header)); break;

    // route commands to controllers
    case EventType::FanCommand:         mFanController.onFanCommand(EventRead<EventData::FanCommand>(header)); break;
    case EventType::HeatpadCommand:     mHeatpadController.onHeatpadCommand(EventRead<EventData::HeatpadCommand>(header)); break;
    
    // special event types
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("GarboxRuntime", "invalid event type");
        break;
    }
}

} // namespace
