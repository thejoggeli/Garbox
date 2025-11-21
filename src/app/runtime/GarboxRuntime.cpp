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

    // heartbeat tick
    mHeartbeatController.onHeartbeatTick();
    dispatchEvents();

    // input tick
    mInputController.onInputTick();
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mI2cPartsController.onInputTick();
    dispatchEvents();

    // behaviour logic tick
    behaviour->onLogicTick();
    dispatchEvents();

    // output tick
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatchEvents();
}

void GarboxRuntime::onDisplayTick(){
    // render tick
    mDisplayController.onRenderTick();
    dispatchEvents();
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
    case EventType::Heartbeat:          activeBehaviour->onHeartbeat(EventRead<EventPayload::Heartbeat>(header)); break;
    case EventType::FanStatus:          activeBehaviour->onFanStatus(EventRead<EventPayload::FanStatus>(header)); break;
    case EventType::FanSample:          activeBehaviour->onFanSample(EventRead<EventPayload::FanSample>(header)); break;
    case EventType::HeatpadStatus:      activeBehaviour->onHeatpadStatus(EventRead<EventPayload::HeatpadStatus>(header)); break;
    case EventType::TemperatureStatus:  activeBehaviour->onTemperatureStatus(EventRead<EventPayload::TemperatureStatus>(header)); break;
    case EventType::TemperatureSample:  activeBehaviour->onTemperatureSample(EventRead<EventPayload::TemperatureSample>(header)); break;

    // route commands to controllers
    case EventType::FanCommand:         mFanController.onFanCommand(EventRead<EventPayload::FanCommand>(header)); break;
    case EventType::HeatpadCommand:     mHeatpadController.onHeatpadCommand(EventRead<EventPayload::HeatpadCommand>(header)); break;
    
    // special event types
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("GarboxRuntime", "invalid event type");
        break;
    }
}

} // namespace
