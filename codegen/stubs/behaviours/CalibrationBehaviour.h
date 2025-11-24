#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/behaviours/generated/CalibrationBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
public:

    CalibrationBehaviour();

    void onLogicTick() final;

    void onHeartbeat(const HeartbeatEvent& event) final;
    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox