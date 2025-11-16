#pragma once

#include "app/controllers/FanController.h"
#include "app/controllers/GarboxController.h"
#include "app/controllers/HeartbeatController.h"

namespace Garbox {

class AppCore {
public:

    AppCore();

    void init();
    void start();
    void tick();

private:

    GarboxController mGarboxController;
    HeartbeatController mHeartbeatController;
    FanController mFanController;

};

}