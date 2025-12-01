// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventReplay.h"
#include "core/assert/Assert.h"

namespace Garbox {

static constexpr size_t MainScreen_ActiveBehaviourChanged_Index = 0 ;
static constexpr size_t MainScreen_DisplayStatus_Index = 1 ;
static constexpr size_t MainScreen_FanSample_Index = 2 ;
static constexpr size_t MainScreen_FanStatus_Index = 3 ;
static constexpr size_t MainScreen_FermentationStatus_Index = 4 ;
static constexpr size_t MainScreen_HeatpadSample_Index = 5 ;
static constexpr size_t MainScreen_HeatpadStatus_Index = 6 ;
static constexpr size_t MainScreen_TemperatureSample_Index = 7 ;
static constexpr size_t MainScreen_TemperatureStatus_Index = 8 ;
static constexpr size_t DebugScreen_Heartbeat_Index = 0 ;

EventReplay::EventReplay(
    // parameter list
 
    MainScreenAbs& mainScreen,  
    DebugScreenAbs& debugScreen):
    // init members
 
    mMainScreen(mainScreen),  
    mDebugScreen(debugScreen){
    // register handlers
    mMainScreenDispatcher.registerHandler(sendActiveBehaviourChangedToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendDisplayStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFermentationStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendTemperatureSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendTemperatureStatusToMainScreen, this);
    mDebugScreenDispatcher.registerHandler(sendHeartbeatToDebugScreen, this);
    // init event headers
    const ComponentDescriptor descriptor { ComponentType::Replay, ComponentId::Replay };
    mHeartbeatBlock.header.type = EventType::Heartbeat;
    mHeartbeatBlock.header.sender = descriptor;
    mFermentationStatusBlock.header.type = EventType::FermentationStatus;
    mFermentationStatusBlock.header.sender = descriptor;
    mDisplayStatusBlock.header.type = EventType::DisplayStatus;
    mDisplayStatusBlock.header.sender = descriptor;
    mFanStatusBlock.header.type = EventType::FanStatus;
    mFanStatusBlock.header.sender = descriptor;
    mFanSampleBlock.header.type = EventType::FanSample;
    mFanSampleBlock.header.sender = descriptor;
    mHeatpadStatusBlock.header.type = EventType::HeatpadStatus;
    mHeatpadStatusBlock.header.sender = descriptor;
    mHeatpadSampleBlock.header.type = EventType::HeatpadSample;
    mHeatpadSampleBlock.header.sender = descriptor;
    mTemperatureStatusBlock.header.type = EventType::TemperatureStatus;
    mTemperatureStatusBlock.header.sender = descriptor;
    mTemperatureSampleBlock.header.type = EventType::TemperatureSample;
    mTemperatureSampleBlock.header.sender = descriptor;
    mActiveBehaviourChangedBlock.header.type = EventType::ActiveBehaviourChanged;
    mActiveBehaviourChangedBlock.header.sender = descriptor;
}

void EventReplay::storeEvent(const EventHeader* header){
    switch(header->type){
        case EventType::Heartbeat:
            mHeartbeatBlock.header.id = header->id;
            mHeartbeatBlock.payload = *static_cast<HeartbeatEvent>(header).payload();
            mDebugScreenDispatcher.markDirty(DebugScreen_Heartbeat_Index);
            break;
        case EventType::FermentationStatus:
            mFermentationStatusBlock.header.id = header->id;
            mFermentationStatusBlock.payload = *static_cast<FermentationStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FermentationStatus_Index);
            break;
        case EventType::DisplayStatus:
            mDisplayStatusBlock.header.id = header->id;
            mDisplayStatusBlock.payload = *static_cast<DisplayStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_DisplayStatus_Index);
            break;
        case EventType::FanStatus:
            mFanStatusBlock.header.id = header->id;
            mFanStatusBlock.payload = *static_cast<FanStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FanStatus_Index);
            break;
        case EventType::FanSample:
            mFanSampleBlock.header.id = header->id;
            mFanSampleBlock.payload = *static_cast<FanSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_FanSample_Index);
            break;
        case EventType::HeatpadStatus:
            mHeatpadStatusBlock.header.id = header->id;
            mHeatpadStatusBlock.payload = *static_cast<HeatpadStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadStatus_Index);
            break;
        case EventType::HeatpadSample:
            mHeatpadSampleBlock.header.id = header->id;
            mHeatpadSampleBlock.payload = *static_cast<HeatpadSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadSample_Index);
            break;
        case EventType::TemperatureStatus:
            mTemperatureStatusBlock.header.id = header->id;
            mTemperatureStatusBlock.payload = *static_cast<TemperatureStatusEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_TemperatureStatus_Index);
            break;
        case EventType::TemperatureSample:
            mTemperatureSampleBlock.header.id = header->id;
            mTemperatureSampleBlock.payload = *static_cast<TemperatureSampleEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_TemperatureSample_Index);
            break;
        case EventType::ActiveBehaviourChanged:
            mActiveBehaviourChangedBlock.header.id = header->id;
            mActiveBehaviourChangedBlock.payload = *static_cast<ActiveBehaviourChangedEvent>(header).payload();
            mMainScreenDispatcher.markDirty(MainScreen_ActiveBehaviourChanged_Index);
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
        default: TriggerExit("EventReplay", "invalid screen id");
    }
}

void EventReplay::sendActiveBehaviourChangedToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveActiveBehaviourChanged(&self->mActiveBehaviourChangedBlock.header); 
}

void EventReplay::sendDisplayStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveDisplayStatus(&self->mDisplayStatusBlock.header); 
}

void EventReplay::sendFanSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveFanSample(&self->mFanSampleBlock.header); 
}

void EventReplay::sendFanStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveFanStatus(&self->mFanStatusBlock.header); 
}

void EventReplay::sendFermentationStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveFermentationStatus(&self->mFermentationStatusBlock.header); 
}

void EventReplay::sendHeatpadSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveHeatpadSample(&self->mHeatpadSampleBlock.header); 
}

void EventReplay::sendHeatpadStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveHeatpadStatus(&self->mHeatpadStatusBlock.header); 
}

void EventReplay::sendTemperatureSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveTemperatureSample(&self->mTemperatureSampleBlock.header); 
}

void EventReplay::sendTemperatureStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.receiveTemperatureStatus(&self->mTemperatureStatusBlock.header); 
}

void EventReplay::sendHeartbeatToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mDebugScreen.onHeartbeat(&self->mHeartbeatBlock.header); 
}

} // namespace