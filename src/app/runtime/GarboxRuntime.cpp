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
    RuntimeAbs(RuntimeAbs::Config {
        .eventPoolSizeBytes = 1024,
        .eventQueueLength = 128,
    }),
    mTickRunner(TickHandlersCount, TickPeriodMillis),
    mEventReplay(mMainScreen, mDebugScreen){

    // register components
    registerControllers({ 
        &mDisplayController,  
        &mDevtoolsController,  
        &mFanController,  
        &mHeartbeatController,  
        &mHeatpadController,  
        &mInputController,  
        &mI2cPartsController 
    });
    registerBehaviours({ 
        &mCalibrationBehaviour,  
        &mFermentationBehaviour 
    });
    registerScreens({ 
        &mMainScreen,  
        &mDebugScreen 
    });

    // setup routing
    setupTickRouting();
    setupEventRouting();

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
    if(mActiveScreen){
        mEventReplay.replay(mActiveScreen->getScreenId());
    }
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
    mControllers.routeTick(TickPhase::Heartbeat);
    mBehaviours.routeTick(TickPhase::Heartbeat);
    mScreens.routeTick(TickPhase::Heartbeat);
    dispatchEvents();
}

void GarboxRuntime::handleInputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::InputTick);
    mControllers.routeTick(TickPhase::Input);
    mBehaviours.routeTick(TickPhase::Input);
    mScreens.routeTick(TickPhase::Input);
    dispatchEvents();
}

void GarboxRuntime::handleLogicTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LogicTick);
    mControllers.routeTick(TickPhase::Logic);
    mBehaviours.routeTick(TickPhase::Logic);
    mScreens.routeTick(TickPhase::Logic);
    dispatchEvents();
}

void GarboxRuntime::handleOutputTick(){
    Profiler::MeasureScoped profiler(ProfilerId::OutputTick);
    mControllers.routeTick(TickPhase::Output);
    mBehaviours.routeTick(TickPhase::Output);
    mScreens.routeTick(TickPhase::Output);
    dispatchEvents();
}

void GarboxRuntime::handleLoggingTick(){
    Profiler::MeasureScoped profiler(ProfilerId::LoggingTick);
    mControllers.routeTick(TickPhase::Logging);
    mBehaviours.routeTick(TickPhase::Logging);
    mScreens.routeTick(TickPhase::Logging);
    dispatchEvents();
}

void GarboxRuntime::handleRenderTick(){
    Profiler::MeasureScoped profiler(ProfilerId::RenderTick);
    mControllers.routeTick(TickPhase::Render);
    mBehaviours.routeTick(TickPhase::Render);
    mScreens.routeTick(TickPhase::Render);
    dispatchEvents();
}

void GarboxRuntime::onRouteEvent(const EventHeader* header){
    
    // store event for replay
    mEventReplay.storeEvent(header);

    // route event to components
    mControllers.routeEvent(header);
    mBehaviours.routeEvent(header);
    mScreens.routeEvent(header);
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

void GarboxRuntime::setupTickRouting(){
    mControllers.setRouteTick(&mDisplayController, TickPhase::Render, true);
    mControllers.setRouteTick(&mDevtoolsController, TickPhase::Logging, true);
    mControllers.setRouteTick(&mFanController, TickPhase::Input, true);
    mControllers.setRouteTick(&mFanController, TickPhase::Output, true);
    mControllers.setRouteTick(&mHeartbeatController, TickPhase::Heartbeat, true);
    mControllers.setRouteTick(&mHeatpadController, TickPhase::Input, true);
    mControllers.setRouteTick(&mHeatpadController, TickPhase::Output, true);
    mControllers.setRouteTick(&mInputController, TickPhase::Input, true);
    mControllers.setRouteTick(&mI2cPartsController, TickPhase::Input, true);
    mBehaviours.setRouteTick(&mCalibrationBehaviour, TickPhase::Logic, true);
    mBehaviours.setRouteTick(&mFermentationBehaviour, TickPhase::Logic, true);
}

void GarboxRuntime::setupEventRouting(){
    mControllers.setRouteEvent(&mDisplayController, EventType::DisplayCommand, true);
    mControllers.setRouteEvent(&mFanController, EventType::FanCommand, true);
    mControllers.setRouteEvent(&mHeatpadController, EventType::HeatpadCommand, true);
    mControllers.setRouteEvent(&mI2cPartsController, EventType::ButtonStateChanged, true);
    mBehaviours.setRouteEvent(&mCalibrationBehaviour, EventType::Heartbeat, true);
    mBehaviours.setRouteEvent(&mCalibrationBehaviour, EventType::FanStatus, true);
    mBehaviours.setRouteEvent(&mCalibrationBehaviour, EventType::FanSample, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::Heartbeat, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::FanStatus, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::FanSample, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::HeatpadStatus, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::TemperatureStatus, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::TemperatureSample, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::ButtonStateChanged, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::ButtonRepeat, true);
    mBehaviours.setRouteEvent(&mFermentationBehaviour, EventType::EncoderStep, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::FanStatus, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::FanSample, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::HeatpadStatus, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::HeatpadSample, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::TemperatureStatus, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::TemperatureSample, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::ActiveBehaviourChanged, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::FermentationStatus, true);
    mScreens.setRouteEvent(&mMainScreen, EventType::DisplayStatus, true);
    mScreens.setRouteEvent(&mDebugScreen, EventType::Heartbeat, true);
}

} // namespace Garbox