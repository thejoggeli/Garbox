// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "GarboxRuntime.h"
#include "core/assert/Assert.h"

namespace Garbox {

GarboxRuntime::GarboxRuntime(const RuntimeAbs::Config& config) : RuntimeAbs(config) {
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

    // call controller tick_phases
    mHeartbeatController.onHeartbeatTick();

    // dispatch events
    dispatchEvents();
}

void GarboxRuntime::onInputTick(){

    // call controller tick_phases
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

    // call controller tick_phases
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();

    // dispatch events
    dispatchEvents();
}

void GarboxRuntime::onRenderTick(){

    // call controller tick_phases
    mDisplayController.onRenderTick();

    // dispatch events
    dispatchEvents();
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){
    switch(header->type){
    case EventType::ActiveBehaviourChanged: {
        const ActiveBehaviourChangedEvent event(header);
        mDisplayController.onActiveBehaviourChanged(event);
        break;
    }
    case EventType::Heartbeat: {
        const HeartbeatEvent event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::BacklightCommand: {
        const BacklightCommandEvent event(header);
        mDisplayController.onBacklightCommand(event);
        break;
    }
    case EventType::FanStatus: {
        const FanStatusEvent event(header);
        mDisplayController.onFanStatus(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::FanSample: {
        const FanSampleEvent event(header);
        mDisplayController.onFanSample(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::FanCommand: {
        const FanCommandEvent event(header);
        mFanController.onFanCommand(event);
        break;
    }
    case EventType::HeatpadStatus: {
        const HeatpadStatusEvent event(header);
        mDisplayController.onHeatpadStatus(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeatpadStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::HeatpadSample: {
        const HeatpadSampleEvent event(header);
        mDisplayController.onHeatpadSample(event);
        break;
    }
    case EventType::HeatpadCommand: {
        const HeatpadCommandEvent event(header);
        mHeatpadController.onHeatpadCommand(event);
        break;
    }
    case EventType::TemperatureStatus: {
        const TemperatureStatusEvent event(header);
        mDisplayController.onTemperatureStatus(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureStatus(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::TemperatureSample: {
        const TemperatureSampleEvent event(header);
        mDisplayController.onTemperatureSample(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureSample(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::ButtonStateChanged: {
        const ButtonStateChangedEvent event(header);
        mI2cPartsController.onButtonStateChanged(event);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonStateChanged(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::ButtonRepeat: {
        const ButtonRepeatEvent event(header);
 
        // send event to active behaviour (if supported)
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonRepeat(event); break;
            default: break; // active behaviour does not support event type
        }
        break;
    }
    case EventType::EncoderStep: {
        const EncoderStepEvent event(header);
 
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