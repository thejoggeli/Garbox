// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/behaviours/abstract/CalibrationBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
public:

    CalibrationBehaviour();

    void onLogicTick() final;

    void onHeartbeat(const HeartbeatEvent& event) final;
    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;

private:

    uint32_t mCounter = 0;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox