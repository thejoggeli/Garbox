#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/screens/MainScreen.h"
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
 
        MainScreenAbs& mainScreen 
    );

    void storeEvent(const EventHeader* header);
    void replay(ScreenId screenId);

private:

    MainScreenAbs& mMainScreen;

    DirtyDispatcher mMainScreenDispatcher {9};

    EventBlock<EventType::FermentationStatus> mFermentationStatusBlock;
    EventBlock<EventType::DisplayStatus> mDisplayStatusBlock;
    EventBlock<EventType::FanStatus> mFanStatusBlock;
    EventBlock<EventType::FanSample> mFanSampleBlock;
    EventBlock<EventType::HeatpadStatus> mHeatpadStatusBlock;
    EventBlock<EventType::HeatpadSample> mHeatpadSampleBlock;
    EventBlock<EventType::TemperatureStatus> mTemperatureStatusBlock;
    EventBlock<EventType::TemperatureSample> mTemperatureSampleBlock;
    EventBlock<EventType::ActiveBehaviourChanged> mActiveBehaviourChangedBlock;

    // send event to 'MainScreen' 
    static void sendActiveBehaviourChangedToMainScreen(void* context);
    static void sendDisplayStatusToMainScreen(void* context);
    static void sendFanSampleToMainScreen(void* context);
    static void sendFanStatusToMainScreen(void* context);
    static void sendFermentationStatusToMainScreen(void* context);
    static void sendHeatpadSampleToMainScreen(void* context);
    static void sendHeatpadStatusToMainScreen(void* context);
    static void sendTemperatureSampleToMainScreen(void* context);
    static void sendTemperatureStatusToMainScreen(void* context);

};

} // namespace