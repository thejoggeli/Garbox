#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/behaviours/calibration/CalibrationBehaviourAbs.h"
// ==== GENERATED END: include ====================================================

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
private:

    uint32_t mCounter = 0;

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    CalibrationBehaviour();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated tick handlers
    void onLogicTick() final;

    // generated event handlers
    void onHeartbeatEvent(const HeartbeatEvent& event) final;

    // generated state changed handlers
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox