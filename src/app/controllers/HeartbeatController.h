#pragma once

#include "app/controllers/generated/HeartbeatControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class AnimatedLed;

class HeartbeatController : public HeartbeatControllerAbs {
public:

    HeartbeatController(ComponentId id, const RuntimeContext& context);

    void onHeartbeatTick() final;

private: 

    void onInit();
    void onStart();

    uint32_t mIntervalMicros;
    AnimatedLed& mHeartbeatLed;
    SoftwareTimer mHeartbeatTimer;

};

} // namespace