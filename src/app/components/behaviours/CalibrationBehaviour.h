// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/generated/behaviours/CalibrationBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
public:

    CalibrationBehaviour();

    void onLogicTick() final;

    void onHeartbeatEvent(const HeartbeatEvent& event) final;
    void onFanStatusEvent(const FanStatusEvent& event) final;
    void onFanSampleEvent(const FanSampleEvent& event) final;

    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;

private:

    uint32_t mCounter = 0;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox