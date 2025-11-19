#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class AnimatedLed;

class HeartbeatController : public ControllerAbs {
public:

    HeartbeatController(ComponentId id);

    void onTick();

private: 

    void onInit();
    void onStart();

    uint32_t mIntervalMicros;
    AnimatedLed& mHeartbeatLed;
    SoftwareTimer mHeartbeatTimer;

};

} // namespace