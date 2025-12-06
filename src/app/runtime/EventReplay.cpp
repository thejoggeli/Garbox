// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventReplay.h"
#include "core/assert/Assert.h"

namespace Garbox {

static constexpr size_t MainScreen_DisplayCommand_Index = 0 ;
static constexpr size_t MainScreen_DisplayStatus_Index = 1 ;
static constexpr size_t MainScreen_FanCommand_Index = 2 ;
static constexpr size_t MainScreen_FanSample_Index = 3 ;
static constexpr size_t MainScreen_FanStatus_Index = 4 ;
static constexpr size_t MainScreen_FermentationStatus_Index = 5 ;
static constexpr size_t MainScreen_HeatpadCommand_Index = 6 ;
static constexpr size_t MainScreen_HeatpadSample_Index = 7 ;
static constexpr size_t MainScreen_HeatpadStatus_Index = 8 ;
static constexpr size_t MainScreen_TemperatureSample_Index = 9 ;
static constexpr size_t MainScreen_TemperatureStatus_Index = 10 ;
static constexpr size_t DebugScreen_ActiveBehaviourChanged_Index = 0 ;
static constexpr size_t DebugScreen_DisplayStatus_Index = 1 ;
static constexpr size_t DebugScreen_FanSample_Index = 2 ;
static constexpr size_t DebugScreen_FanStatus_Index = 3 ;
static constexpr size_t DebugScreen_FermentationStatus_Index = 4 ;
static constexpr size_t DebugScreen_HeatpadSample_Index = 5 ;
static constexpr size_t DebugScreen_HeatpadStatus_Index = 6 ;
static constexpr size_t DebugScreen_TemperatureSample_Index = 7 ;
static constexpr size_t DebugScreen_TemperatureStatus_Index = 8 ;

EventReplay::EventReplay(
    // parameter list
 
    MainScreenAbs& mainScreen,  
    DebugScreenAbs& debugScreen):
    // init members
 
    mMainScreen(mainScreen),  
    mDebugScreen(debugScreen){
    // register handlers
    mMainScreenDispatcher.registerHandler(sendDisplayCommandToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendDisplayStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanCommandToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFermentationStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadCommandToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendTemperatureSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendTemperatureStatusToMainScreen, this);
    mDebugScreenDispatcher.registerHandler(sendActiveBehaviourChangedToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendDisplayStatusToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendFanSampleToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendFanStatusToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendFermentationStatusToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendHeatpadSampleToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendHeatpadStatusToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendTemperatureSampleToDebugScreen, this);
    mDebugScreenDispatcher.registerHandler(sendTemperatureStatusToDebugScreen, this);
    // init event headers
    const ComponentDescriptor descriptor { ComponentType::Replay, ComponentId::Replay };
    mFermentationStatusBlock.header.type = EventType::FermentationStatus;
    mFermentationStatusBlock.header.sender = descriptor;
    mDisplayCommandBlock.header.type = EventType::DisplayCommand;
    mDisplayCommandBlock.header.sender = descriptor;
    mDisplayStatusBlock.header.type = EventType::DisplayStatus;
    mDisplayStatusBlock.header.sender = descriptor;
    mFanStatusBlock.header.type = EventType::FanStatus;
    mFanStatusBlock.header.sender = descriptor;
    mFanSampleBlock.header.type = EventType::FanSample;
    mFanSampleBlock.header.sender = descriptor;
    mFanCommandBlock.header.type = EventType::FanCommand;
    mFanCommandBlock.header.sender = descriptor;
    mHeatpadStatusBlock.header.type = EventType::HeatpadStatus;
    mHeatpadStatusBlock.header.sender = descriptor;
    mHeatpadSampleBlock.header.type = EventType::HeatpadSample;
    mHeatpadSampleBlock.header.sender = descriptor;
    mHeatpadCommandBlock.header.type = EventType::HeatpadCommand;
    mHeatpadCommandBlock.header.sender = descriptor;
    mTemperatureStatusBlock.header.type = EventType::TemperatureStatus;
    mTemperatureStatusBlock.header.sender = descriptor;
    mTemperatureSampleBlock.header.type = EventType::TemperatureSample;
    mTemperatureSampleBlock.header.sender = descriptor;
    mActiveBehaviourChangedBlock.header.type = EventType::ActiveBehaviourChanged;
    mActiveBehaviourChangedBlock.header.sender = descriptor;
}

void EventReplay::storeEvent(const EventHeader* header){
    switch(header->type){
        case EventType::FermentationStatus:
            mFermentationStatusBlock.header.id = header->id;
            mFermentationStatusBlock.payload = *static_cast<FermentationStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FermentationStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_FermentationStatus_Index);
            break;
        case EventType::DisplayCommand:
            mDisplayCommandBlock.header.id = header->id;
            mDisplayCommandBlock.payload = *static_cast<DisplayCommandEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_DisplayCommand_Index);
            break;
        case EventType::DisplayStatus:
            mDisplayStatusBlock.header.id = header->id;
            mDisplayStatusBlock.payload = *static_cast<DisplayStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_DisplayStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_DisplayStatus_Index);
            break;
        case EventType::FanStatus:
            mFanStatusBlock.header.id = header->id;
            mFanStatusBlock.payload = *static_cast<FanStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FanStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_FanStatus_Index);
            break;
        case EventType::FanSample:
            mFanSampleBlock.header.id = header->id;
            mFanSampleBlock.payload = *static_cast<FanSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FanSample_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_FanSample_Index);
            break;
        case EventType::FanCommand:
            mFanCommandBlock.header.id = header->id;
            mFanCommandBlock.payload = *static_cast<FanCommandEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FanCommand_Index);
            break;
        case EventType::HeatpadStatus:
            mHeatpadStatusBlock.header.id = header->id;
            mHeatpadStatusBlock.payload = *static_cast<HeatpadStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_HeatpadStatus_Index);
            break;
        case EventType::HeatpadSample:
            mHeatpadSampleBlock.header.id = header->id;
            mHeatpadSampleBlock.payload = *static_cast<HeatpadSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadSample_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_HeatpadSample_Index);
            break;
        case EventType::HeatpadCommand:
            mHeatpadCommandBlock.header.id = header->id;
            mHeatpadCommandBlock.payload = *static_cast<HeatpadCommandEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadCommand_Index);
            break;
        case EventType::TemperatureStatus:
            mTemperatureStatusBlock.header.id = header->id;
            mTemperatureStatusBlock.payload = *static_cast<TemperatureStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_TemperatureStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_TemperatureStatus_Index);
            break;
        case EventType::TemperatureSample:
            mTemperatureSampleBlock.header.id = header->id;
            mTemperatureSampleBlock.payload = *static_cast<TemperatureSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_TemperatureSample_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_TemperatureSample_Index);
            break;
        case EventType::ActiveBehaviourChanged:
            mActiveBehaviourChangedBlock.header.id = header->id;
            mActiveBehaviourChangedBlock.payload = *static_cast<ActiveBehaviourChangedEvent>(header).payload();
            mDebugScreenDispatcher.markDirty(DebugScreen_ActiveBehaviourChanged_Index);
            break;
        default: break; // no screen listens to event type
    }
}

void EventReplay::replay(ScreenId screenId){
    switch(screenId){
        case ScreenId::Main:
            mMainScreenDispatcher.dispatch();
            break;
        case ScreenId::Debug:
            mDebugScreenDispatcher.dispatch();
            break;
        default: break; // replay not enabled for this screen
    }
}

void EventReplay::sendDisplayCommandToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.onDisplayCommand(&self->mDisplayCommandBlock.header); 
}

void EventReplay::sendDisplayStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    DisplayStatusPayload& payload = self->mDisplayStatusBlock.payload; 
    model.setDisplayBrightness(payload.brightness); 
}

void EventReplay::sendFanCommandToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.onFanCommand(&self->mFanCommandBlock.header); 
}

void EventReplay::sendFanSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    FanSamplePayload& payload = self->mFanSampleBlock.payload; 
    model.setFanMeasuredRpm(payload.measuredRpm); 
}

void EventReplay::sendFanStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    FanStatusPayload& payload = self->mFanStatusBlock.payload; 
    model.setFanState(payload.state); 
    model.setFanTargetSpeed(payload.targetSpeed); 
}

void EventReplay::sendFermentationStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    FermentationStatusPayload& payload = self->mFermentationStatusBlock.payload; 
    model.setTargetTemperature(payload.targetTemperature); 
    model.setEngineState(payload.heaterEngineState); 
}

void EventReplay::sendHeatpadCommandToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.onHeatpadCommand(&self->mHeatpadCommandBlock.header); 
}

void EventReplay::sendHeatpadSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    HeatpadSamplePayload& payload = self->mHeatpadSampleBlock.payload; 
    model.setHeatpadMeasuredVoltage(payload.measuredVoltage); 
    model.setHeatpadMeasuredCurrent(payload.measuredCurrent); 
}

void EventReplay::sendHeatpadStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    HeatpadStatusPayload& payload = self->mHeatpadStatusBlock.payload; 
    model.setHeatpadState(payload.state); 
    model.setHeatpadCurrentDuty(payload.currentDutyCycle); 
}

void EventReplay::sendTemperatureSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    TemperatureSamplePayload& payload = self->mTemperatureSampleBlock.payload; 
    model.setMeasuredTemperature(payload.temperatureCelcius); 
    model.setMeasuredHumidity(payload.humidityRelative); 
}

void EventReplay::sendTemperatureStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    MainScreen::Model& model = self->mMainScreen.model();
    TemperatureStatusPayload& payload = self->mTemperatureStatusBlock.payload; 
    model.setShtDriverEnabled(payload.driverEnabled); 
    model.setShtPowerEnabled(payload.powerEnabled); 
    model.setShtResetting(payload.resetting); 
    model.setShtHasSample(payload.hasFirstSample); 
}

void EventReplay::sendActiveBehaviourChangedToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    ActiveBehaviourChangedPayload& payload = self->mActiveBehaviourChangedBlock.payload; 
    model.setBehaviour(payload.newBehaviour); 
}

void EventReplay::sendDisplayStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    DisplayStatusPayload& payload = self->mDisplayStatusBlock.payload; 
    model.setDisplayBrightness(payload.brightness); 
    model.setDisplaySkipped(payload.skipped); 
}

void EventReplay::sendFanSampleToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    FanSamplePayload& payload = self->mFanSampleBlock.payload; 
    model.setFanMeasuredRpm(payload.measuredRpm); 
}

void EventReplay::sendFanStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    FanStatusPayload& payload = self->mFanStatusBlock.payload; 
    model.setFanState(payload.state); 
    model.setFanTargetSpeed(payload.targetSpeed); 
}

void EventReplay::sendFermentationStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    FermentationStatusPayload& payload = self->mFermentationStatusBlock.payload; 
    model.setEngineState(payload.heaterEngineState); 
    model.setEngineTargetTemperature(payload.targetTemperature); 
    model.setEngineMeasuredTemperature(payload.measuredTemperature); 
    model.setEngineMeasuredHumidity(payload.measuredHumidity); 
}

void EventReplay::sendHeatpadSampleToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    HeatpadSamplePayload& payload = self->mHeatpadSampleBlock.payload; 
    model.setHeatpadPwmProgress(payload.pwmProgressMicros); 
    model.setHeatpadMeasuredVoltage(payload.measuredVoltage); 
    model.setHeatpadMeasuredCurrent(payload.measuredCurrent); 
}

void EventReplay::sendHeatpadStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    HeatpadStatusPayload& payload = self->mHeatpadStatusBlock.payload; 
    model.setHeatpadState(payload.state); 
    model.setHeatpadCurrentDuty(payload.currentDutyCycle); 
    model.setHeatpadCurrentPeriod(payload.currentPeriodMicros); 
    model.setHeatpadNextDuty(payload.nextDutyCycle); 
    model.setHeatpadNextPeriod(payload.nextPeriodMicros); 
}

void EventReplay::sendTemperatureSampleToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    TemperatureSamplePayload& payload = self->mTemperatureSampleBlock.payload; 
    model.setSensorTemperatureCelcius(payload.temperatureCelcius); 
    model.setSensorHumidityRelative(payload.humidityRelative); 
}

void EventReplay::sendTemperatureStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    DebugScreen::Model& model = self->mDebugScreen.model();
    TemperatureStatusPayload& payload = self->mTemperatureStatusBlock.payload; 
    model.setShtDriverEnabled(payload.driverEnabled); 
    model.setShtPowerEnabled(payload.powerEnabled); 
    model.setShtResetting(payload.resetting); 
}

} // namespace