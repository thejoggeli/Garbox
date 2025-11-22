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
    setQueuedBehaviour(&mCalibrationBehaviour);
}

void GarboxRuntime::onStart(){
    // behaviours and controllers are already started
}

void GarboxRuntime::onHeartbeatTick(){

    // call controller ticks
    mHeartbeatController.onHeartbeatTick();

    // dispatch events
    dispatchEvents();
}
void GarboxRuntime::onInputTick(){

    // call controller ticks
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mInputController.onInputTick();
    mI2cPartsController.onInputTick();

    // dispatch events
    dispatchEvents();
}
void GarboxRuntime::onLogicTick(){

    // call behaviour tick
    switch(mActiveBehaviour->getBehaviourId()){
        case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onLogicTick(); break;
        case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onLogicTick(); break;
        default: break; // active behaviour does not support tick type
    }

    // dispatch events
    dispatchEvents();
}
void GarboxRuntime::onOutputTick(){

    // call controller ticks
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();

    // dispatch events
    dispatchEvents();
}
void GarboxRuntime::onRenderTick(){

    // call controller ticks
    mDisplayController.onRenderTick();

    // dispatch events
    dispatchEvents();
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){
    switch(header->type){
    case EventType::Heartbeat: {
        const EventRead<EventPayload::Heartbeat> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::BacklightCommand: {
        const EventRead<EventPayload::BacklightCommand> event(header);
        mDisplayController.onBacklightCommand(event);
        break;
    }
    case EventType::FanStatus: {
        const EventRead<EventPayload::FanStatus> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::FanSample: {
        const EventRead<EventPayload::FanSample> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::FanCommand: {
        const EventRead<EventPayload::FanCommand> event(header);
        mFanController.onFanCommand(event);
        break;
    }
    case EventType::HeatpadStatus: {
        const EventRead<EventPayload::HeatpadStatus> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeatpadStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::HeatpadCommand: {
        const EventRead<EventPayload::HeatpadCommand> event(header);
        mHeatpadController.onHeatpadCommand(event);
        break;
    }
    case EventType::TemperatureStatus: {
        const EventRead<EventPayload::TemperatureStatus> event(header);
        mDisplayController.onTemperatureStatus(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::TemperatureSample: {
        const EventRead<EventPayload::TemperatureSample> event(header);
        mDisplayController.onTemperatureSample(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureSample(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::ButtonStateChanged: {
        const EventRead<EventPayload::ButtonStateChanged> event(header);
        mI2cPartsController.onButtonStateChanged(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonStateChanged(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::ButtonRepeat: {
        const EventRead<EventPayload::ButtonRepeat> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonRepeat(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::EncoderStep: {
        const EventRead<EventPayload::EncoderStep> event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onEncoderStep(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("GarboxRuntime", "invalid event type");
        break;
    }
}

ControllerAbs* GarboxRuntime::resolveController(ControllerId id){
    switch(id){
        case ControllerId::Display: return &mDisplayController;
        case ControllerId::Fan: return &mFanController;
        case ControllerId::Heartbeat: return &mHeartbeatController;
        case ControllerId::Heatpad: return &mHeatpadController;
        case ControllerId::Input: return &mInputController;
        case ControllerId::I2cParts: return &mI2cPartsController;
        default: TriggerExit("GarboxRuntime", "controller with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

BehaviourAbs* GarboxRuntime::resolveBehaviour(BehaviourId id){
    switch(id){
        case BehaviourId::Calibration: return &mCalibrationBehaviour;
        case BehaviourId::Fermentation: return &mFermentationBehaviour;
        default: TriggerExit("GarboxRuntime", "behaviour with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

} // namespace Garbox