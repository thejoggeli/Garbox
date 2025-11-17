#pragma once

#include "app/controllers/FanController.h"
#include "app/controllers/GarboxController.h"
#include "app/controllers/HeartbeatController.h"
#include "core/event/EventFactory.h"

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

    // controllers
    GarboxController mGarboxController;
    HeartbeatController mHeartbeatController;
    FanController mFanController;

};

}