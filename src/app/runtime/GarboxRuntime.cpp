// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "GarboxRuntime.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

GarboxRuntime::GarboxRuntime():
    // call behaviours constructors
    mCalibrationBehaviour(),
    mFermentationBehaviour(),
    // call controllers constructors
    mDisplayController(),
    mFanController(),
    mHeartbeatController(),
    mHeatpadController(),
    mInputController(),
    mI2cPartsController()
{
    // nothing to do
}

void GarboxRuntime::onRegisterBehaviours(){
    registerBehaviour(&mCalibrationBehaviour);
    registerBehaviour(&mFermentationBehaviour);
}

void GarboxRuntime::onRegisterControllers(){
    registerController(&mDisplayController);
    registerController(&mFanController);
    registerController(&mHeartbeatController);
    registerController(&mHeatpadController);
    registerController(&mInputController);
    registerController(&mI2cPartsController);
}

void GarboxRuntime::onInit(){
    // behaviours and controllers are already initialized
    setQueuedBehaviour(&mFermentationBehaviour);
}

void GarboxRuntime::onStart(){
    // behaviours and controllers are already started
}

void GarboxRuntime::onOutputTick(){
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatchEvents();
}

void GarboxRuntime::onRenderTick(){
    mDisplayController.onRenderTick();
    dispatchEvents();
}

void GarboxRuntime::onInputTick(){
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mInputController.onInputTick();
    mI2cPartsController.onInputTick();
    dispatchEvents();
}

void GarboxRuntime::onLogicTick(){
    BaseBehaviourAbs* behaviour = static_cast<BaseBehaviourAbs*>(getActiveBehaviour());
    AssertExit(behaviour != nullptr, "GarboxRuntime", "no behaviour set");
    behaviour->onLogicTick();
    dispatchEvents();
}

void GarboxRuntime::onHeartbeatTick(){
    mHeartbeatController.onHeartbeatTick();
    dispatchEvents();
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){
    switch(header->type){
    case EventType::Heartbeat: {
        const EventRead<EventPayload::Heartbeat> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onHeartbeat(event);
        break;
    }
    case EventType::BacklightCommand: {
        const EventRead<EventPayload::BacklightCommand> event(header);
        mDisplayController.onBacklightCommand(event);
        break;
    }
    case EventType::FanStatus: {
        const EventRead<EventPayload::FanStatus> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onFanStatus(event);
        break;
    }
    case EventType::FanSample: {
        const EventRead<EventPayload::FanSample> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onFanSample(event);
        break;
    }
    case EventType::FanCommand: {
        const EventRead<EventPayload::FanCommand> event(header);
        mFanController.onFanCommand(event);
        break;
    }
    case EventType::HeatpadStatus: {
        const EventRead<EventPayload::HeatpadStatus> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onHeatpadStatus(event);
        break;
    }
    case EventType::HeatpadCommand: {
        const EventRead<EventPayload::HeatpadCommand> event(header);
        mHeatpadController.onHeatpadCommand(event);
        break;
    }
    case EventType::TemperatureStatus: {
        const EventRead<EventPayload::TemperatureStatus> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onTemperatureStatus(event);
        mDisplayController.onTemperatureStatus(event);
        break;
    }
    case EventType::TemperatureSample: {
        const EventRead<EventPayload::TemperatureSample> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onTemperatureSample(event);
        mDisplayController.onTemperatureSample(event);
        break;
    }
    case EventType::ButtonStateChanged: {
        const EventRead<EventPayload::ButtonStateChanged> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onButtonStateChanged(event);
        mI2cPartsController.onButtonStateChanged(event);
        break;
    }
    case EventType::ButtonRepeat: {
        const EventRead<EventPayload::ButtonRepeat> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onButtonRepeat(event);
        break;
    }
    case EventType::EncoderStep: {
        const EventRead<EventPayload::EncoderStep> event(header);
        static_cast<BaseBehaviourAbs*>(getActiveBehaviour())->onEncoderStep(event);
        break;
    }
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("GarboxRuntime", "invalid event type");
        break;
    }
}

ControllerIfc* GarboxRuntime::resolveController(ControllerId id){
    switch(id){
    case ControllerId::Display: return &mDisplayController;
    case ControllerId::Fan: return &mFanController;
    case ControllerId::Heartbeat: return &mHeartbeatController;
    case ControllerId::Heatpad: return &mHeatpadController;
    case ControllerId::Input: return &mInputController;
    case ControllerId::I2cParts: return &mI2cPartsController;
    default:
        TriggerExit("GarboxRuntime", "controller with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

BehaviourIfc* GarboxRuntime::resolveBehaviour(BehaviourId id){
    switch(id){
    case BehaviourId::Calibration: return &mCalibrationBehaviour;
    case BehaviourId::Fermentation: return &mFermentationBehaviour;
    default:
        TriggerExit("GarboxRuntime", "behaviour with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

} // namespace Garbox