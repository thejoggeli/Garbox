// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "GarboxRuntime.h"
#include "core/assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/time/Time.h"

namespace Garbox {

static constexpr size_t TickHandlersCount = 6;
static constexpr size_t TickPeriodMillis = 33;

static constexpr uint32_t HeartbeatTickDelayMillis = 0;
static constexpr uint32_t InputTickDelayMillis = 0;
static constexpr uint32_t LogicTickDelayMillis = 0;
static constexpr uint32_t OutputTickDelayMillis = 0;
static constexpr uint32_t LoggingTickDelayMillis = 0;
static constexpr uint32_t RenderTickDelayMillis = 20;

GarboxRuntime::GarboxRuntime(const RuntimeAbs::Config& config):
    RuntimeAbs(config),
    mTickRunner(TickHandlersCount, TickPeriodMillis){

    // set start and end tick handlers
    mTickRunner.setTickStartHandler([this](){ handleTickStart(); });
    mTickRunner.setTickEndHandler([this](){ handleTickEnd(); });

    //  register all tick phases
    mTickRunner.registerTickPhase([this](){ handleHeartbeatTick(); }, HeartbeatTickDelayMillis);
    mTickRunner.registerTickPhase([this](){ handleInputTick(); }, InputTickDelayMillis);
    mTickRunner.registerTickPhase([this](){ handleLogicTick(); }, LogicTickDelayMillis);
    mTickRunner.registerTickPhase([this](){ handleOutputTick(); }, OutputTickDelayMillis);
    mTickRunner.registerTickPhase([this](){ handleLoggingTick(); }, LoggingTickDelayMillis);
    mTickRunner.registerTickPhase([this](){ handleRenderTick(); }, RenderTickDelayMillis);
}

void GarboxRuntime::onRegister(){

    // register all behaviours
    registerBehaviour(&mCalibrationBehaviour);
    registerBehaviour(&mFermentationBehaviour);
    
    // register all controllers
    registerController(&mDisplayController);
    registerController(&mDevtoolsController);
    registerController(&mFanController);
    registerController(&mHeartbeatController);
    registerController(&mHeatpadController);
    registerController(&mInputController);
    registerController(&mI2cPartsController);
}

void GarboxRuntime::onInit(){
    // behaviours and controllers are already initialized when this method is called
    setQueuedBehaviour(&mFermentationBehaviour);
    Profiler::Init();
}

void GarboxRuntime::onStart(){
    // behaviours and controllers are already started when this method is called
    Profiler::SetEnabled(true);
}

void GarboxRuntime::onRun(){
    Time::Start();
    Profiler::Reset();
    mTickRunner.run();
}

void GarboxRuntime::handleTickStart(){
    Time::Tick();
    Profiler::MeasurePeriodic(ProfilerId::MainPeriod);
    Profiler::MeasureBegin(ProfilerId::MainBusy);
    applyQueuedBehaviour();
}

void GarboxRuntime::handleTickEnd(){
    mContext.tickCount++;
    Profiler::MeasureEnd(ProfilerId::MainBusy);
}

void GarboxRuntime::handleHeartbeatTick(){
    Profiler::MeasureScoped profiler(ProfilerId::HeartbeatTick);
    mHeartbeatController.onHeartbeatTick();
    dispatchEvents();
}

void GarboxRuntime::handleInputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::InputTick);
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mInputController.onInputTick();
    mI2cPartsController.onInputTick();
    dispatchEvents();
}

void GarboxRuntime::handleLogicTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LogicTick);
    switch(mActiveBehaviour->getBehaviourId()){
        case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onLogicTick(); break;
        case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onLogicTick(); break;
        default: break; // active behaviour does not support tick type
    }
    dispatchEvents();
}

void GarboxRuntime::handleOutputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::OutputTick);
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatchEvents();
}

void GarboxRuntime::handleLoggingTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LoggingTick);
    mDevtoolsController.onLoggingTick();
    dispatchEvents();
}

void GarboxRuntime::handleRenderTick(){
    Profiler::MeasureScoped profiler(ProfilerId::RenderTick);
    mDisplayController.onRenderTick();
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
        case ControllerId::Devtools: return &mDevtoolsController;
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