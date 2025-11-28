#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventReplay.h"
#include "core/assert/Assert.h"

namespace Garbox {

static constexpr size_t MainScreen_FanSample_Index = 0 ;
static constexpr size_t MainScreen_FanStatus_Index = 1 ;
static constexpr size_t MainScreen_HeatpadSample_Index = 2 ;
static constexpr size_t MainScreen_HeatpadStatus_Index = 3 ;
static constexpr size_t DebugScreen_FanStatus_Index = 0 ;

EventReplay::EventReplay(
    // parameter list 
    MainScreenAbs& mainScreen,  
    DebugScreenAbs& debugScreen):
    // init members 
    mMainScreen(mainScreen),  
    mDebugScreen(debugScreen){
    // register handlers
    mMainScreenDispatcher.registerHandler(sendFanSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendFanStatusToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadSampleToMainScreen, this);
    mMainScreenDispatcher.registerHandler(sendHeatpadStatusToMainScreen, this);
    mDebugScreenDispatcher.registerHandler(sendFanStatusToDebugScreen, this);
}

void EventReplay::handleEvent(const EventHeader* header){
    switch(header->type){
        case EventType::FanStatus:
            mMainScreenDispatcher.markDirty(MainScreen_FanStatus_Index);
            mDebugScreenDispatcher.markDirty(DebugScreen_FanStatus_Index);
            break;
        case EventType::FanSample:
            mMainScreenDispatcher.markDirty(MainScreen_FanSample_Index);
            break;
        case EventType::HeatpadStatus:
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadStatus_Index);
            break;
        case EventType::HeatpadSample:
            mMainScreenDispatcher.markDirty(MainScreen_HeatpadSample_Index);
            break;
        default: // no screen listens to event type
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

void EventReplay::sendFanSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.updateFanSample(self->mFanSampleShadow); 
}

void EventReplay::sendFanStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.updateFanStatus(self->mFanStatusShadow); 
}

void EventReplay::sendHeatpadSampleToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.updateHeatpadSample(self->mHeatpadSampleShadow); 
}

void EventReplay::sendHeatpadStatusToMainScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mMainScreen.updateHeatpadStatus(self->mHeatpadStatusShadow); 
}

void EventReplay::sendFanStatusToDebugScreen(void* context){ 
    EventReplay* self = static_cast<EventReplay*>(context);
    self->mDebugScreen.updateFanStatus(self->mFanStatusShadow); 
}

} // namespace