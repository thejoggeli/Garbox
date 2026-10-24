#include "simulation/SimulationRuntime.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"
#include "shared/types/EventAlias.h"

namespace Garbox {

namespace {

RuntimeAbs::Config MakeConfig(){
    return RuntimeAbs::Config {
#if defined(GARBOX_SIMULATION_UI)
        .numComponents = 2,
        .numStates = 7,
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
    registerComponent(&mSimpleScreen);
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

void SimulationRuntime::onStart(){}

void SimulationRuntime::onRun(const TaskStopToken&){}

void SimulationRuntime::onRouteStateChanged(const StateAbs& state){
    switch(state.type()){
    case StateType::FanSample:
        mFermentationBehaviour.onFanSampleStateChanged(mFanSample);
#if defined(GARBOX_SIMULATION_UI)
        mSimpleScreen.onFanSampleStateChanged(mFanSample);
#endif
        break;
    case StateType::FanStatus:
        mFermentationBehaviour.onFanStatusStateChanged(mFanStatus);
#if defined(GARBOX_SIMULATION_UI)
        mSimpleScreen.onFanStatusStateChanged(mFanStatus);
#endif
        break;
    case StateType::TemperatureSample:
        mFermentationBehaviour.onTemperatureSampleStateChanged(mTemperatureSample);
#if defined(GARBOX_SIMULATION_UI)
        mSimpleScreen.onTemperatureSampleStateChanged(mTemperatureSample);
#endif
        break;
    case StateType::TemperatureStatus:
        mFermentationBehaviour.onTemperatureStatusStateChanged(mTemperatureStatus);
#if defined(GARBOX_SIMULATION_UI)
        mSimpleScreen.onTemperatureStatusStateChanged(mTemperatureStatus);
#endif
        break;
    case StateType::FermentationStatus:
#if defined(GARBOX_SIMULATION_UI)
        mSimpleScreen.onFermentationStatusStateChanged(mFermentationStatus);
#endif
        break;
#if defined(GARBOX_SIMULATION_UI)
    case StateType::HeatpadStatus:
        mSimpleScreen.onHeatpadStatusStateChanged(mHeatpadStatus);
        break;
    case StateType::HeatpadSample:
        mSimpleScreen.onHeatpadSampleStateChanged(mHeatpadSample);
        break;
#endif
    default:
        TriggerExit("SimulationRuntime", "unexpected state type");
        break;
    }
}

void SimulationRuntime::onRouteEvent(const EventHeader* header){
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
    case EventType::Button:
        mSimpleScreen.onButtonEvent(ButtonEvent(header));
        break;
    case EventType::ButtonRepeat:
        mSimpleScreen.onButtonRepeatEvent(ButtonRepeatEvent(header));
        break;
    case EventType::EncoderStep:
        mSimpleScreen.onEncoderStepEvent(EncoderStepEvent(header));
        break;
    case EventType::ActiveScreenChanged:
        break;
#endif
    default:
        TriggerExit("SimulationRuntime", "unexpected event type");
        break;
    }
}

void SimulationRuntime::onActiveBehaviourChanged(){}

void SimulationRuntime::onActiveScreenChanged(){}

BehaviourAbs* SimulationRuntime::resolveBehaviour(BehaviourId id){
    return id == BehaviourId::Fermentation ? &mFermentationBehaviour : nullptr;
}

ControllerAbs* SimulationRuntime::resolveController(ControllerId){
    return nullptr;
}

ScreenAbs* SimulationRuntime::resolveScreen(ScreenId id){
#if defined(GARBOX_SIMULATION_UI)
    if(id == ScreenId::Simple){
        return &mSimpleScreen;
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
#endif
}

} // namespace Garbox
