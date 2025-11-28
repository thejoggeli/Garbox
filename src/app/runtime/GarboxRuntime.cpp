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

GarboxRuntime::GarboxRuntime():
    RuntimeAbs({
        .maxComponents = 11,
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 128,
    }),
    mTickRunner(TickHandlersCount, TickPeriodMillis),
    mEventReplay(mMainScreen, mDebugScreen){

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
    registerComponent(&mCalibrationBehaviour);
    registerComponent(&mFermentationBehaviour);
    registerComponent(&mDisplayController);
    registerComponent(&mDevtoolsController);
    registerComponent(&mFanController);
    registerComponent(&mHeartbeatController);
    registerComponent(&mHeatpadController);
    registerComponent(&mInputController);
    registerComponent(&mI2cPartsController);
    registerComponent(&mMainScreen);
    registerComponent(&mDebugScreen);
}

void GarboxRuntime::onInit(){
    // behaviours and controllers are already initialized when this method is called
    setQueuedBehaviour(&mFermentationBehaviour);
    setQueuedScreen(&mMainScreen);
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

void GarboxRuntime::onActiveBehaviourChanged(){
    // nothing to do
}

void GarboxRuntime::onActiveScreenChanged(){
    mEventReplay.replay();
}

void GarboxRuntime::handleTickStart(){
    Time::Tick();
    Profiler::MeasurePeriodic(ProfilerId::MainPeriod);
    Profiler::MeasureBegin(ProfilerId::MainBusy);
    applyQueuedBehaviour();
    applyQueuedScreen();
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
    case EventType::Heartbeat: {
        const HeartbeatEvent event(header);
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onHeartbeat(event);
            mFermentationBehaviour.onHeartbeat(event);
            mDebugScreen.onHeartbeat(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeartbeat(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onHeartbeat(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::FermentationStatus: {
        const FermentationStatusEvent event(header);
        mDisplayController.onFermentationStatus(event);
        if(header->sendToInactiveComponents){
            mMainScreen.onFermentationStatus(event);
        }
        else {
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFermentationStatus(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::DisplayCommand: {
        const DisplayCommandEvent event(header);
        mDisplayController.onDisplayCommand(event);
        if(header->sendToInactiveComponents){
            mMainScreen.onDisplayCommand(event);
        }
        else {
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onDisplayCommand(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::DisplayStatus: {
        const DisplayStatusEvent event(header);
        if(header->sendToInactiveComponents){
            mMainScreen.onDisplayStatus(event);
        }
        else {
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onDisplayStatus(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::FanStatus: {
        const FanStatusEvent event(header);
        mDisplayController.onFanStatus(event);
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onFanStatus(event);
            mFermentationBehaviour.onFanStatus(event);
            mMainScreen.onFanStatus(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanStatus(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFanStatus(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::FanSample: {
        const FanSampleEvent event(header);
        mDisplayController.onFanSample(event);
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onFanSample(event);
            mFermentationBehaviour.onFanSample(event);
            mMainScreen.onFanSample(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanSample(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFanSample(event); break;
                default: break; // active screen does not support event type
            }
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
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onHeatpadStatus(event);
            mMainScreen.onHeatpadStatus(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeatpadStatus(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onHeatpadStatus(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::HeatpadSample: {
        const HeatpadSampleEvent event(header);
        mDisplayController.onHeatpadSample(event);
        if(header->sendToInactiveComponents){
            mMainScreen.onHeatpadSample(event);
        }
        else {
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onHeatpadSample(event); break;
                default: break; // active screen does not support event type
            }
        }
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
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onTemperatureStatus(event);
            mMainScreen.onTemperatureStatus(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureStatus(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onTemperatureStatus(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::TemperatureSample: {
        const TemperatureSampleEvent event(header);
        mDisplayController.onTemperatureSample(event);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onTemperatureSample(event);
            mMainScreen.onTemperatureSample(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureSample(event); break;
                default: break; // active behaviour does not support event type
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onTemperatureSample(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::ButtonStateChanged: {
        const ButtonStateChangedEvent event(header);
        mI2cPartsController.onButtonStateChanged(event);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonStateChanged(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonStateChanged(event); break;
                default: break; // active behaviour does not support event type
            }
        }
        break;
    }
    case EventType::ButtonRepeat: {
        const ButtonRepeatEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonRepeat(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonRepeat(event); break;
                default: break; // active behaviour does not support event type
            }
        }
        break;
    }
    case EventType::EncoderStep: {
        const EncoderStepEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onEncoderStep(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onEncoderStep(event); break;
                default: break; // active behaviour does not support event type
            }
        }
        break;
    }
    case EventType::ActiveBehaviourChanged: {
        const ActiveBehaviourChangedEvent event(header);
        mDisplayController.onActiveBehaviourChanged(event);
        if(header->sendToInactiveComponents){
            mMainScreen.onActiveBehaviourChanged(event);
        }
        else {
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onActiveBehaviourChanged(event); break;
                default: break; // active screen does not support event type
            }
        }
        break;
    }
    case EventType::ActiveScreenChanged: {
        break;
    }
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("GarboxRuntime", "invalid event type");
        break;
    }
}

BehaviourAbs* GarboxRuntime::resolveBehaviour(BehaviourId id){
    switch(id){
        case BehaviourId::Calibration: return &mCalibrationBehaviour;
        case BehaviourId::Fermentation: return &mFermentationBehaviour;
        default: TriggerExit("GarboxRuntime", "behaviour with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
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

ScreenAbs* GarboxRuntime::resolveScreen(ScreenId id){
    switch(id){
        case ScreenId::Main: return &mMainScreen;
        case ScreenId::Debug: return &mDebugScreen;
        default: TriggerExit("GarboxRuntime", "screen with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

} // namespace Garbox