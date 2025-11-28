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

    DirtyDispatcher mMainScreenDispatcher {4};
    DirtyDispatcher mDebugScreenDispatcher {1};

    FanStatusPayload mFanStatusShadow;
    FanSamplePayload mFanSampleShadow;
    HeatpadStatusPayload mHeatpadStatusShadow;
    HeatpadSamplePayload mHeatpadSampleShadow;

    static void sendFanSampleToMainScreen(void* context);
    static void sendFanStatusToMainScreen(void* context);
    static void sendHeatpadSampleToMainScreen(void* context);
    static void sendHeatpadStatusToMainScreen(void* context);
    static void sendFanStatusToDebugScreen(void* context);

};

} // namespace