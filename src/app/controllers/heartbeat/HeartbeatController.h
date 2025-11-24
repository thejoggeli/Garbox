#pragma once

#include "app/controllers/abstract/HeartbeatControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class AnimatedLed;

class HeartbeatController : public HeartbeatControllerAbs {
public:

    HeartbeatController();

    void onHeartbeatTick() final;

private: 

    void onInit();
    void onStart();

    uint32_t mIntervalMicros;
    AnimatedLed& mHeartbeatLed;
    SoftwareTimer mHeartbeatTimer;

};

} // namespace