#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/screens/debug/DebugScreen.h"
#include "app/screens/eventlog/EventLogScreen.h"
#include "app/screens/main/MainScreen.h"
#include "core/application/screen/ScreenAbs.h"
#include "core/util/helpers/DirtyDispatcher.h"
#include "shared/types/EventAlias.h"
#include "shared/types/EventPayload.h"
#include "shared/types/EventType.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class EventReplay {
public:

    EventReplay(
 
        MainScreenAbs& mainScreen,  
        DebugScreenAbs& debugScreen 
    );

    void storeEvent(const EventHeader* header);
    void replay(ScreenId screenId);

private:

    MainScreenAbs& mMainScreen;
    DebugScreenAbs& mDebugScreen;

    DirtyDispatcher mMainScreenDispatcher {11};
    DirtyDispatcher mDebugScreenDispatcher {9};

    EventBlock<EventType::FermentationStatus> mFermentationStatusBlock;
    EventBlock<EventType::DisplayCommand> mDisplayCommandBlock;
    EventBlock<EventType::DisplayStatus> mDisplayStatusBlock;
    EventBlock<EventType::FanStatus> mFanStatusBlock;
    EventBlock<EventType::FanSample> mFanSampleBlock;
    EventBlock<EventType::FanCommand> mFanCommandBlock;
    EventBlock<EventType::HeatpadStatus> mHeatpadStatusBlock;
    EventBlock<EventType::HeatpadSample> mHeatpadSampleBlock;
    EventBlock<EventType::HeatpadCommand> mHeatpadCommandBlock;
    EventBlock<EventType::TemperatureStatus> mTemperatureStatusBlock;
    EventBlock<EventType::TemperatureSample> mTemperatureSampleBlock;
    EventBlock<EventType::ActiveBehaviourChanged> mActiveBehaviourChangedBlock;

    // send event to 'MainScreen' 
    static void sendDisplayCommandToMainScreen(void* context);
    static void sendDisplayStatusToMainScreen(void* context);
    static void sendFanCommandToMainScreen(void* context);
    static void sendFanSampleToMainScreen(void* context);
    static void sendFanStatusToMainScreen(void* context);
    static void sendFermentationStatusToMainScreen(void* context);
    static void sendHeatpadCommandToMainScreen(void* context);
    static void sendHeatpadSampleToMainScreen(void* context);
    static void sendHeatpadStatusToMainScreen(void* context);
    static void sendTemperatureSampleToMainScreen(void* context);
    static void sendTemperatureStatusToMainScreen(void* context);

    // send event to 'DebugScreen' 
    static void sendActiveBehaviourChangedToDebugScreen(void* context);
    static void sendDisplayStatusToDebugScreen(void* context);
    static void sendFanSampleToDebugScreen(void* context);
    static void sendFanStatusToDebugScreen(void* context);
    static void sendFermentationStatusToDebugScreen(void* context);
    static void sendHeatpadSampleToDebugScreen(void* context);
    static void sendHeatpadStatusToDebugScreen(void* context);
    static void sendTemperatureSampleToDebugScreen(void* context);
    static void sendTemperatureStatusToDebugScreen(void* context);

};

} // namespace