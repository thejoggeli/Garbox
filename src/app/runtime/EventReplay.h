#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/screens/MainScreen.h"
#include "app/screens/DebugScreen.h"
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

    void handleEvent(const EventHeader* header);
    void replay(ScreenId screenId);

private:

    MainScreenAbs& mMainScreen;
    DebugScreenAbs& mDebugScreen;

    DirtyDispatcher mMainScreenDispatcher {10};
    DirtyDispatcher mDebugScreenDispatcher {1};

    EventBlock<EventType::Heartbeat> mHeartbeatBlock;
    EventBlock<EventType::FermentationStatus> mFermentationStatusBlock;
    EventBlock<EventType::DisplayCommand> mDisplayCommandBlock;
    EventBlock<EventType::DisplayStatus> mDisplayStatusBlock;
    EventBlock<EventType::FanStatus> mFanStatusBlock;
    EventBlock<EventType::FanSample> mFanSampleBlock;
    EventBlock<EventType::HeatpadStatus> mHeatpadStatusBlock;
    EventBlock<EventType::HeatpadSample> mHeatpadSampleBlock;
    EventBlock<EventType::TemperatureStatus> mTemperatureStatusBlock;
    EventBlock<EventType::TemperatureSample> mTemperatureSampleBlock;
    EventBlock<EventType::ActiveBehaviourChanged> mActiveBehaviourChangedBlock;

    static void sendFanStatusToMainScreen(void* context);
    static void sendFanSampleToMainScreen(void* context);
    static void sendHeatpadStatusToMainScreen(void* context);
    static void sendHeatpadSampleToMainScreen(void* context);
    static void sendTemperatureStatusToMainScreen(void* context);
    static void sendTemperatureSampleToMainScreen(void* context);
    static void sendDisplayCommandToMainScreen(void* context);
    static void sendActiveBehaviourChangedToMainScreen(void* context);
    static void sendFermentationStatusToMainScreen(void* context);
    static void sendDisplayStatusToMainScreen(void* context);
    static void sendHeartbeatToDebugScreen(void* context);

};

} // namespace