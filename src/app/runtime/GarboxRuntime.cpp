// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "GarboxRuntime.h"

#include "app/runtime/SnapshotRegistry.h"
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
    RuntimeAbs(RuntimeAbs::Config {
        .numComponents = 12,
        .numStates = 11,
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 128,
        .maxDispatchRecursionDepth = 3
    }),
    mTickRunner(TickHandlersCount, TickPeriodMillis),
    mStateRegistry(){

    // register states
    registerState(&mStateRegistry.getDisplayStatus());
    registerState(&mStateRegistry.getDisplayDiagnostics());
    registerState(&mStateRegistry.getFanStatus());
    registerState(&mStateRegistry.getFanSample());
    registerState(&mStateRegistry.getFermentationStatus());
    registerState(&mStateRegistry.getHeatpadStatus());
    registerState(&mStateRegistry.getHeatpadSample());
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
    registerComponent(&mMainScreen);
    registerComponent(&mDebugScreen);
    registerComponent(&mEventLogScreen);

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

    // bind 'CalibrationBehaviour' states
    mCalibrationBehaviour.bindStates(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample()
    );

    // bind 'FermentationBehaviour' states
    mFermentationBehaviour.bindStates(
        mStateRegistry.getFermentationStatus(),
        mStateRegistry.getFanSample(),
        mStateRegistry.getFanStatus(),
        mStateRegistry.getTemperatureSample(),
        mStateRegistry.getTemperatureStatus()
    );

    // bind 'DisplayController' states
    mDisplayController.bindStates(
        mStateRegistry.getDisplayStatus(),
        mStateRegistry.getDisplayDiagnostics()
    );

    // bind 'FanController' states
    mFanController.bindStates(
        mStateRegistry.getFanStatus(),
        mStateRegistry.getFanSample()
    );

    // bind 'HeatpadController' states
    mHeatpadController.bindStates(
        mStateRegistry.getHeatpadStatus(),
        mStateRegistry.getHeatpadSample()
    );

    // bind 'I2cPartsController' states
    mI2cPartsController.bindStates(
        mStateRegistry.getTemperatureStatus(),
        mStateRegistry.getTemperatureSample()
    );

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
    mStateRegistry.getActiveBehaviour().setBehaviour(mActiveBehaviour->getBehaviourId());
}

void GarboxRuntime::onActiveScreenChanged(){
    mStateRegistry.getActiveScreen().setScreen(mActiveScreen->getScreenId());
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
    dispatch();
}

void GarboxRuntime::handleInputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::InputTick);
    mFanController.onInputTick();
    mHeatpadController.onInputTick();
    mInputController.onInputTick();
    mI2cPartsController.onInputTick();
    dispatch();
}

void GarboxRuntime::handleLogicTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LogicTick);
    switch(mActiveBehaviour->getBehaviourId()){
        case BehaviourId::Calibration:
            static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onLogicTick();
            break;
        case BehaviourId::Fermentation:
            static_cast<FermentationBehaviour*>(mActiveBehaviour)->onLogicTick();
            break;
        default: break; // active behaviour does not receive tick type
    }
    dispatch();
}

void GarboxRuntime::handleOutputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::OutputTick);
    mFanController.onOutputTick();
    mHeatpadController.onOutputTick();
    dispatch();
}

void GarboxRuntime::handleLoggingTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LoggingTick);
    mDevtoolsController.onLoggingTick();
    dispatch();
}

void GarboxRuntime::handleRenderTick(){
    Profiler::MeasureScoped profiler(ProfilerId::RenderTick);
    mDisplayController.onRenderTick();
    dispatch();
}

void GarboxRuntime::onRouteStateChanged(const StateAbs& state){
    
    switch(state.type()){
    case StateType::DisplayStatus: break;
    case StateType::DisplayDiagnostics: break;
    case StateType::FanStatus: {
        const FanStatusState& fanStatus = static_cast<const FanStatusState&>(state);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatusStateChanged(fanStatus); break;
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onFanStatusStateChanged(fanStatus); break;
            default: break; // active behaviour does not read state
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
        break;
    }
    case StateType::FermentationStatus: break;
    case StateType::HeatpadStatus: break;
    case StateType::HeatpadSample: break;
    case StateType::TemperatureStatus: {
        const TemperatureStatusState& temperatureStatus = static_cast<const TemperatureStatusState&>(state);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureStatusStateChanged(temperatureStatus); break;
            default: break; // active behaviour does not read state
        }
        break;
    }
    case StateType::TemperatureSample: {
        const TemperatureSampleState& temperatureSample = static_cast<const TemperatureSampleState&>(state);
        switch(mActiveBehaviour->getBehaviourId()){
            case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onTemperatureSampleStateChanged(temperatureSample); break;
            default: break; // active behaviour does not read state
        }
        break;
    }
    case StateType::ActiveBehaviour: break;
    case StateType::ActiveScreen: break;
    case StateType::Null:
    case StateType::Count:
        TriggerDebug("GarboxRuntime", "invalid state type");
        break;
    }
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){

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
    case EventType::ButtonStateChanged: {
        const ButtonStateChangedEvent event(header);
        mI2cPartsController.onButtonStateChangedEvent(event);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonStateChangedEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonStateChangedEvent(event); break;
                default: break; // active behaviour does not receive 'ButtonStateChanged' event
            }
        }
        break;
    }
    case EventType::ButtonRepeat: {
        const ButtonRepeatEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onButtonRepeatEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onButtonRepeatEvent(event); break;
                default: break; // active behaviour does not receive 'ButtonRepeat' event
            }
        }
        break;
    }
    case EventType::EncoderStep: {
        const EncoderStepEvent event(header);
        if(header->sendToInactiveComponents){
            mFermentationBehaviour.onEncoderStepEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Fermentation: static_cast<FermentationBehaviour*>(mActiveBehaviour)->onEncoderStepEvent(event); break;
                default: break; // active behaviour does not receive 'EncoderStep' event
            }
        }
        break;
    }
    case EventType::FermentationStatus: {
        const FermentationStatusEvent event(header);
        SnapshotRegistry::UpdateFermentationStatus(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setEngineState(event->fermentationState);
                model.setEngineTargetTemperature(event->targetTemperature);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setTargetTemperature(event->targetTemperature);
                model.setEngineState(event->fermentationState);
                break;
            }
            default: break; // active screen has no model binding to any field of 'FermentationStatus' event
        }
        break;
    }
    case EventType::DisplayStatus: {
        const DisplayStatusEvent event(header);
        SnapshotRegistry::UpdateDisplayStatus(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setDisplayBrightness(event->brightness);
                model.setDisplaySkipped(event->skipped);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setDisplayBrightness(event->brightness);
                break;
            }
            default: break; // active screen has no model binding to any field of 'DisplayStatus' event
        }
        break;
    }
    case EventType::FanStatus: {
        const FanStatusEvent event(header);
        SnapshotRegistry::UpdateFanStatus(*event.payload());
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onFanStatusEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanStatusEvent(event); break;
                default: break; // active behaviour does not receive 'FanStatus' event
            }
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setFanState(event->state);
                model.setFanTargetSpeed(event->targetSpeed);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setFanState(event->state);
                model.setFanTargetSpeed(event->targetSpeed);
                break;
            }
            default: break; // active screen has no model binding to any field of 'FanStatus' event
        }
        break;
    }
    case EventType::FanSample: {
        const FanSampleEvent event(header);
        SnapshotRegistry::UpdateFanSample(*event.payload());
        if(header->sendToInactiveComponents){
            mCalibrationBehaviour.onFanSampleEvent(event);
        }
        else {
            switch(mActiveBehaviour->getBehaviourId()){
                case BehaviourId::Calibration: static_cast<CalibrationBehaviour*>(mActiveBehaviour)->onFanSampleEvent(event); break;
                default: break; // active behaviour does not receive 'FanSample' event
            }
        }
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setFanMeasuredRpm(event->measuredRpm);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setFanMeasuredRpm(event->measuredRpm);
                break;
            }
            default: break; // active screen has no model binding to any field of 'FanSample' event
        }
        break;
    }
    case EventType::HeatpadStatus: {
        const HeatpadStatusEvent event(header);
        SnapshotRegistry::UpdateHeatpadStatus(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setHeatpadState(event->state);
                model.setHeatpadCurrentDuty(event->currentDutyCycle);
                model.setHeatpadCurrentPeriod(event->currentPeriodMicros);
                model.setHeatpadNextDuty(event->nextDutyCycle);
                model.setHeatpadNextPeriod(event->nextPeriodMicros);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setHeatpadState(event->state);
                model.setHeatpadNextDuty(event->nextDutyCycle);
                break;
            }
            default: break; // active screen has no model binding to any field of 'HeatpadStatus' event
        }
        break;
    }
    case EventType::HeatpadSample: {
        const HeatpadSampleEvent event(header);
        SnapshotRegistry::UpdateHeatpadSample(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setHeatpadPwmProgress(event->pwmProgressMicros);
                model.setHeatpadMeasuredVoltage(event->measuredVoltage);
                model.setHeatpadMeasuredCurrent(event->measuredCurrent);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setHeatpadMeasuredVoltage(event->measuredVoltage);
                model.setHeatpadMeasuredCurrent(event->measuredCurrent);
                break;
            }
            default: break; // active screen has no model binding to any field of 'HeatpadSample' event
        }
        break;
    }
    case EventType::TemperatureStatus: {
        const TemperatureStatusEvent event(header);
        SnapshotRegistry::UpdateTemperatureStatus(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setShtDriverEnabled(event->driverEnabled);
                model.setShtPowerEnabled(event->powerEnabled);
                model.setShtResetting(event->resetting);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setShtDriverEnabled(event->driverEnabled);
                model.setShtPowerEnabled(event->powerEnabled);
                model.setShtResetting(event->resetting);
                model.setShtHasSample(event->hasFirstSample);
                break;
            }
            default: break; // active screen has no model binding to any field of 'TemperatureStatus' event
        }
        break;
    }
    case EventType::TemperatureSample: {
        const TemperatureSampleEvent event(header);
        SnapshotRegistry::UpdateTemperatureSample(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setSensorTemperatureCelcius(event->temperatureCelcius);
                model.setSensorHumidityRelative(event->humidityRelative);
                break;
            }
            case ScreenId::Main: {
                MainScreen::Model& model = static_cast<MainScreen*>(mActiveScreen)->model();
                model.setMeasuredTemperature(event->temperatureCelcius);
                model.setMeasuredHumidity(event->humidityRelative);
                break;
            }
            default: break; // active screen has no model binding to any field of 'TemperatureSample' event
        }
        break;
    }
    case EventType::ActiveBehaviourChanged: {
        const ActiveBehaviourChangedEvent event(header);
        SnapshotRegistry::UpdateActiveBehaviourChanged(*event.payload());
        switch(mActiveScreen->getScreenId()){
            case ScreenId::Debug: {
                DebugScreen::Model& model = static_cast<DebugScreen*>(mActiveScreen)->model();
                model.setBehaviour(event->newBehaviour);
                break;
            }
            default: break; // active screen has no model binding to any field of 'ActiveBehaviourChanged' event
        }
        break;
    }
    case EventType::ActiveScreenChanged: {
        const ActiveScreenChangedEvent event(header);
        SnapshotRegistry::UpdateActiveScreenChanged(*event.payload());
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
        case ScreenId::EventLog: return &mEventLogScreen;
        default: TriggerExit("GarboxRuntime", "screen with id not found", static_cast<uint32_t>(id));
    }
    return nullptr;
}

} // namespace Garbox