#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class AnimatedLed;

class HeartbeatController : public ControllerAbs {
public:

    HeartbeatController();

private: 


    void onInit();
    void onStart();
    void onTick();

    uint32_t mIntervalMicros;
    AnimatedLed& mHeartbeatLed;
    SoftwareTimer mHeartbeatTimer;

};

} // namespace