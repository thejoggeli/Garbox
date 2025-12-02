#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/behaviours/abstract/CalibrationBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
public:

    CalibrationBehaviour();

    // tick handlers
    void onLogicTick() final;

    // event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;
    virtual void onFanStatus(const FanStatusEvent& event) = 0;
    virtual void onFanSample(const FanSampleEvent& event) = 0;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox