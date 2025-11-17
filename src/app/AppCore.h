#pragma once

#include "app/controllers/DisplayController.h"
#include "app/controllers/FanController.h"
#include "app/controllers/GarboxController.h"
#include "app/controllers/HeartbeatController.h"
#include "app/controllers/HeatpadController.h"

#include "core/event/EventFactory.h"
#include "core/event/EventForwarder.h"

namespace Garbox {

class AppCore {
public:

    AppCore();

    void init();
    void start();
    void mainTick();
    void displayTick();

private:

    EventFactory mEventFactory;
    EventForwarder mEventForwarder;

    // controllers
    DisplayController mDisplayController;
    FanController mFanController;
    GarboxController mGarboxController;
    HeartbeatController mHeartbeatController;
    HeatpadController mHeatpadController;

    void handleForwardedEvent(const Event* event);
    void routeEvent(const Event* event);

};

}