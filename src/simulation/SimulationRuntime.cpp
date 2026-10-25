#include "simulation/SimulationRuntime.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"
#include "shared/types/EventAlias.h"

#if defined(GARBOX_SIMULATION_UI)
#include "app/services/GarboxHistory.h"
#endif

namespace Garbox {

namespace {

RuntimeAbs::Config MakeConfig(){
    return RuntimeAbs::Config {
#if defined(GARBOX_SIMULATION_UI)
        .numComponents = 6,
        .numStates = 12,
#else
        .numComponents = 1,
        .numStates = 5,
#endif
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 32,
        .maxDispatchRecursionDepth = 4,
    };
}

constexpr ComponentDescriptor SimulationSender {
    ComponentType::Runtime,
    ComponentId::Runtime,
};

} // namespace

SimulationRuntime::SimulationRuntime(): RuntimeAbs(MakeConfig()){
    registerState(&mFermentationStatus);
    registerState(&mFanSample);
    registerState(&mFanStatus);
    registerState(&mTemperatureSample);
    registerState(&mTemperatureStatus);
    registerComponent(&mFermentationBehaviour);

#if defined(GARBOX_SIMULATION_UI)
    registerState(&mHeatpadStatus);
    registerState(&mHeatpadSample);
    registerState(&mHeatpadProgress);
    registerState(&mDisplayStatus);
    registerState(&mDisplayDiagnostics);
    registerState(&mActiveBehaviourState);
    registerState(&mActiveScreenState);
    registerComponent(&mSimpleScreen);
    registerComponent(&mMainScreen);
    registerComponent(&mDebugScreen);
    registerComponent(&mEventLogScreen);
    registerComponent(&mStateLogScreen);
#endif

    mFermentationBehaviour.mStates.emplace(
        mFermentationStatus,
        mFanSample,
        mFanStatus,
        mTemperatureSample,
        mTemperatureStatus
    );

#if defined(GARBOX_SIMULATION_UI)
    mSimpleScreen.mStates.emplace(
        mFanStatus,
        mFanSample,
        mHeatpadStatus,
        mHeatpadSample,
        mTemperatureStatus,
        mTemperatureSample,
        mFermentationStatus
    );
    mMainScreen.mStates.emplace(
        mFanStatus,
        mFanSample,
        mHeatpadStatus,
        mHeatpadSample,
        mTemperatureStatus,
        mTemperatureSample,
        mFermentationStatus
    );
    mDebugScreen.mStates.emplace(
        mFanStatus,
        mFanSample,
        mHeatpadStatus,
        mHeatpadSample,
        mHeatpadProgress,
        mDisplayStatus,
        mDisplayDiagnostics,
        mTemperatureStatus,
        mTemperatureSample,
        mActiveBehaviourState,
        mActiveScreenState,
        mFermentationStatus
    );
#endif
}

void SimulationRuntime::boot(){
    AssertExit(!mBooted, "SimulationRuntime", "already booted");
    Time::Start();
    init();
    dispatch();
    start();
    dispatch();
    mBooted = true;
}

void SimulationRuntime::step(){
    AssertExit(mBooted, "SimulationRuntime", "not booted");
    Time::Tick();
    publishInputs();
    dispatch();
    mFermentationBehaviour.onLogicTick();
    dispatch();
    mContext.tickCount++;
}

void SimulationRuntime::setInputs(const Inputs& inputs){
    mInputs = inputs;
}

const SimulationRuntime::Inputs& SimulationRuntime::inputs() const {
    return mInputs;
}

const SimulationRuntime::Outputs& SimulationRuntime::outputs() const {
    return mOutputs;
}

void SimulationRuntime::requestFermentationEnabled(bool enabled){
    RequestFermentationModeEvent event =
        getEventFactory().make<EventType::RequestFermentationMode>(SimulationSender);
    if(!event){
        return;
    }
    event->enabled = enabled;
    receiveEvent(event.header());
    dispatch();
}

void SimulationRuntime::requestTargetTemperature(float temperatureCelcius){
    RequestTargetTemperatureEvent event =
        getEventFactory().make<EventType::RequestTargetTemperature>(SimulationSender);
    if(!event){
        return;
    }
    event->targetTemperature = temperatureCelcius;
    receiveEvent(event.header());
    dispatch();
}

void SimulationRuntime::requestAutomaticFan(bool enabled){
    RequestFanModeEvent event =
        getEventFactory().make<EventType::RequestFanMode>(SimulationSender);
    if(!event){
        return;
    }
    event->autoFan = enabled;
    receiveEvent(event.header());
    dispatch();
}

const FermentationStatusState& SimulationRuntime::fermentationStatus() const {
    return mFermentationStatus;
}

const TemperatureStatusState& SimulationRuntime::temperatureStatus() const {
    return mTemperatureStatus;
}

const TemperatureSampleState& SimulationRuntime::temperatureSample() const {
    return mTemperatureSample;
}

uint32_t SimulationRuntime::tickCount() const {
    return mContext.tickCount;
}

#if defined(GARBOX_SIMULATION_UI)
void SimulationRuntime::render(){
    requestRenderScreenNow();
}

void SimulationRuntime::injectButton(ButtonState oldState, ButtonState newState){
    ButtonEvent event = getEventFactory().make<EventType::Button>(SimulationSender);
    if(!event){
        return;
    }
    event->oldState = oldState;
    event->newState = newState;
    receiveEvent(event.header());
    dispatch();
}

void SimulationRuntime::injectEncoderSteps(int32_t steps){
    EncoderStepEvent event = getEventFactory().make<EventType::EncoderStep>(SimulationSender);
    if(!event){
        return;
    }
    event->steps = steps;
    receiveEvent(event.header());
    dispatch();
}
#endif

void SimulationRuntime::onInit(){
    setQueuedBehaviour(&mFermentationBehaviour);
#if defined(GARBOX_SIMULATION_UI)
    setQueuedScreen(&mSimpleScreen);
#endif
}

void SimulationRuntime::onStart(){
#if defined(GARBOX_SIMULATION_UI)
    GarboxHistory::Instance().reset();
#endif
}

void SimulationRuntime::onRun(const TaskStopToken&){}

void SimulationRuntime::onRouteStateChanged(const StateAbs& state){
#if defined(GARBOX_SIMULATION_UI)
    mStateLogScreen.onStateChanged(state);
#endif
    switch(state.type()){
    case StateType::FanSample:
        mFermentationBehaviour.onFanSampleStateChanged(mFanSample);
#if defined(GARBOX_SIMULATION_UI)
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onFanSampleStateChanged(mFanSample); break;
        case ScreenId::Main: mMainScreen.onFanSampleStateChanged(mFanSample); break;
        case ScreenId::Debug: mDebugScreen.onFanSampleStateChanged(mFanSample); break;
        default: break;
        }
#endif
        break;
    case StateType::FanStatus:
        mFermentationBehaviour.onFanStatusStateChanged(mFanStatus);
#if defined(GARBOX_SIMULATION_UI)
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onFanStatusStateChanged(mFanStatus); break;
        case ScreenId::Main: mMainScreen.onFanStatusStateChanged(mFanStatus); break;
        case ScreenId::Debug: mDebugScreen.onFanStatusStateChanged(mFanStatus); break;
        default: break;
        }
#endif
        break;
    case StateType::TemperatureSample:
        mFermentationBehaviour.onTemperatureSampleStateChanged(mTemperatureSample);
#if defined(GARBOX_SIMULATION_UI)
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onTemperatureSampleStateChanged(mTemperatureSample); break;
        case ScreenId::Main: mMainScreen.onTemperatureSampleStateChanged(mTemperatureSample); break;
        case ScreenId::Debug: mDebugScreen.onTemperatureSampleStateChanged(mTemperatureSample); break;
        default: break;
        }
#endif
        break;
    case StateType::TemperatureStatus:
        mFermentationBehaviour.onTemperatureStatusStateChanged(mTemperatureStatus);
#if defined(GARBOX_SIMULATION_UI)
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onTemperatureStatusStateChanged(mTemperatureStatus); break;
        case ScreenId::Main: mMainScreen.onTemperatureStatusStateChanged(mTemperatureStatus); break;
        case ScreenId::Debug: mDebugScreen.onTemperatureStatusStateChanged(mTemperatureStatus); break;
        default: break;
        }
#endif
        break;
    case StateType::FermentationStatus:
#if defined(GARBOX_SIMULATION_UI)
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onFermentationStatusStateChanged(mFermentationStatus); break;
        case ScreenId::Main: mMainScreen.onFermentationStatusStateChanged(mFermentationStatus); break;
        case ScreenId::Debug: mDebugScreen.onFermentationStatusStateChanged(mFermentationStatus); break;
        default: break;
        }
#endif
        break;
#if defined(GARBOX_SIMULATION_UI)
    case StateType::HeatpadStatus:
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onHeatpadStatusStateChanged(mHeatpadStatus); break;
        case ScreenId::Main: mMainScreen.onHeatpadStatusStateChanged(mHeatpadStatus); break;
        case ScreenId::Debug: mDebugScreen.onHeatpadStatusStateChanged(mHeatpadStatus); break;
        default: break;
        }
        break;
    case StateType::HeatpadSample:
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onHeatpadSampleStateChanged(mHeatpadSample); break;
        case ScreenId::Main: mMainScreen.onHeatpadSampleStateChanged(mHeatpadSample); break;
        case ScreenId::Debug: mDebugScreen.onHeatpadSampleStateChanged(mHeatpadSample); break;
        default: break;
        }
        break;
    case StateType::HeatpadProgress:
        if(getActiveScreenId() == ScreenId::Debug){
            mDebugScreen.onHeatpadProgressStateChanged(mHeatpadProgress);
        }
        break;
    case StateType::DisplayStatus:
        if(getActiveScreenId() == ScreenId::Debug){
            mDebugScreen.onDisplayStatusStateChanged(mDisplayStatus);
        }
        break;
    case StateType::DisplayDiagnostics:
        if(getActiveScreenId() == ScreenId::Debug){
            mDebugScreen.onDisplayDiagnosticsStateChanged(mDisplayDiagnostics);
        }
        break;
    case StateType::ActiveBehaviour:
        if(getActiveScreenId() == ScreenId::Debug){
            mDebugScreen.onActiveBehaviourStateChanged(mActiveBehaviourState);
        }
        break;
    case StateType::ActiveScreen:
        if(getActiveScreenId() == ScreenId::Debug){
            mDebugScreen.onActiveScreenStateChanged(mActiveScreenState);
        }
        break;
#endif
    default:
        TriggerExit("SimulationRuntime", "unexpected state type");
        break;
    }
}

void SimulationRuntime::onRouteEvent(const EventHeader* header){
#if defined(GARBOX_SIMULATION_UI)
    mEventLogScreen.onEvent(header);
#endif
    switch(header->type){
    case EventType::DisplayCommand: {
        const DisplayCommandEvent event(header);
        mOutputs.display = *event.payload();
        mOutputs.displayCommandCount++;
        break;
    }
    case EventType::FanCommand: {
        const FanCommandEvent event(header);
        mOutputs.fan = *event.payload();
        mOutputs.fanCommandCount++;
        break;
    }
    case EventType::HeatpadCommand: {
        const HeatpadCommandEvent event(header);
        mOutputs.heatpad = *event.payload();
        mOutputs.heatpadCommandCount++;
        break;
    }
    case EventType::RequestFermentationMode:
        mFermentationBehaviour.onRequestFermentationModeEvent(
            RequestFermentationModeEvent(header));
        break;
    case EventType::RequestTargetTemperature:
        mFermentationBehaviour.onRequestTargetTemperatureEvent(
            RequestTargetTemperatureEvent(header));
        break;
    case EventType::RequestFanMode:
        mFermentationBehaviour.onRequestFanModeEvent(RequestFanModeEvent(header));
        break;
    case EventType::Heartbeat:
        mFermentationBehaviour.onHeartbeatEvent(HeartbeatEvent(header));
        break;
    case EventType::ActiveBehaviourChanged:
        break;
#if defined(GARBOX_SIMULATION_UI)
    case EventType::Button: {
        const ButtonEvent event(header);
        switch(getActiveScreenId()){
        case ScreenId::Simple: mSimpleScreen.onButtonEvent(event); break;
        case ScreenId::Main: mMainScreen.onButtonEvent(event); break;
        case ScreenId::Debug: mDebugScreen.onButtonEvent(event); break;
        case ScreenId::EventLog: mEventLogScreen.onButtonEvent(event); break;
        case ScreenId::StateLog: mStateLogScreen.onButtonEvent(event); break;
        default: break;
        }
        break;
    }
    case EventType::ButtonRepeat:
        if(getActiveScreenId() == ScreenId::Simple){
            mSimpleScreen.onButtonRepeatEvent(ButtonRepeatEvent(header));
        }
        break;
    case EventType::EncoderStep: {
        const EncoderStepEvent event(header);
        if(getActiveScreenId() == ScreenId::Simple){
            mSimpleScreen.onEncoderStepEvent(event);
        }
        else if(getActiveScreenId() == ScreenId::Main){
            mMainScreen.onEncoderStepEvent(event);
        }
        break;
    }
    case EventType::ActiveScreenChanged:
        break;
#endif
    default:
        TriggerExit("SimulationRuntime", "unexpected event type");
        break;
    }
}

void SimulationRuntime::onActiveBehaviourChanged(){
#if defined(GARBOX_SIMULATION_UI)
    mActiveBehaviourState.setBehaviour(getActiveBehaviourId());
#endif
}

void SimulationRuntime::onActiveScreenChanged(){
#if defined(GARBOX_SIMULATION_UI)
    mActiveScreenState.setScreen(getActiveScreenId());
#endif
}

BehaviourAbs* SimulationRuntime::resolveBehaviour(BehaviourId id){
    return id == BehaviourId::Fermentation ? &mFermentationBehaviour : nullptr;
}

ControllerAbs* SimulationRuntime::resolveController(ControllerId){
    return nullptr;
}

ScreenAbs* SimulationRuntime::resolveScreen(ScreenId id){
#if defined(GARBOX_SIMULATION_UI)
    switch(id){
    case ScreenId::Simple: return &mSimpleScreen;
    case ScreenId::Main: return &mMainScreen;
    case ScreenId::Debug: return &mDebugScreen;
    case ScreenId::EventLog: return &mEventLogScreen;
    case ScreenId::StateLog: return &mStateLogScreen;
    default: break;
    }
#else
    (void)id;
#endif
    return nullptr;
}

void SimulationRuntime::publishInputs(){
    mTemperatureStatus.setPowerEnabled(mInputs.sensorAvailable);
    mTemperatureStatus.setDriverEnabled(mInputs.sensorAvailable);
    mTemperatureStatus.setResetting(false);
    mTemperatureStatus.setHasFirstSample(mInputs.sensorAvailable);
    mTemperatureStatus.setIsRunning(mInputs.sensorAvailable);

    mTemperatureSample.setTemperatureCelcius(mInputs.temperatureCelcius);
    mTemperatureSample.setHumidityRelative(mInputs.humidityRelative);

    mFanStatus.setState(mInputs.fanStalled ? FanState::Stalled : FanState::Enabled);
    mFanStatus.setTargetSpeed(mOutputs.fan.targetSpeed);
    mFanStatus.setRpmControl(mOutputs.fan.enableRpmControl);
    mFanSample.setMeasuredRpm(mInputs.fanMeasuredRpm);

#if defined(GARBOX_SIMULATION_UI)
    mHeatpadStatus.setState(mOutputs.heatpad.enabled
        ? HeatpadState::Heating
        : HeatpadState::Disabled);
    mHeatpadStatus.setCurrentDutyCycle(mOutputs.heatpad.dutyCycle);
    mHeatpadStatus.setNextDutyCycle(mOutputs.heatpad.dutyCycle);
    mHeatpadStatus.setCurrentPeriodMicros(mOutputs.heatpad.periodMicros);
    mHeatpadStatus.setNextPeriodMicros(mOutputs.heatpad.periodMicros);
    mHeatpadSample.setMeasuredVoltage(mOutputs.heatpad.enabled
        ? 17.0f * mOutputs.heatpad.dutyCycle
        : 0.0f);
    mHeatpadSample.setMeasuredCurrent(mOutputs.heatpad.enabled
        ? 1.0f * mOutputs.heatpad.dutyCycle
        : 0.0f);
    const uint32_t period = mOutputs.heatpad.periodMicros;
    mHeatpadProgress.setPwmProgressMicros(period > 0
        ? static_cast<float>(Time::GetTickMicros() % period)
        : 0.0f);
    mDisplayStatus.setBrightness(mOutputs.display.brightness);
    mDisplayDiagnostics.setSkippedFrames(0);

    GarboxHistory& history = GarboxHistory::Instance();
    history.measuredTemperatureSample(
        static_cast<int32_t>(mInputs.temperatureCelcius * GarboxHistory::TemperaturScaleFactor));
    history.targetTemperatureSample(
        static_cast<int32_t>(mFermentationStatus.getTargetTemperature()
            * GarboxHistory::TemperaturScaleFactor));
    history.powerSample(
        static_cast<int32_t>(mOutputs.heatpad.dutyCycle * 100.0f
            * GarboxHistory::PowerScaleFactor));
#endif
}

} // namespace Garbox
