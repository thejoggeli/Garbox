#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/controllers/heartbeat/HeartbeatControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class AnimatedLed;

class HeartbeatController : public HeartbeatControllerAbs {
private: 

    uint32_t mIntervalMicros;
    AnimatedLed& mHeartbeatLed;
    SoftwareTimer mHeartbeatTimer;

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    HeartbeatController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onHeartbeatTick() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace