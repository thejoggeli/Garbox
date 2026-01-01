// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "Runtime.h"

#include "core/assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/time/Time.h"

namespace Garbox {

static constexpr size_t TickHandlersCount = 7;
static constexpr size_t TickPeriodMillis = 33;

static constexpr uint32_t HeartbeatTickDelayMillis = 0;
static constexpr uint32_t SensorReadTickDelayMillis = 0;
static constexpr uint32_t UserInputTickDelayMillis = 0;
static constexpr uint32_t LogicTickDelayMillis = 0;
static constexpr uint32_t ActorWriteTickDelayMillis = 0;
static constexpr uint32_t LoggingTickDelayMillis = 0;
static constexpr uint32_t RenderTickDelayMillis = 20;

Runtime::Runtime():
    RuntimeAbs(RuntimeAbs::Config {
        .numComponents = 15,
        .numStates = 12,
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 128,
        .maxDispatchRecursionDepth = 3
    }),
    mTickRunner(TickHandlersCount, TickPeriodMillis, this),
    mStateRegistry(){

    // register states
    registerState(&mStateRegistry.getDisplayStatus());
    registerState(&mStateRegistry.getDisplayDiagnostics());
    registerState(&mStateRegistry.getFanStatus());
    registerState(&mStateRegistry.getFanSample());
    registerState(&mStateRegistry.getFermentationStatus());
    registerState(&mStateRegistry.getHeatpadStatus());
    registerState(&mStateRegistry.getHeatpadSample());
    registerState(&mStateRegistry.getHeatpadProgress());
    registerState(&mStateRegistry.getTemperatureStatus());
    registerState(&mStateRegistry.getTemperatureSample());
    registerState(&mStateRegistry.getActiveBehaviour());
    registerState(&mStateRegistry.getActiveScreen());

    // register components
    registerComponent(&mCalibrationBehaviour);
    registerComponent(&mFermentationBehaviour);
    registerComponent(&mDisplayController);
    registerComponent(&mDevtoolsController);
    registerComponent(&mFanController);
    registerComponent(&mHeartbeatController);
    registerComponent(&mHeatpadController);
    registerComponent(&mInputController);
    registerComponent(&mI2cPartsController);
    registerComponent(&mTimeSeriesController);
    registerComponent(&mSimpleScreen);
    registerComponent(&mMainScreen);
    registerComponent(&mDebugScreen);
    registerComponent(&mEventLogScreen);
    registerComponent(&mStateLogScreen);

    // set start and end tick handlers
    mTickRunner.setTickStartHandler(handleTickStart);
    mTickRunner.setTickEndHandler(handleTickEnd);

    //  register all tick phases
    mTickRunner.registerTickPhase(handleHeartbeatTick, HeartbeatTickDelayMillis);
    mTickRunner.registerTickPhase(handleSensorReadTick, SensorReadTickDelayMillis);
    mTickRunner.registerTickPhase(handleUserInputTick, UserInputTickDelayMillis);
    mTickRunner.registerTickPhase(handleLogicTick, LogicTickDelayMillis);
    mTickRunner.registerTickPhase(handleActorWriteTick, ActorWriteTickDelayMillis);
    mTickRunner.registerTickPhase(handleLoggingTick, LoggingTickDelayMillis);
    mTickRunner.registerTickPhase(handleRenderTick, RenderTickDelayMillis);

    // bind 'CalibrationBehaviour' states
    mCalibrationBehaviour.mStates.emplace(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample()
    );

    // bind 'FermentationBehaviour' states
    mFermentationBehaviour.mStates.emplace(
        mStateRegistry.getFermentationStatus(),
        mStateRegistry.getFanSample(),
        mStateRegistry.getFanStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getTemperatureStatus()
    );

    // bind 'DisplayController' states
    mDisplayController.mStates.emplace(
        mStateRegistry.getDisplayStatus(),
        mStateRegistry.getDisplayDiagnostics()
    );

    // bind 'FanController' states
    mFanController.mStates.emplace(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample()
    );

    // bind 'HeatpadController' states
    mHeatpadController.mStates.emplace(
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getHeatpadSample(),
        mStateRegistry.getHeatpadProgress()
    );

    // bind 'I2cPartsController' states
    mI2cPartsController.mStates.emplace(
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample()
    );

    // bind 'TimeSeriesController' states
    mTimeSeriesController.mStates.emplace(
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getFermentationStatus()
    );

    // bind 'SimpleScreen' states
    mSimpleScreen.mStates.emplace(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample(),
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getHeatpadSample(),
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getFermentationStatus()
    );

    // bind 'MainScreen' states
    mMainScreen.mStates.emplace(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample(),
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getHeatpadSample(),
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getFermentationStatus()
    );

    // bind 'DebugScreen' states
    mDebugScreen.mStates.emplace(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample(),
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getHeatpadSample(),
        mStateRegistry.getHeatpadProgress(),
        mStateRegistry.getDisplayStatus(),
        mStateRegistry.getDisplayDiagnostics(),
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getActiveBehaviour(),
        mStateRegistry.getActiveScreen(),
        mStateRegistry.getFermentationStatus()
    );

}

void Runtime::onInit(){
    // behaviours and controllers are already initialized when this method is called
    setQueuedBehaviour(&mFermentationBehaviour);
    setQueuedScreen(&mSimpleScreen);
    Profiler::Init();
}

void Runtime::onStart(){
    // behaviours and controllers are already started when this method is called
    Profiler::SetEnabled(true);
}

void Runtime::onRun(){
    Time::Start();
    Profiler::Reset();
    mTickRunner.run();
}

void Runtime::onActiveBehaviourChanged(){
    mStateRegistry.getActiveBehaviour().setBehaviour(mActiveBehaviour->getBehaviourId());
}

void Runtime::onActiveScreenChanged(){
    mStateRegistry.getActiveScreen().setScreen(mActiveScreen->getScreenId());
}

void Runtime::handleTickStart(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Time::Tick();
    Profiler::MeasurePeriodic(ProfilerId::MainPeriod);
    Profiler::MeasureBegin(ProfilerId::MainBusy);
    self->applyQueuedBehaviour();
}

void Runtime::handleTickEnd(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    self->mContext.tickCount++;
    Profiler::MeasureEnd(ProfilerId::MainBusy);
}

void Runtime::handleHeartbeatTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::HeartbeatTick);
    self->mHeartbeatController.onHeartbeatTick();
    self->dispatch();
}

void Runtime::handleSensorReadTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::SensorReadTick);
    self->mFanController.onSensorReadTick();
    self->mHeatpadController.onSensorReadTick();
    self->mInputController.onSensorReadTick();
    self->mI2cPartsController.onSensorReadTick();
    self->dispatch();
}

void Runtime::handleUserInputTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::UserInputTick);
    switch(self->mActiveScreen->getScreenId()){
        case ScreenId::Main:
            static_cast<MainScreen*>(self->mActiveScreen)->onUserInputTick();
        break;
        default: break; // active behaviour does not receive tick type
    }
    self->dispatch();
}

void Runtime::handleLogicTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::LogicTick);
    switch(self->mActiveBehaviour->getBehaviourId()){
        case BehaviourId::Calibration:
            static_cast<CalibrationBehaviour*>(self->mActiveBehaviour)->onLogicTick();
            break;
        case BehaviourId::Fermentation:
            static_cast<FermentationBehaviour*>(self->mActiveBehaviour)->onLogicTick();
            break;
        default: break; // active behaviour does not receive tick type
    }
    self->dispatch();
}

void Runtime::handleActorWriteTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::ActorWriteTick);
    self->mFanController.onActorWriteTick();
    self->mHeatpadController.onActorWriteTick();
    self->mTimeSeriesController.onActorWriteTick();
    self->dispatch();
}

void Runtime::handleLoggingTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::LoggingTick);
    self->mDevtoolsController.onLoggingTick();
    self->dispatch();
}

void Runtime::handleRenderTick(void* ctx){
    Runtime* self = static_cast<Runtime*>(ctx);
    Profiler::MeasureScoped profiler(ProfilerId::RenderTick);
    self->mDisplayController.onRenderTick();
    self->dispatch();
}

void Runtime::onRouteStateChanged(const StateAbs& state){

    // read all state changes
    mStateLogScreen.onStateChanged(state);
    
    switch(state.type()){
    case StateType::DisplayStatus: {
        const DisplayStatusState& displayStatus = static_cast<const DisplayStatusState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onDisplayStatusStateChanged(displayStatus); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::DisplayDiagnostics: {
        const DisplayDiagnosticsState& displayDiagnostics = static_cast<const DisplayDiagnosticsState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onDisplayDiagnosticsStateChanged(displayDiagnostics); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::FanStatus: {
        const FanStatusState& fanStatus = static_cast<const FanStatusState&>(state);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatusStateChanged(fanStatus); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanStatusStateChanged(fanStatus); break;
            default: break; // active behaviour does not read state
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onFanStatusStateChanged(fanStatus); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFanStatusStateChanged(fanStatus); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onFanStatusStateChanged(fanStatus); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::FanSample: {
        const FanSampleState& fanSample = static_cast<const FanSampleState&>(state);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanSampleStateChanged(fanSample); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanSampleStateChanged(fanSample); break;
            default: break; // active behaviour does not read state
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onFanSampleStateChanged(fanSample); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFanSampleStateChanged(fanSample); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onFanSampleStateChanged(fanSample); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::FermentationStatus: {
        const FermentationStatusState& fermentationStatus = static_cast<const FermentationStatusState&>(state);
        mTimeSeriesController.onFermentationStatusStateChanged(fermentationStatus);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onFermentationStatusStateChanged(fermentationStatus); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onFermentationStatusStateChanged(fermentationStatus); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onFermentationStatusStateChanged(fermentationStatus); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::HeatpadStatus: {
        const HeatpadStatusState& heatpadStatus = static_cast<const HeatpadStatusState&>(state);
        mTimeSeriesController.onHeatpadStatusStateChanged(heatpadStatus);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onHeatpadStatusStateChanged(heatpadStatus); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onHeatpadStatusStateChanged(heatpadStatus); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onHeatpadStatusStateChanged(heatpadStatus); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::HeatpadSample: {
        const HeatpadSampleState& heatpadSample = static_cast<const HeatpadSampleState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onHeatpadSampleStateChanged(heatpadSample); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onHeatpadSampleStateChanged(heatpadSample); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onHeatpadSampleStateChanged(heatpadSample); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::HeatpadProgress: {
        const HeatpadProgressState& heatpadProgress = static_cast<const HeatpadProgressState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onHeatpadProgressStateChanged(heatpadProgress); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::TemperatureStatus: {
        const TemperatureStatusState& temperatureStatus = static_cast<const TemperatureStatusState&>(state);
        mTimeSeriesController.onTemperatureStatusStateChanged(temperatureStatus);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureStatusStateChanged(temperatureStatus); break;
            default: break; // active behaviour does not read state
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onTemperatureStatusStateChanged(temperatureStatus); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onTemperatureStatusStateChanged(temperatureStatus); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onTemperatureStatusStateChanged(temperatureStatus); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::TemperatureSample: {
        const TemperatureSampleState& temperatureSample = static_cast<const TemperatureSampleState&>(state);
        mTimeSeriesController.onTemperatureSampleStateChanged(temperatureSample);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureSampleStateChanged(temperatureSample); break;
            default: break; // active behaviour does not read state
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onTemperatureSampleStateChanged(temperatureSample); break;
            case ScreenId::Main: static_cast<MainScreen*>(mActiveScreen)->onTemperatureSampleStateChanged(temperatureSample); break;
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onTemperatureSampleStateChanged(temperatureSample); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::ActiveBehaviour: {
        const ActiveBehaviourState& activeBehaviour = static_cast<const ActiveBehaviourState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onActiveBehaviourStateChanged(activeBehaviour); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::ActiveScreen: {
        const ActiveScreenState& activeScreen = static_cast<const ActiveScreenState&>(state);
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: static_cast<DebugScreen*>(mActiveScreen)->onActiveScreenStateChanged(activeScreen); break;
            default: break; // active screen does not read state 
        }
        break;
    }
    case StateType::Null:
    case StateType::Count:
        TriggerDebug("Runtime", "invalid state type");
        break;
    }
}

void Runtime::onRouteEvent(const EventHeader* header){

    // receive all events
    mEventLogScreen.onEvent(header);

    // route event to components
    switch(header->type){
    case EventType::Heartbeat: {
        const HeartbeatEvent event(header);
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onHeartbeatEvent(event);
            mFermentationBehaviour.onHeartbeatEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onHeartbeatEvent(event); break;
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onHeartbeatEvent(event); break;
                default: break; // active behaviour does not receive 'Heartbeat' event
            }
        }
        break;
    }
    case EventType::DisplayCommand: {
        const DisplayCommandEvent event(header);
        mDisplayController.onDisplayCommandEvent(event);
        break;
    }
    case EventType::FanCommand: {
        const FanCommandEvent event(header);
        mFanController.onFanCommandEvent(event);
        break;
    }
    case EventType::HeatpadCommand: {
        const HeatpadCommandEvent event(header);
        mHeatpadController.onHeatpadCommandEvent(event);
        break;
    }
    case EventType::Button: {
        const ButtonEvent event(header);
        mI2cPartsController.onButtonEvent(event);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonEvent(event);
            mSimpleScreen.onButtonEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonEvent(event); break;
                default: break; // active behaviour does not receive 'Button' event
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onButtonEvent(event); break;
                default: break; // active screen does not receive 'Button' event
            }
        }
        break;
    }
    case EventType::ButtonRepeat: {
        const ButtonRepeatEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonRepeatEvent(event);
            mSimpleScreen.onButtonRepeatEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonRepeatEvent(event); break;
                default: break; // active behaviour does not receive 'ButtonRepeat' event
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onButtonRepeatEvent(event); break;
                default: break; // active screen does not receive 'ButtonRepeat' event
            }
        }
        break;
    }
    case EventType::EncoderStep: {
        const EncoderStepEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onEncoderStepEvent(event);
            mSimpleScreen.onEncoderStepEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onEncoderStepEvent(event); break;
                default: break; // active behaviour does not receive 'EncoderStep' event
            }
            switch(mActiveScreen->getScreenId()){
                case ScreenId::Simple: static_cast<SimpleScreen*>(mActiveScreen)->onEncoderStepEvent(event); break;
                default: break; // active screen does not receive 'EncoderStep' event
            }
        }
        break;
    }
    case EventType::RequestFanMode: {
        const RequestFanModeEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onRequestFanModeEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onRequestFanModeEvent(event); break;
                default: break; // active behaviour does not receive 'RequestFanMode' event
            }
        }
        break;
    }
    case EventType::RequestFermentationMode: {
        const RequestFermentationModeEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onRequestFermentationModeEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onRequestFermentationModeEvent(event); break;
                default: break; // active behaviour does not receive 'RequestFermentationMode' event
            }
        }
        break;
    }
    case EventType::RequestTargetTemperature: {
        const RequestTargetTemperatureEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onRequestTargetTemperatureEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onRequestTargetTemperatureEvent(event); break;
                default: break; // active behaviour does not receive 'RequestTargetTemperature' event
            }
        }
        break;
    }
    case EventType::ActiveBehaviourChanged:
        // no component receives this event type
        break;
    case EventType::ActiveScreenChanged:
        // no component receives this event type
        break;
    case EventType::Null:
    case EventType::Count:
        TriggerDebug("Runtime", "invalid event type");
        break;
    }
}

BehaviourAbs* Runtime::resolveBehaviour(BehaviourId id){
    switch(id){
        case BehaviourId::Calibration: return &mCalibrationBehaviour;
        case BehaviourId::Fermentation: return &mFermentationBehaviour;
        default: TriggerExit("Runtime", "behaviour with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

ControllerAbs* Runtime::resolveController(ControllerId id){
    switch(id){
        case ControllerId::Display: return &mDisplayController;
        case ControllerId::Devtools: return &mDevtoolsController;
        case ControllerId::Fan: return &mFanController;
        case ControllerId::Heartbeat: return &mHeartbeatController;
        case ControllerId::Heatpad: return &mHeatpadController;
        case ControllerId::Input: return &mInputController;
        case ControllerId::I2cParts: return &mI2cPartsController;
        case ControllerId::TimeSeries: return &mTimeSeriesController;
        default: TriggerExit("Runtime", "controller with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

ScreenAbs* Runtime::resolveScreen(ScreenId id){
    switch(id){
        case ScreenId::Simple: return &mSimpleScreen;
        case ScreenId::Main: return &mMainScreen;
        case ScreenId::Debug: return &mDebugScreen;
        case ScreenId::EventLog: return &mEventLogScreen;
        case ScreenId::StateLog: return &mStateLogScreen;
        default: TriggerExit("Runtime", "screen with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

} // namespace Garbox